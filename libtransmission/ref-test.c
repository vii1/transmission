/*
 * This file Copyright (C) 2016 Mnemosyne LLC
 *
 * It may be used under the GNU GPL versions 2 or 3
 * or any future license endorsed by Mnemosyne LLC.
 *
 */

#include "transmission.h"
#include "ref.h"

#include "libtransmission-test.h"

static const void * freed_obj = NULL;
static int free_count = 0;

static void
obj_free_reset (void)
{
  freed_obj = NULL;
  free_count = 0;
}

static void
obj_free (const void * obj)
{
  freed_obj = obj;
  ++free_count;
}

static const tr_ref_vftbl obj_vftbl =
{
  .free = &obj_free,
  .to_string = NULL,
};

typedef struct obj
{
  tr_ref ref;
}
obj;

static int
test_init (void)
{
  obj o;

  tr_ref_init (&o, &obj_vftbl);
  check_ptr_eq (&obj_vftbl, o.ref.vftbl);
  check_int_eq (1, o.ref.count);

  return 0;
}

static int
test_inc_dec (void)
{
  obj o;

  obj_free_reset ();

  tr_ref_init (&o, &obj_vftbl);
  check_ptr_eq (NULL, freed_obj);
  check_int_eq (0, free_count);

  tr_ref_inc (&o);
  check_int_eq (2, o.ref.count);
  check_ptr_eq (NULL, freed_obj);
  check_int_eq (0, free_count);

  tr_ref_inc (&o);
  check_int_eq (3, o.ref.count);
  check_ptr_eq (NULL, freed_obj);
  check_int_eq (0, free_count);

  tr_ref_dec (&o);
  check_int_eq (2, o.ref.count);
  check_ptr_eq (NULL, freed_obj);
  check_int_eq (0, free_count);

  tr_ref_dec (&o);
  check_int_eq (1, o.ref.count);
  check_ptr_eq (NULL, freed_obj);
  check_int_eq (0, free_count);

  tr_ref_dec (&o);
  check_int_eq (0, o.ref.count);
  check_ptr_eq (&o, freed_obj);
  check_int_eq (1, free_count);

  return 0;
}

int
main (void)
{
  const testFunc tests[] = { test_init,
                             test_inc_dec };

  return runTests (tests, NUM_TESTS (tests));
}
