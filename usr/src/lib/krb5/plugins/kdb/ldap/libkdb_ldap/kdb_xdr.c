/*
 * lib/kdb/kdb_ldap/kdb_xdr.c
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 */

#include <k5-int.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "kdb_xdr.h"

#define safe_realloc(p,n) ((p)?(realloc(p,n)):(malloc(n)))

krb5_error_code
krb5_dbe_update_tl_data(context, entry, new_tl_data)
    krb5_context          context;
    krb5_db_entry       * entry;
    krb5_tl_data	* new_tl_data;
{
    krb5_tl_data        * tl_data;
    krb5_octet          * tmp;

    /* copy the new data first, so we can fail cleanly if malloc()
       fails */

    if ((tmp = (krb5_octet *) malloc(new_tl_data->tl_data_length)) == NULL)
	return(ENOMEM);

    /* Find an existing entry of the specified type and point at
       it, or NULL if not found */

    for (tl_data = entry->tl_data; tl_data; tl_data = tl_data->tl_data_next)
	if (tl_data->tl_data_type == new_tl_data->tl_data_type)
	    break;

    /* if necessary, chain a new record in the beginning and point at it */

    if (!tl_data) {
	if ((tl_data = (krb5_tl_data *) calloc(1, sizeof(krb5_tl_data)))
	    == NULL) {
	    free(tmp);
	    return(ENOMEM);
	}
	tl_data->tl_data_next = entry->tl_data;
	entry->tl_data = tl_data;
	entry->n_tl_data++;
    }

    /* fill in the record */

    free(tl_data->tl_data_contents);

    tl_data->tl_data_type = new_tl_data->tl_data_type;
    tl_data->tl_data_length = new_tl_data->tl_data_length;
    tl_data->tl_data_contents = tmp;
    memcpy(tmp, new_tl_data->tl_data_contents, tl_data->tl_data_length);

    return(0);
}

krb5_error_code
krb5_dbe_lookup_tl_data(context, entry, ret_tl_data)
    krb5_context          context;
    krb5_db_entry       * entry;
    krb5_tl_data        * ret_tl_data;
{
    krb5_tl_data *tl_data;

    for (tl_data = entry->tl_data; tl_data; tl_data = tl_data->tl_data_next) {
	if (tl_data->tl_data_type == ret_tl_data->tl_data_type) {
	    *ret_tl_data = *tl_data;
	    return(0);
	}
    }

    /* if the requested record isn't found, return zero bytes.
       if it ever means something to have a zero-length tl_data,
       this code and its callers will have to be changed */

    ret_tl_data->tl_data_length = 0;
    ret_tl_data->tl_data_contents = NULL;
    return(0);
}

krb5_error_code
krb5_dbe_update_last_pwd_change(context, entry, stamp)
    krb5_context          context;
    krb5_db_entry       * entry;
    krb5_timestamp	  stamp;
{
    krb5_tl_data        tl_data;
    krb5_octet          buf[4]; /* this is the encoded size of an int32 */

    tl_data.tl_data_type = KRB5_TL_LAST_PWD_CHANGE;
    tl_data.tl_data_length = sizeof(buf);
    krb5_kdb_encode_int32((krb5_int32) stamp, buf);
    tl_data.tl_data_contents = buf;

    return(krb5_dbe_update_tl_data(context, entry, &tl_data));
}

krb5_error_code
krb5_dbe_lookup_last_pwd_change(context, entry, stamp)
    krb5_context          context;
    krb5_db_entry       * entry;
    krb5_timestamp	* stamp;
{
    krb5_tl_data        tl_data;
    krb5_error_code	code;
    krb5_int32		tmp;

    tl_data.tl_data_type = KRB5_TL_LAST_PWD_CHANGE;

    if ((code = krb5_dbe_lookup_tl_data(context, entry, &tl_data)))
	return(code);

    if (tl_data.tl_data_length != 4) {
	*stamp = 0;
	return(0);
    }

    krb5_kdb_decode_int32(tl_data.tl_data_contents, tmp);

    *stamp = (krb5_timestamp) tmp;

    return(0);
}

/* it seems odd that there's no function to remove a tl_data, but if
   I need one, I'll add one */

krb5_error_code
krb5_dbe_update_mod_princ_data(context, entry, mod_date, mod_princ)
    krb5_context	  context;
    krb5_db_entry	* entry;
    krb5_timestamp	  mod_date;
    krb5_const_principal  mod_princ;
{
    krb5_tl_data          tl_data;

    krb5_error_code 	  retval = 0;
    krb5_octet		* nextloc = 0;
    char		* unparse_mod_princ = 0;
    unsigned int	unparse_mod_princ_size;

    if ((retval = krb5_unparse_name(context, mod_princ,
				    &unparse_mod_princ)))
	return(retval);

    unparse_mod_princ_size = strlen(unparse_mod_princ) + 1;

    if ((nextloc = (krb5_octet *) malloc(unparse_mod_princ_size + 4))
	== NULL) {
	free(unparse_mod_princ);
	return(ENOMEM);
    }

    tl_data.tl_data_type = KRB5_TL_MOD_PRINC;
    tl_data.tl_data_length = unparse_mod_princ_size + 4;
    tl_data.tl_data_contents = nextloc;

    /* Mod Date */
    krb5_kdb_encode_int32(mod_date, nextloc);

    /* Mod Princ */
    memcpy(nextloc+4, unparse_mod_princ, unparse_mod_princ_size);

    retval = krb5_dbe_update_tl_data(context, entry, &tl_data);

    free(unparse_mod_princ);
    free(nextloc);

    return(retval);
}

krb5_error_code
krb5_dbe_lookup_mod_princ_data(context, entry, mod_time, mod_princ)
    krb5_context	  context;
    krb5_db_entry	* entry;
    krb5_timestamp	* mod_time;
    krb5_principal	* mod_princ;
{
    krb5_tl_data        tl_data;
    krb5_error_code	code;

    tl_data.tl_data_type = KRB5_TL_MOD_PRINC;

    if ((code = krb5_dbe_lookup_tl_data(context, entry, &tl_data)))
	return(code);

    if ((tl_data.tl_data_length < 5) ||
	(tl_data.tl_data_contents[tl_data.tl_data_length-1] != '\0'))
	return(KRB5_KDB_TRUNCATED_RECORD);

    /* Mod Date */
    krb5_kdb_decode_int32(tl_data.tl_data_contents, *mod_time);

    /* Mod Princ */
    if ((code = krb5_parse_name(context,
				(const char *) (tl_data.tl_data_contents+4),
				mod_princ)))
	return(code);

    return(0);
}
