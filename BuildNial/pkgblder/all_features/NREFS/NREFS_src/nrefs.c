/*==============================================================

  MODULE   NREFS.C

  ** UNSAFE **

  This module provides a mutable ref object by creating a single 
  element vector and providing a routine to change the value of 
  that vector. 

  COPYRIGHT NIAL Systems Limited  1983-2016

================================================================*/


/* Q'Nial file that selects features and loads the xxxspecs.h file */

#include "switches.h"

/* standard library header files */

/* IOLIB */
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#ifdef UNIXSYS
#include <sys/mman.h>
#endif
#include <sys/fcntl.h>

/* STDLIB */
#include <stdlib.h>
#ifdef UNIXSYS
#include <limits.h>
#endif

/* STLIB */
#include <string.h>

/* SJLIB */
#include <setjmp.h>

/* CLIB */
#include <ctype.h>

/* VARARGSLIB */
#include <stdarg.h>
#include <stddef.h>


/* Q'Nial header files */

#include "qniallim.h"
#include "absmach.h"
#include "basics.h"
#include "lib_main.h"
#include "if.h"
#include "ops.h"

#include "utils.h"           /* for ngetname */

#ifdef NREFS


/* ========================== Reference cells ====================== */


/**
 * Create a new reference cell identified by a phrase
 * and set its value
 */
void inewref(void) {
  nialptr x = apop();
  nialptr pname, pval, res;
  nialint reslen = 2;

  if (kind(x) != atype || tally(x) != 2) {
    apush(makefault("args"));
    freeup(x);
    return;
  }

  splitfb(x, &pname, &pval);

  if (kind(pname) != phrasetype) {
    apush(makefault("?name_invalid"));
    freeup(x);
    return;
  }

  res = new_create_array(atype, 1, 0, &reslen);

  store_array(res, 0, pname);
  store_array(res, 1, pval);
    
  apush(res);
  freeup(x);
  return;
}


/**
 * _refset ref-cell new_value
 * ------------------------------
 *
 * Change the value of a reference
 *
 * NOTE: This is a mutable operation
 */
void irefset(void) {
  nialptr x = apop();
  nialptr pref, pval;

  if (kind(x) != atype || tally(x) != 2) {
    apush(makefault("?args"));
    freeup(x);
    return;
  }

  pref = fetch_array(x, 0);
  pval = fetch_array(x, 1);

  if (kind(pref) == atype && valence(pref) == 1 && tally(pref) == 2) {
    replace_array(pref, 1, pval);
    apush(pval);
  } else {
    apush(makefault("?not a refcell"));
  }
  
  freeup(x);
  return;
}


void irefget(void) {
  nialptr x = apop();

  if (kind(x) != atype || tally(x) != 2 || valence(x) != 1) {
    apush(makefault("?not_a_refcell"));
    freeup(x);
    return;
  }

  apush(fetch_array(x, 1));
  freeup(x);
  return;
}



#endif  /* NREFS */

