/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2004, 2010, Oracle and/or its affiliates. All rights reserved.
 */

#include <sys/sysmacros.h>
#include <sys/socket.h>
#include <sys/ddi.h>
#include <sys/sunddi.h>

#include <netinet/in.h>
#include <netinet/ip6.h>

#include <inet/common.h>
#include <inet/ip.h>
#include <inet/ip6.h>
#include <inet/ipclassifier.h>
#include <inet/ipsec_impl.h>
#include <inet/ipp_common.h>
#include <inet/sctp_ip.h>

#include <inet/sctp/sctp_impl.h>
#include <inet/sctp/sctp_addr.h>

/* Default association hash size.  The size must be a power of 2. */
#define	SCTP_CONN_HASH_SIZE	8192

uint_t		sctp_conn_hash_size = SCTP_CONN_HASH_SIZE; /* /etc/system */

void
sctp_hash_init(sctp_stack_t *sctps)
{
	int i;

	/* Start with /etc/system value */
	sctps->sctps_conn_hash_size = sctp_conn_hash_size;

	if (!ISP2(sctps->sctps_conn_hash_size)) {
		/* Not a power of two. Round up to nearest power of two */
		for (i = 0; i < 31; i++) {
			if (sctps->sctps_conn_hash_size < (1 << i))
				break;
		}
		sctps->sctps_conn_hash_size = 1 << i;
	}
	if (sctps->sctps_conn_hash_size < SCTP_CONN_HASH_SIZE) {
		sctps->sctps_conn_hash_size = SCTP_CONN_HASH_SIZE;
		cmn_err(CE_CONT, "using sctp_conn_hash_size = %u\n",
		    sctps->sctps_conn_hash_size);
	}
	sctps->sctps_conn_fanout =
	    (sctp_tf_t *)kmem_zalloc(sctps->sctps_conn_hash_size *
	    sizeof (sctp_tf_t), KM_SLEEP);
	for (i = 0; i < sctps->sctps_conn_hash_size; i++) {
		mutex_init(&sctps->sctps_conn_fanout[i].tf_lock, NULL,
		    MUTEX_DEFAULT, NULL);
	}
	sctps->sctps_listen_fanout = kmem_zalloc(SCTP_LISTEN_FANOUT_SIZE *
	    sizeof (sctp_tf_t),	KM_SLEEP);
	for (i = 0; i < SCTP_LISTEN_FANOUT_SIZE; i++) {
		mutex_init(&sctps->sctps_listen_fanout[i].tf_lock, NULL,
		    MUTEX_DEFAULT, NULL);
	}
	sctps->sctps_bind_fanout = kmem_zalloc(SCTP_BIND_FANOUT_SIZE *
	    sizeof (sctp_tf_t),	KM_SLEEP);
	for (i = 0; i < SCTP_BIND_FANOUT_SIZE; i++) {
		mutex_init(&sctps->sctps_bind_fanout[i].tf_lock, NULL,
		    MUTEX_DEFAULT, NULL);
	}
}

void
sctp_hash_destroy(sctp_stack_t *sctps)
{
	int i;

	for (i = 0; i < sctps->sctps_conn_hash_size; i++) {
		mutex_destroy(&sctps->sctps_conn_fanout[i].tf_lock);
	}
	kmem_free(sctps->sctps_conn_fanout, sctps->sctps_conn_hash_size *
	    sizeof (sctp_tf_t));
	sctps->sctps_conn_fanout = NULL;

	for (i = 0; i < SCTP_LISTEN_FANOUT_SIZE; i++) {
		mutex_destroy(&sctps->sctps_listen_fanout[i].tf_lock);
	}
	kmem_free(sctps->sctps_listen_fanout, SCTP_LISTEN_FANOUT_SIZE *
	    sizeof (sctp_tf_t));
	sctps->sctps_listen_fanout = NULL;

	for (i = 0; i < SCTP_BIND_FANOUT_SIZE; i++) {
		mutex_destroy(&sctps->sctps_bind_fanout[i].tf_lock);
	}
	kmem_free(sctps->sctps_bind_fanout, SCTP_BIND_FANOUT_SIZE *
	    sizeof (sctp_tf_t));
	sctps->sctps_bind_fanout = NULL;
}

sctp_t *
sctp_conn_match(in6_addr_t **faddrpp, uint32_t nfaddr, in6_addr_t *laddr,
    uint32_t ports, zoneid_t zoneid, iaflags_t iraflags, sctp_stack_t *sctps)
{
	sctp_tf_t		*tf;
	sctp_t			*sctp;
	sctp_faddr_t		*fp;
	conn_t			*connp;
	in6_addr_t		**faddrs, **endaddrs = &faddrpp[nfaddr];

	tf = &(sctps->sctps_conn_fanout[SCTP_CONN_HASH(sctps, ports)]);
	mutex_enter(&tf->tf_lock);

	for (sctp = tf->tf_sctp; sctp != NULL; sctp =
	    sctp->sctp_conn_hash_next) {
		connp = sctp->sctp_connp;
		if (ports != connp->conn_ports)
			continue;
		if (!(connp->conn_zoneid == zoneid ||
		    connp->conn_allzones))
			continue;

		/* check for faddr match */
		for (fp = sctp->sctp_faddrs; fp != NULL; fp = fp->sf_next) {
			for (faddrs = faddrpp; faddrs < endaddrs; faddrs++) {
				if (IN6_ARE_ADDR_EQUAL(*faddrs,
				    &fp->sf_faddr)) {
					/* check for laddr match */
					if (sctp_saddr_lookup(sctp, laddr, 0)
					    != NULL) {
						SCTP_REFHOLD(sctp);
						mutex_exit(&tf->tf_lock);
						return (sctp);
					}
				}
			}
		}

		/* no match; continue to the next in the chain */
	}

	mutex_exit(&tf->tf_lock);
	return (sctp);
}

static sctp_t *
listen_match(in6_addr_t *laddr, uint32_t ports, zoneid_t zoneid,
    iaflags_t iraflags, sctp_stack_t *sctps)
{
	sctp_t			*sctp;
	sctp_tf_t		*tf;
	uint16_t		lport;
	conn_t			*connp;

	lport = ((uint16_t *)&ports)[1];

	tf = &(sctps->sctps_listen_fanout[SCTP_LISTEN_HASH(ntohs(lport))]);
	mutex_enter(&tf->tf_lock);

	for (sctp = tf->tf_sctp; sctp; sctp = sctp->sctp_listen_hash_next) {
		connp = sctp->sctp_connp;
		if (lport != connp->conn_lport)
			continue;

		if (!(connp->conn_zoneid == zoneid ||
		    connp->conn_allzones))
			continue;

		if (sctp_saddr_lookup(sctp, laddr, 0) != NULL) {
			SCTP_REFHOLD(sctp);
			goto done;
		}
		/* no match; continue to the next in the chain */
	}

done:
	mutex_exit(&tf->tf_lock);
	return (sctp);
}

/* called by ipsec_sctp_pol */
conn_t *
sctp_find_conn(in6_addr_t *src, in6_addr_t *dst, uint32_t ports,
    zoneid_t zoneid, iaflags_t iraflags, sctp_stack_t *sctps)
{
	sctp_t *sctp;

	sctp = sctp_conn_match(&src, 1, dst, ports, zoneid, iraflags, sctps);
	if (sctp == NULL) {
		/* Not in conn fanout; check listen fanout */
		sctp = listen_match(dst, ports, zoneid, iraflags, sctps);
		if (sctp == NULL)
			return (NULL);
	}
	return (sctp->sctp_connp);
}

/*
 * This is called from sctp_fanout() with IP header src & dst addresses.
 * First call sctp_conn_match() to get a match by passing in src & dst
 * addresses from IP header.
 * However sctp_conn_match() can return no match under condition such as :
 * A host can send an INIT ACK from a different address than the INIT was sent
 * to (in a multi-homed env).
 * According to RFC4960, a host can send additional addresses in an INIT
 * ACK chunk.
 * Therefore extract all addresses from the INIT ACK chunk, pass to
 * sctp_conn_match() to get a match.
 */
static sctp_t *
sctp_lookup_by_faddrs(mblk_t *mp, sctp_hdr_t *sctph, in6_addr_t *srcp,
    in6_addr_t *dstp, uint32_t ports, zoneid_t zoneid, sctp_stack_t *sctps,
    iaflags_t iraflags)
{
	sctp_t			*sctp;
	sctp_chunk_hdr_t	*ich;
	sctp_init_chunk_t	*iack;
	sctp_parm_hdr_t		*ph;
	ssize_t			mlen, remaining;
	uint16_t		param_type, addr_len = PARM_ADDR4_LEN;
	in6_addr_t		src;
	in6_addr_t		**addrbuf = NULL, **faddrpp = NULL;
	boolean_t		isv4;
	uint32_t		totaddr, nfaddr = 0;

	/*
	 * If we get a match with the passed-in IP header src & dst addresses,
	 * quickly return the matched sctp.
	 */
	if ((sctp = sctp_conn_match(&srcp, 1, dstp, ports, zoneid, iraflags,
	    sctps)) != NULL) {
		return (sctp);
	}

	/*
	 * Currently sctph is set to NULL in icmp error fanout case
	 * (ip_fanout_sctp()).
	 * The above sctp_conn_match() should handle that, otherwise
	 * return no match found.
	 */
	if (sctph == NULL)
		return (NULL);

	/*
	 * Do a pullup again in case the previous one was partially successful,
	 * so try to complete the pullup here and have a single contiguous
	 * chunk for processing of entire INIT ACK chunk below.
	 */
	if (mp->b_cont != NULL) {
		if (pullupmsg(mp, -1) == 0) {
			return (NULL);
		}
	}

	mlen = mp->b_wptr - (uchar_t *)(sctph + 1);
	if ((ich = sctp_first_chunk((uchar_t *)(sctph + 1), mlen)) == NULL) {
		return (NULL);
	}

	if (ich->sch_id == CHUNK_INIT_ACK) {
		remaining = ntohs(ich->sch_len) - sizeof (*ich) -
		    sizeof (*iack);
		if (remaining < sizeof (*ph)) {
			return (NULL);
		}

		isv4 = (iraflags & IRAF_IS_IPV4) ? B_TRUE : B_FALSE;
		if (!isv4)
			addr_len = PARM_ADDR6_LEN;
		totaddr = remaining/addr_len;

		iack = (sctp_init_chunk_t *)(ich + 1);
		ph = (sctp_parm_hdr_t *)(iack + 1);

		addrbuf = (in6_addr_t **)
		    kmem_zalloc(totaddr * sizeof (in6_addr_t *), KM_NOSLEEP);
		if (addrbuf == NULL)
			return (NULL);
		faddrpp = addrbuf;

		while (ph != NULL) {
			/*
			 * According to RFC4960 :
			 * All integer fields in an SCTP packet MUST be
			 * transmitted in network byte order,
			 * unless otherwise stated.
			 * Therefore convert the param type to host byte order.
			 * Also do not add src address present in IP header
			 * as it has already been thru sctp_conn_match() above.
			 */
			param_type = ntohs(ph->sph_type);
			switch (param_type) {
			case PARM_ADDR4:
				IN6_INADDR_TO_V4MAPPED((struct in_addr *)
				    (ph + 1), &src);
				if (IN6_ARE_ADDR_EQUAL(&src, srcp))
					break;
				*faddrpp = (in6_addr_t *)
				    kmem_zalloc(sizeof (in6_addr_t),
				    KM_NOSLEEP);
				if (*faddrpp == NULL)
					break;
				IN6_INADDR_TO_V4MAPPED((struct in_addr *)
				    (ph + 1), *faddrpp);
				nfaddr++;
				faddrpp++;
				break;
			case PARM_ADDR6:
				*faddrpp = (in6_addr_t *)(ph + 1);
				if (IN6_ARE_ADDR_EQUAL(*faddrpp, srcp))
					break;
				nfaddr++;
				faddrpp++;
				break;
			default:
				break;
			}
			ph = sctp_next_parm(ph, &remaining);
		}

		ASSERT(nfaddr < totaddr);

		if (nfaddr > 0) {
			sctp = sctp_conn_match(addrbuf, nfaddr, dstp, ports,
			    zoneid, iraflags, sctps);

			if (isv4) {
				for (faddrpp = addrbuf; nfaddr > 0;
				    faddrpp++, nfaddr--) {
					if (IN6_IS_ADDR_V4MAPPED(*faddrpp)) {
						kmem_free(*faddrpp,
						    sizeof (in6_addr_t));
					}
				}
			}
		}
		kmem_free(addrbuf, totaddr * sizeof (in6_addr_t *));
	}
	return (sctp);
}

/*
 * Fanout to a sctp instance.
 */
conn_t *
sctp_fanout(in6_addr_t *src, in6_addr_t *dst, uint32_t ports,
    ip_recv_attr_t *ira, mblk_t *mp, sctp_stack_t *sctps, sctp_hdr_t *sctph)
{
	zoneid_t zoneid = ira->ira_zoneid;
	iaflags_t iraflags = ira->ira_flags;
	sctp_t *sctp;

	sctp = sctp_lookup_by_faddrs(mp, sctph, src, dst, ports, zoneid,
	    sctps, iraflags);
	if (sctp == NULL) {
		/* Not in conn fanout; check listen fanout */
		sctp = listen_match(dst, ports, zoneid, iraflags, sctps);
		if (sctp == NULL)
			return (NULL);
	}
	/*
	 * For labeled systems, there's no need to check the
	 * label here.  It's known to be good as we checked
	 * before allowing the connection to become bound.
	 */
	return (sctp->sctp_connp);
}

/*
 * Fanout for ICMP errors for SCTP
 * The caller puts <fport, lport> in the ports parameter.
 */
void
ip_fanout_sctp(mblk_t *mp, ipha_t *ipha, ip6_t *ip6h, uint32_t ports,
    ip_recv_attr_t *ira)
{
	sctp_t		*sctp;
	conn_t		*connp;
	in6_addr_t	map_src, map_dst;
	in6_addr_t	*src, *dst;
	boolean_t	secure;
	ill_t		*ill = ira->ira_ill;
	ip_stack_t	*ipst = ill->ill_ipst;
	netstack_t	*ns = ipst->ips_netstack;
	ipsec_stack_t	*ipss = ns->netstack_ipsec;
	sctp_stack_t	*sctps = ns->netstack_sctp;
	iaflags_t	iraflags = ira->ira_flags;
	ill_t		*rill = ira->ira_rill;

	ASSERT(iraflags & IRAF_ICMP_ERROR);

	secure = iraflags & IRAF_IPSEC_SECURE;

	/* Assume IP provides aligned packets - otherwise toss */
	if (!OK_32PTR(mp->b_rptr)) {
		BUMP_MIB(ill->ill_ip_mib, ipIfStatsInDiscards);
		ip_drop_input("ipIfStatsInDiscards", mp, ill);
		freemsg(mp);
		return;
	}

	if (!(iraflags & IRAF_IS_IPV4)) {
		src = &ip6h->ip6_src;
		dst = &ip6h->ip6_dst;
	} else {
		IN6_IPADDR_TO_V4MAPPED(ipha->ipha_src, &map_src);
		IN6_IPADDR_TO_V4MAPPED(ipha->ipha_dst, &map_dst);
		src = &map_src;
		dst = &map_dst;
	}
	connp = sctp_fanout(src, dst, ports, ira, mp, sctps, NULL);
	if (connp == NULL) {
		ip_fanout_sctp_raw(mp, ipha, ip6h, ports, ira);
		return;
	}
	sctp = CONN2SCTP(connp);

	/*
	 * We check some fields in conn_t without holding a lock.
	 * This should be fine.
	 */
	if (((iraflags & IRAF_IS_IPV4) ?
	    CONN_INBOUND_POLICY_PRESENT(connp, ipss) :
	    CONN_INBOUND_POLICY_PRESENT_V6(connp, ipss)) ||
	    secure) {
		mp = ipsec_check_inbound_policy(mp, connp, ipha,
		    ip6h, ira);
		if (mp == NULL) {
			SCTP_REFRELE(sctp);
			return;
		}
	}

	ira->ira_ill = ira->ira_rill = NULL;

	mutex_enter(&sctp->sctp_lock);
	if (sctp->sctp_running) {
		sctp_add_recvq(sctp, mp, B_FALSE, ira);
		mutex_exit(&sctp->sctp_lock);
	} else {
		sctp->sctp_running = B_TRUE;
		mutex_exit(&sctp->sctp_lock);

		mutex_enter(&sctp->sctp_recvq_lock);
		if (sctp->sctp_recvq != NULL) {
			sctp_add_recvq(sctp, mp, B_TRUE, ira);
			mutex_exit(&sctp->sctp_recvq_lock);
			WAKE_SCTP(sctp);
		} else {
			mutex_exit(&sctp->sctp_recvq_lock);
			if (ira->ira_flags & IRAF_ICMP_ERROR) {
				sctp_icmp_error(sctp, mp);
			} else {
				sctp_input_data(sctp, mp, ira);
			}
			WAKE_SCTP(sctp);
		}
	}
	SCTP_REFRELE(sctp);
	ira->ira_ill = ill;
	ira->ira_rill = rill;
}

void
sctp_conn_hash_remove(sctp_t *sctp)
{
	sctp_tf_t *tf = sctp->sctp_conn_tfp;

	if (!tf) {
		return;
	}

	mutex_enter(&tf->tf_lock);
	ASSERT(tf->tf_sctp);
	if (tf->tf_sctp == sctp) {
		tf->tf_sctp = sctp->sctp_conn_hash_next;
		if (sctp->sctp_conn_hash_next) {
			ASSERT(tf->tf_sctp->sctp_conn_hash_prev == sctp);
			tf->tf_sctp->sctp_conn_hash_prev = NULL;
		}
	} else {
		ASSERT(sctp->sctp_conn_hash_prev);
		ASSERT(sctp->sctp_conn_hash_prev->sctp_conn_hash_next == sctp);
		sctp->sctp_conn_hash_prev->sctp_conn_hash_next =
		    sctp->sctp_conn_hash_next;

		if (sctp->sctp_conn_hash_next) {
			ASSERT(sctp->sctp_conn_hash_next->sctp_conn_hash_prev
			    == sctp);
			sctp->sctp_conn_hash_next->sctp_conn_hash_prev =
			    sctp->sctp_conn_hash_prev;
		}
	}
	sctp->sctp_conn_hash_next = NULL;
	sctp->sctp_conn_hash_prev = NULL;
	sctp->sctp_conn_tfp = NULL;
	mutex_exit(&tf->tf_lock);
}

void
sctp_conn_hash_insert(sctp_tf_t *tf, sctp_t *sctp, int caller_holds_lock)
{
	if (sctp->sctp_conn_tfp) {
		sctp_conn_hash_remove(sctp);
	}

	if (!caller_holds_lock) {
		mutex_enter(&tf->tf_lock);
	} else {
		ASSERT(MUTEX_HELD(&tf->tf_lock));
	}

	sctp->sctp_conn_hash_next = tf->tf_sctp;
	if (tf->tf_sctp) {
		tf->tf_sctp->sctp_conn_hash_prev = sctp;
	}
	sctp->sctp_conn_hash_prev = NULL;
	tf->tf_sctp = sctp;
	sctp->sctp_conn_tfp = tf;
	if (!caller_holds_lock) {
		mutex_exit(&tf->tf_lock);
	}
}

void
sctp_listen_hash_remove(sctp_t *sctp)
{
	sctp_tf_t *tf = sctp->sctp_listen_tfp;

	if (!tf) {
		return;
	}

	mutex_enter(&tf->tf_lock);
	ASSERT(tf->tf_sctp);
	if (tf->tf_sctp == sctp) {
		tf->tf_sctp = sctp->sctp_listen_hash_next;
		if (sctp->sctp_listen_hash_next != NULL) {
			ASSERT(tf->tf_sctp->sctp_listen_hash_prev == sctp);
			tf->tf_sctp->sctp_listen_hash_prev = NULL;
		}
	} else {
		ASSERT(sctp->sctp_listen_hash_prev);
		ASSERT(sctp->sctp_listen_hash_prev->sctp_listen_hash_next ==
		    sctp);
		ASSERT(sctp->sctp_listen_hash_next == NULL ||
		    sctp->sctp_listen_hash_next->sctp_listen_hash_prev == sctp);

		sctp->sctp_listen_hash_prev->sctp_listen_hash_next =
		    sctp->sctp_listen_hash_next;

		if (sctp->sctp_listen_hash_next != NULL) {
			sctp_t *next = sctp->sctp_listen_hash_next;

			ASSERT(next->sctp_listen_hash_prev == sctp);
			next->sctp_listen_hash_prev =
			    sctp->sctp_listen_hash_prev;
		}
	}
	sctp->sctp_listen_hash_next = NULL;
	sctp->sctp_listen_hash_prev = NULL;
	sctp->sctp_listen_tfp = NULL;
	mutex_exit(&tf->tf_lock);
}

void
sctp_listen_hash_insert(sctp_tf_t *tf, sctp_t *sctp)
{
	if (sctp->sctp_listen_tfp) {
		sctp_listen_hash_remove(sctp);
	}

	mutex_enter(&tf->tf_lock);
	sctp->sctp_listen_hash_next = tf->tf_sctp;
	if (tf->tf_sctp) {
		tf->tf_sctp->sctp_listen_hash_prev = sctp;
	}
	sctp->sctp_listen_hash_prev = NULL;
	tf->tf_sctp = sctp;
	sctp->sctp_listen_tfp = tf;
	mutex_exit(&tf->tf_lock);
}

/*
 * Hash list insertion routine for sctp_t structures.
 * Inserts entries with the ones bound to a specific IP address first
 * followed by those bound to INADDR_ANY.
 */
void
sctp_bind_hash_insert(sctp_tf_t *tbf, sctp_t *sctp, int caller_holds_lock)
{
	sctp_t	**sctpp;
	sctp_t	*sctpnext;

	if (sctp->sctp_ptpbhn != NULL) {
		ASSERT(!caller_holds_lock);
		sctp_bind_hash_remove(sctp);
	}
	sctpp = &tbf->tf_sctp;
	if (!caller_holds_lock) {
		mutex_enter(&tbf->tf_lock);
	} else {
		ASSERT(MUTEX_HELD(&tbf->tf_lock));
	}
	sctpnext = sctpp[0];
	if (sctpnext) {
		sctpnext->sctp_ptpbhn = &sctp->sctp_bind_hash;
	}
	sctp->sctp_bind_hash = sctpnext;
	sctp->sctp_ptpbhn = sctpp;
	sctpp[0] = sctp;
	/* For sctp_*_hash_remove */
	sctp->sctp_bind_lockp = &tbf->tf_lock;
	if (!caller_holds_lock)
		mutex_exit(&tbf->tf_lock);
}

/*
 * Hash list removal routine for sctp_t structures.
 */
void
sctp_bind_hash_remove(sctp_t *sctp)
{
	sctp_t	*sctpnext;
	kmutex_t *lockp;

	lockp = sctp->sctp_bind_lockp;

	if (sctp->sctp_ptpbhn == NULL)
		return;

	ASSERT(lockp != NULL);
	mutex_enter(lockp);
	if (sctp->sctp_ptpbhn) {
		sctpnext = sctp->sctp_bind_hash;
		if (sctpnext) {
			sctpnext->sctp_ptpbhn = sctp->sctp_ptpbhn;
			sctp->sctp_bind_hash = NULL;
		}
		*sctp->sctp_ptpbhn = sctpnext;
		sctp->sctp_ptpbhn = NULL;
	}
	mutex_exit(lockp);
	sctp->sctp_bind_lockp = NULL;
}

/*
 * Similar to but different from sctp_conn_match().
 *
 * Matches sets of addresses as follows: if the argument addr set is
 * a complete subset of the corresponding addr set in the sctp_t, it
 * is a match.
 *
 * Caller must hold tf->tf_lock.
 *
 * Returns with a SCTP_REFHOLD sctp structure. Caller must do a SCTP_REFRELE.
 */
sctp_t *
sctp_lookup(sctp_t *sctp1, in6_addr_t *faddr, sctp_tf_t *tf, uint32_t *ports,
    int min_state)
{
	sctp_t *sctp;
	sctp_faddr_t *fp;

	ASSERT(MUTEX_HELD(&tf->tf_lock));

	for (sctp = tf->tf_sctp; sctp != NULL;
	    sctp = sctp->sctp_conn_hash_next) {
		if (*ports != sctp->sctp_connp->conn_ports ||
		    sctp->sctp_state < min_state) {
			continue;
		}

		/* check for faddr match */
		for (fp = sctp->sctp_faddrs; fp != NULL; fp = fp->sf_next) {
			if (IN6_ARE_ADDR_EQUAL(faddr, &fp->sf_faddr)) {
				break;
			}
		}

		if (fp == NULL) {
			/* no faddr match; keep looking */
			continue;
		}

		/*
		 * There is an existing association with the same peer
		 * address.  So now we need to check if our local address
		 * set overlaps with the one of the existing association.
		 * If they overlap, we should return it.
		 */
		if (sctp_compare_saddrs(sctp1, sctp) <= SCTP_ADDR_OVERLAP) {
			goto done;
		}

		/* no match; continue searching */
	}

done:
	if (sctp != NULL) {
		SCTP_REFHOLD(sctp);
	}
	return (sctp);
}
