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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */


/*
 * Bridge module containing functions with different names than those in the
 * usba10 module, so the usba10 module can call functions in this (usba) module.
 *
 * The usba10 module is present to satisfy a runtime loader dependency for
 * legacy DDK drivers (V0.8).  The usba10 module calls functions in this (usba)
 * module to carry out driver requests.
 *
 * The intent is that this file disappear once krtld functionality is improved
 * so that drivers dependent on usba10 can call into usba without these wrapper
 * functions.
 *
 * Please see the on81-patch gate include/sys/usba10/usba10_usbai.h
 * header file for descriptions and comments for these functions.
 */

#include <sys/usb/usba.h>
#include <sys/usb/usba/usbai_private.h>
#include <sys/usb/usba/usba10.h>

#ifndef __lock_lint

int
usba10_usb_register_client(
	dev_info_t			*dip,
	uint_t				version,
	usb_client_dev_data_t		**dev_data,
	usb_reg_parse_lvl_t		parse_level,
	usb_flags_t			flags)
{
	return (usb_register_client(
	    dip, version, dev_data, parse_level, flags));
}


void
usba10_usb_unregister_client(
	dev_info_t			*dip,
	usb_client_dev_data_t		*dev_data)
{
	usb_unregister_client(dip, dev_data);
}


void
usba10_usb_free_descr_tree(
	dev_info_t			*dip,
	usb_client_dev_data_t		*dev_data)
{
	usb_free_descr_tree(dip, dev_data);
}


size_t
usba10_usb_parse_data(
	char			*format,
	uchar_t 		*data,
	size_t			datalen,
	void			*structure,
	size_t			structlen)
{
	return (usb_parse_data(
	    format, data, datalen, structure, structlen));
}

usb_ep_data_t *
usba10_usb_get_ep_data(
	dev_info_t		*dip,
	usb_client_dev_data_t	*dev_datap,
	uint_t			interface,
	uint_t			alternate,
	uint_t			type,
	uint_t			direction)
{
	return (usb_get_ep_data(
	    dip, dev_datap, interface, alternate, type, direction));
}


int
usba10_usb_get_string_descr(
	dev_info_t		*dip,
	uint16_t		langid,
	uint8_t			index,
	char			*buf,
	size_t			buflen)
{
	return (usb_get_string_descr(dip, langid, index, buf, buflen));
}


int
usba10_usb_get_addr(dev_info_t *dip)
{
	return (usb_get_addr(dip));
}


int
usba10_usb_get_if_number(dev_info_t *dip)
{
	return (usb_get_if_number(dip));
}


boolean_t
usba10_usb_owns_device(dev_info_t *dip)
{
	return (usb_owns_device(dip));
}


int
usba10_usb_pipe_get_state(
	usb_pipe_handle_t	pipe_handle,
	usb_pipe_state_t	*pipe_state,
	usb_flags_t		flags)
{
	return (usb_pipe_get_state(pipe_handle, pipe_state, flags));
}


int
usba10_usb_ep_num(usb_pipe_handle_t ph)
{
	return (usb_ep_num(ph));
}


int
usba10_usb_pipe_open(
	dev_info_t		*dip,
	usb_ep_descr_t		*ep,
	usb_pipe_policy_t	*pipe_policy,
	usb_flags_t		flags,
	usb_pipe_handle_t	*pipe_handle)
{
	return (usb_pipe_open(dip, ep, pipe_policy, flags, pipe_handle));
}


void
usba10_usb_pipe_close(
	dev_info_t		*dip,
	usb_pipe_handle_t	pipe_handle,
	usb_flags_t		flags,
	void			(*cb)(
				    usb_pipe_handle_t	ph,
				    usb_opaque_t	arg,	/* cb arg */
				    int			rval,
				    usb_cb_flags_t	flags),
	usb_opaque_t		cb_arg)
{
	usb_pipe_close(dip, pipe_handle, flags, cb, cb_arg);
}


int
usba10_usb_pipe_drain_reqs(
	dev_info_t		*dip,
	usb_pipe_handle_t	pipe_handle,
	uint_t			time,
	usb_flags_t		flags,
	void			(*cb)(
				    usb_pipe_handle_t	ph,
				    usb_opaque_t	arg,	/* cb arg */
				    int			rval,
				    usb_cb_flags_t	flags),
	usb_opaque_t		cb_arg)
{
	return (usb_pipe_drain_reqs(
	    dip, pipe_handle, time, flags, cb, cb_arg));
}


int
usba10_usb_pipe_set_private(
	usb_pipe_handle_t	pipe_handle,
	usb_opaque_t		data)
{
	return (usb_pipe_set_private(pipe_handle, data));
}


usb_opaque_t
usba10_usb_pipe_get_private(usb_pipe_handle_t pipe_handle)
{
	return (usb_pipe_get_private(pipe_handle));
}


void
usba10_usb_pipe_reset(
	dev_info_t		*dip,
	usb_pipe_handle_t	pipe_handle,
	usb_flags_t		usb_flags,
	void			(*cb)(
					usb_pipe_handle_t ph,
					usb_opaque_t	arg,
					int		rval,
					usb_cb_flags_t	flags),
	usb_opaque_t		cb_arg)
{
	usb_pipe_reset(dip, pipe_handle, usb_flags, cb, cb_arg);
}


usb_ctrl_req_t *
usba10_usb_alloc_ctrl_req(
	dev_info_t		*dip,
	size_t			len,
	usb_flags_t		flags)
{
	return (usb_alloc_ctrl_req(dip, len, flags));
}


void
usba10_usb_free_ctrl_req(usb_ctrl_req_t *reqp)
{
	usb_free_ctrl_req(reqp);
}


int
usba10_usb_pipe_ctrl_xfer(
	usb_pipe_handle_t	pipe_handle,
	usb_ctrl_req_t		*reqp,
	usb_flags_t		flags)
{
	return (usb_pipe_ctrl_xfer(pipe_handle, reqp, flags));
}


int
usba10_usb_get_status(
	dev_info_t		*dip,
	usb_pipe_handle_t	ph,
	uint_t			type,	/* bmRequestType */
	uint_t			what,	/* 0, interface, endpoint number */
	uint16_t		*status,
	usb_flags_t		flags)
{
	return (usb_get_status(dip, ph, type, what, status, flags));
}


int
usba10_usb_clear_feature(
	dev_info_t		*dip,
	usb_pipe_handle_t	ph,
	uint_t			type,	/* bmRequestType */
	uint_t			feature,
	uint_t			what,	/* 0, interface, endpoint number */
	usb_flags_t		flags)
{
	return (usb_clear_feature(dip, ph, type, feature, what, flags));
}


int
usba10_usb_pipe_ctrl_xfer_wait(
	usb_pipe_handle_t	pipe_handle,
	usb_ctrl_setup_t	*setup,
	mblk_t			**data,
	usb_cr_t		*completion_reason,
	usb_cb_flags_t		*cb_flags,
	usb_flags_t		flags)
{
	return (usb_pipe_ctrl_xfer_wait(
	    pipe_handle, setup, data, completion_reason, cb_flags, flags));
}


int
usba10_usb_set_cfg(
	dev_info_t		*dip,
	uint_t			cfg_index,
	usb_flags_t		usb_flags,
	void			(*cb)(
					usb_pipe_handle_t ph,
					usb_opaque_t	arg,
					int		rval,
					usb_cb_flags_t	flags),
	usb_opaque_t		cb_arg)
{
	return (usb_set_cfg(dip, cfg_index, usb_flags, cb, cb_arg));
}


int
usba10_usb_get_cfg(
	dev_info_t		*dip,
	uint_t			*cfgval,
	usb_flags_t		usb_flags)
{
	return (usb_get_cfg(dip, cfgval, usb_flags));
}


int
usba10_usb_set_alt_if(
	dev_info_t		*dip,
	uint_t			interface,
	uint_t			alt_number,
	usb_flags_t		usb_flags,
	void			(*cb)(
					usb_pipe_handle_t ph,
					usb_opaque_t	arg,
					int		rval,
					usb_cb_flags_t	flags),
	usb_opaque_t		cb_arg)
{
	return (usb_set_alt_if(
	    dip, interface, alt_number, usb_flags, cb, cb_arg));
}


int
usba10_usb_get_alt_if(
	dev_info_t		*dip,
	uint_t			if_number,
	uint_t			*alt_number,
	usb_flags_t		flags)
{
	return (usb_get_alt_if(dip, if_number, alt_number, flags));
}


usb_bulk_req_t *
usba10_usb_alloc_bulk_req(
	dev_info_t		*dip,
	size_t			len,
	usb_flags_t		flags)
{
	return (usb_alloc_bulk_req(dip, len, flags));
}


void
usba10_usb_free_bulk_req(usb_bulk_req_t *reqp)
{
	usb_free_bulk_req(reqp);
}


int
usba10_usb_pipe_bulk_xfer(
	usb_pipe_handle_t	pipe_handle,
	usb_bulk_req_t		*reqp,
	usb_flags_t		flags)
{
	return (usb_pipe_bulk_xfer(pipe_handle, reqp, flags));
}


int
usba10_usb_pipe_bulk_transfer_size(
	dev_info_t		*dip,
	size_t			*size)
{
	return (usb_pipe_bulk_transfer_size(dip, size));
}


usb_intr_req_t *
usba10_usb_alloc_intr_req(
	dev_info_t		*dip,
	size_t			len,
	usb_flags_t		flags)
{
	return (usb_alloc_intr_req(dip, len, flags));
}


void
usba10_usb_free_intr_req(usb_intr_req_t *reqp)
{
	usb_free_intr_req(reqp);
}


int
usba10_usb_pipe_intr_xfer(
	usb_pipe_handle_t	pipe_handle,
	usb_intr_req_t		*req,
	usb_flags_t		flags)
{
	return (usb_pipe_intr_xfer(pipe_handle, req, flags));
}


void
usba10_usb_pipe_stop_intr_polling(
	usb_pipe_handle_t	pipe_handle,
	usb_flags_t		flags)
{
	usb_pipe_stop_intr_polling(pipe_handle, flags);
}


usb_isoc_req_t *
usba10_usb_alloc_isoc_req(
	dev_info_t		*dip,
	uint_t			isoc_pkts_count,
	size_t			len,
	usb_flags_t		flags)
{
	return (usb_alloc_isoc_req(dip, isoc_pkts_count, len, flags));
}


void
usba10_usb_free_isoc_req(usb_isoc_req_t *usb_isoc_req)
{
	usb_free_isoc_req(usb_isoc_req);
}


usb_frame_number_t
usba10_usb_get_current_frame_number(dev_info_t	*dip)
{
	return (usb_get_current_frame_number(dip));
}


uint_t
usba10_usb_get_max_isoc_pkts(dev_info_t *dip)
{
	return (usb_get_max_isoc_pkts(dip));
}


int
usba10_usb_pipe_isoc_xfer(
	usb_pipe_handle_t	pipe_handle,
	usb_isoc_req_t		*reqp,
	usb_flags_t		flags)
{
	return (usb_pipe_isoc_xfer(pipe_handle, reqp, flags));
}


void
usba10_usb_pipe_stop_isoc_polling(
	usb_pipe_handle_t	pipe_handle,
	usb_flags_t		flags)
{
	usb_pipe_stop_isoc_polling(pipe_handle, flags);
}


int
usba10_usb_req_raise_power(
	dev_info_t	*dip,
	int		comp,
	int		level,
	void		(*cb)(void *arg, int rval),
	void		*arg,
	usb_flags_t	flags)
{
	return (usb_req_raise_power(dip, comp, level, cb, arg, flags));
}


int
usba10_usb_req_lower_power(
	dev_info_t	*dip,
	int		comp,
	int		level,
	void		(*cb)(void *arg, int rval),
	void		*arg,
	usb_flags_t	flags)
{
	return (usb_req_raise_power(dip, comp, level, cb, arg, flags));
}


int
usba10_usb_is_pm_enabled(dev_info_t *dip)
{
	return (usb_is_pm_enabled(dip));
}

int
usba10_usb_handle_remote_wakeup(
	dev_info_t	*dip,
	int		cmd)
{
	return (usb_handle_remote_wakeup(dip, cmd));
}


int
usba10_usb_create_pm_components(
	dev_info_t	*dip,
	uint_t		*pwrstates)
{
	return (usb_create_pm_components(dip, pwrstates));
}


int
usba10_usb_set_device_pwrlvl0(dev_info_t *dip)
{
	return (usb_set_device_pwrlvl0(dip));
}


int
usba10_usb_set_device_pwrlvl1(dev_info_t *dip)
{
	return (usb_set_device_pwrlvl1(dip));
}


int
usba10_usb_set_device_pwrlvl2(dev_info_t *dip)
{
	return (usb_set_device_pwrlvl2(dip));
}


int
usba10_usb_set_device_pwrlvl3(dev_info_t *dip)
{
	return (usb_set_device_pwrlvl3(dip));
}


int
usba10_usb_async_req(
	dev_info_t	*dip,
	void		(*func)(void *),
	void		*arg,
	usb_flags_t	flag)
{
	return (usb_async_req(dip, func, arg, flag));
}


int
usba10_usb_register_event_cbs(
	dev_info_t	*dip,
	usb_event_t	*usb_evt_data,
	usb_flags_t	flags)
{
	return (usb_register_event_cbs(dip, usb_evt_data, flags));
}


void
usba10_usb_unregister_event_cbs(
	dev_info_t	*dip,
	usb_event_t	*usb_evt_data)
{
	usb_unregister_event_cbs(dip, usb_evt_data);
}


void
usba10_usb_fail_checkpoint(
	dev_info_t	*dip,
	usb_flags_t	flags)
{
	usb_fail_checkpoint(dip, flags);
}


int
usba10_usba_vlog(
	usb_log_handle_t	handle,
	uint_t			level,
	uint_t			mask,
	char			*fmt,
	va_list			ap)
{
	return (usba_vlog(handle, level, mask, fmt, ap));
}


usb_log_handle_t
usba10_usb_alloc_log_handle(
	dev_info_t	*dip,
	char		*name,
	uint_t		*errlevel,
	uint_t		*mask,
	uint_t		*instance_filter,
	uint_t		show_label,
	usb_flags_t	flags)
{
	return (usb_alloc_log_handle(
	    dip, name, errlevel, mask, instance_filter, show_label, flags));
}


void
usba10_usb_free_log_handle(usb_log_handle_t handle)
{
	usb_free_log_handle(handle);
}


int
usba10_usb_log_descr_tree(
	usb_client_dev_data_t	*dev_data,
	usb_log_handle_t	log_handle,
	uint_t			level,
	uint_t			mask)
{
	return (usb_log_descr_tree(dev_data, log_handle, level, mask));
}


int
usba10_usb_print_descr_tree(
	dev_info_t		*dip,
	usb_client_dev_data_t	*dev_data)
{
	return (usb_print_descr_tree(dip, dev_data));
}


int
usba10_usb_check_same_device(
	dev_info_t		*dip,
	usb_log_handle_t	log_handle,
	int			log_level,
	int			log_mask,
	uint_t			check_mask,
	char			*device_string)
{
	return (usb_check_same_device(
	    dip, log_handle, log_level, log_mask, check_mask, device_string));
}


const char *
usba10_usb_str_cr(usb_cr_t cr)
{
	return (usb_str_cr(cr));
}


char *
usba10_usb_str_cb_flags(
	usb_cb_flags_t cb_flags,
	char *buffer,
	size_t length)
{
	return (usb_str_cb_flags(cb_flags, buffer, length));
}


const char *
usba10_usb_str_pipe_state(usb_pipe_state_t state)
{
	return (usb_str_pipe_state(state));
}


const char *
usba10_usb_str_dev_state(int state)
{
	return (usb_str_dev_state(state));
}


const char *
usba10_usb_str_rval(int rval)
{
	return (usb_str_rval(rval));
}


int
usba10_usb_rval2errno(int rval)
{
	return (usb_rval2errno(rval));
}


usb_serialization_t
usba10_usb_init_serialization(
	dev_info_t	*s_dip,
	uint_t		flag)
{
	return (usb_init_serialization(s_dip, flag));
}


void
usba10_usb_fini_serialization(usb_serialization_t usb_serp)
{
	usb_fini_serialization(usb_serp);
}


int
usba10_usb_serialize_access(
	usb_serialization_t	usb_serp,
	uint_t			how_to_wait,
	uint_t			delta_timeout)
{
	return (usb_serialize_access(
	    usb_serp, how_to_wait, delta_timeout));
}


int
usba10_usb_try_serialize_access(
	usb_serialization_t usb_serp,
	uint_t flag)
{
	return (usb_try_serialize_access(usb_serp, flag));
}


void
usba10_usb_release_access(usb_serialization_t usb_serp)
{
	usb_release_access(usb_serp);
}

#endif
