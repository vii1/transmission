/*
 * This file Copyright (C) 2016 Mnemosyne LLC
 *
 * It may be used under the GNU GPL versions 2 or 3
 * or any future license endorsed by Mnemosyne LLC.
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define tr_ref_init(obj, vftbl) tr_ref_init_impl ((obj), &(obj)->ref, (vftbl))
#define tr_ref_inc(obj)         tr_ref_inc_impl ((obj), &(obj)->ref, __FILE__, __LINE__)
#define tr_ref_dec(obj)         tr_ref_dec_impl ((obj), &(obj)->ref, __FILE__, __LINE__)

typedef void         (*tr_ref_free_func)      (const void * obj);
typedef const char * (*tr_ref_to_string_func) (const void * obj);

typedef struct tr_ref_vftbl
{
  tr_ref_free_func      free;
  tr_ref_to_string_func to_string;
}
tr_ref_vftbl;

typedef struct tr_ref
{
  const tr_ref_vftbl * vftbl;
  int                  count;
}
tr_ref;

void tr_ref_init_impl (const void         * obj,
                       tr_ref             * ref,
                       const tr_ref_vftbl * vftbl);

void tr_ref_inc_impl  (const void         * obj,
                       const tr_ref       * ref,
                       const char         * file,
                       int                  line);

void tr_ref_dec_impl  (const void         * obj,
                       const tr_ref       * ref,
                       const char         * file,
                       int                  line);

#ifdef __cplusplus
}
#endif
