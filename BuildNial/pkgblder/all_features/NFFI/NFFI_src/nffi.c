/**
 * nffi.c
 * --------
 *
 * Foreign function interface for Nial using libffi
 * Contributed by John Gibbons.
 *
 */


#include "switches.h"


#ifdef NFFI

/* IOLIB */
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/fcntl.h>

/* LIMITLIB */
#include <limits.h>

/* STDLIB */
#include <stdlib.h>

/* SJLIB */
#include <setjmp.h>

/* MATHLIB */
#include <math.h>

/* Q'Nial header files */

#include "qniallim.h"
#include "lib_main.h"
#include "absmach.h"
#include "ops.h"

#include <dlfcn.h>
#include <string.h>

#include <ffi.h>


/* --------------- C Reference Support --------------- */


/**
 * Generic C reference model to map local C pointers to
 * integers for Nial
 */

typedef struct {
  int ref_type;              /* The internal type of the object, defined below */ 
  void *ref_ptr;             /* Pointer to C data */
  nialptr ref_data;          /* Associated Nial object if necessary */
  int ref_code;              /* FFI code when appropriate */
  int ref_flags;             /* Interface flags, defined below */
} CRefType, *CRefPtr; 

#define MAX_CREF   1024
static int num_cref = 0;
static CRefPtr crefs[MAX_CREF];

/**
 * Define the reference types we will handle
 */

#define FREE_CREF_SLOT         0             /* Unused */
#define NFFI_CIF               1            
#define NFFI_TYPE              2
#define NFFI_LIB               3
#define NFFI_FUNC              4
#define NFFI_CPTR              5

/*
 * Define flags for references 
 */

#define NFFI_LOCAL_MALLOC     0x01        /* Did we create this */




/* Macro to validate a reference index */
#define VALID_CREF_INDEX(i) (0 <= (i) && (i) < MAX_CREF && crefs[(i)] != NULL)
#define VALID_CREF(i, rtype) (0 <= (i) && (i) < MAX_CREF && crefs[(i)] != NULL && crefs[(i)]->ref_type == rtype)


/**
 * Allocate a reference entry and return the index.
 * Return -1 if no free references.
 */
static int alloc_cref(int rtype) {
  static int init = 0;
  int i;

  if (init == 0) {
    int j;
    for (j = 0; j < MAX_CREF; j++) {
      CRefPtr p = (CRefPtr)malloc(sizeof(CRefType));
      crefs[j] = p;
      p->ref_type = FREE_CREF_SLOT;
      p->ref_ptr  = NULL;
      p->ref_data = invalidptr;
      p->ref_code = -1;
      p->ref_flags = 0;
    }
    
    init = 1;
  }
  
  for (i = 0; i < num_cref; i++) {
    if (crefs[i]->ref_type == FREE_CREF_SLOT) {
      crefs[i]->ref_type = rtype;
      crefs[i]->ref_ptr = NULL;
      crefs[i]->ref_data = invalidptr;
      crefs[i]->ref_code = -1;
      crefs[i]->ref_flags = 0;
      return i;
    }
  }

  if (i < MAX_CREF) {
    num_cref++;
    crefs[i]->ref_type = rtype;
    crefs[i]->ref_ptr = NULL;
    crefs[i]->ref_data = invalidptr;
    crefs[i]->ref_code = -1;
    crefs[i]->ref_flags = 0;
    return i;
  }

  return -1;
}
  

/**
 * Free an existing reference object
 */
static void free_cref(int i) {
  if (crefs[i]->ref_ptr != NULL) {
    switch (crefs[i]->ref_type) {
    case FREE_CREF_SLOT:
      break;

    case NFFI_CIF:
      free(crefs[i]->ref_ptr);
      break;

    case NFFI_LIB:
      dlclose(crefs[i]->ref_ptr);
      break;

    case NFFI_TYPE:
      if (crefs[i]->ref_flags & NFFI_LOCAL_MALLOC) {
	free(crefs[i]->ref_ptr);
      }
      break;
      
    case NFFI_CPTR:
      if (crefs[i]->ref_flags & NFFI_LOCAL_MALLOC) {
	free(crefs[i]->ref_ptr);
      }
      break;
      
    default:
      /* Nothing at moment */
      break;
    }
  }
  
  crefs[i]->ref_type = FREE_CREF_SLOT;
  crefs[i]->ref_ptr  = NULL;

  if (crefs[i]->ref_data != invalidptr) {
    decrrefcnt(crefs[i]->ref_data);
    freeup(crefs[i]->ref_data);
    crefs[i]->ref_data = invalidptr;
  }

  crefs[i]->ref_code = -1;
  
  return;
}



/* ----------------------- Type Table --------------------- */

ffi_type *nffi_type_table[] = { &ffi_type_void,               /*  0 */
				&ffi_type_uint8,              /*  1 */
				&ffi_type_sint8,              /*  2 */
				&ffi_type_uint16,             /*  3 */
				&ffi_type_sint16,             /*  4 */
				&ffi_type_uint32,             /*  5 */
				&ffi_type_sint32,             /*  6 */
				&ffi_type_uint64,             /*  7 */
				&ffi_type_sint64,             /*  8 */
				&ffi_type_float,              /*  9 */
				&ffi_type_double,             /* 10 */
				&ffi_type_uchar,              /* 11 */
				&ffi_type_schar,              /* 12 */
				&ffi_type_ushort,             /* 13 */
				&ffi_type_sshort,             /* 14 */
				&ffi_type_uint,               /* 15 */
				&ffi_type_sint,               /* 16 */
				&ffi_type_ulong,              /* 17 */
				&ffi_type_slong,              /* 18 */
				&ffi_type_longdouble,         /* 19 */
				&ffi_type_pointer,            /* 20 */
				&ffi_type_complex_float,      /* 21 */
				&ffi_type_complex_double,     /* 22 */
				&ffi_type_complex_longdouble, /* 23 */
				NULL};

const int num_basic_types = 24;
#define max_cif_args 128

/*
 * Use a union to convert values
 */
typedef union NFFI_VAL {
  uint8_t            uint8_val;
  int8_t             sint8_val;
  uint16_t           uint16_val;
  int16_t            sint16_val;
  uint32_t           uint32_val;
  int32_t            sint32_val;
  uint64_t           uint64_val;
  int64_t            sint64_val;
  float              float_val;
  double             double_val;
  unsigned char      uchar_val;
  char               schar_val;
  unsigned short     ushort_val;
  short              sshort_val;
  unsigned int       uint_val;
  int                sint_val;
  unsigned long      ulong_val;
  long               slong_val;
  long double        long_double_val;
  void*              pointer_val;
} nffi_val;



  

/* -------------------------- Nial Interface ----------------- */


/**
 * Return the type pointer of either a basic type or
 * a created type
 */ 
static inline ffi_type *get_ffi_type(unsigned int idx) {

  if (idx < num_basic_types) {
    return nffi_type_table[idx];
  } else {
    int iref = idx - num_basic_types;

    if (VALID_CREF(iref, NFFI_TYPE)) {
      return (ffi_type*)(crefs[iref]->ref_ptr);
    } else {
      return NULL;
    }
  }
}



/**
 * get the application binary interface 
 */
static inline ffi_abi get_ffi_abi(unsigned int idx) {
  return FFI_DEFAULT_ABI;
}



/**
 * free a slot of any type
 */
void inffi_free(void) {
  nialptr x = apop();

  if (kind(x) == inttype) {
    if (VALID_CREF_INDEX(intval(x))) {
      free_cref(intval(x));
      apush(True_val);
    } else {
      apush(makefault("?invalid ref"));
    }
  } else {
    apush(makefault("?args"));
  }

  freeup(x);
  return;
}

   
  
/** 
 * Create a new cif from Nial data. This creates a calling interface 
 * based on argument and return types which can be used over and over 
 *
 *  ref = new_cif abi rtype atypes
 */

void inffi_new_cif(void) {
  nialptr x = apop();
  nialptr n_abi, n_rtype, n_atypes;
  ffi_type *rtype, *atypes[max_cif_args];
  int i, numargs = 0;
  ffi_cif *cifp = (ffi_cif*)malloc(sizeof(ffi_cif));
  

  if (kind(x) != atype || tally(x) != 3) {
    apush(makefault("?invalid args"));
    goto error_exit;
  }

  n_abi = fetch_array(x, 0);
  n_rtype = fetch_array(x, 1);
  n_atypes = fetch_array(x, 2);

  if (kind(n_abi) != inttype || kind(n_rtype) != inttype || kind(n_atypes) != inttype) {
    apush(makefault("?arg_types not int"));
    goto error_exit;
  }

  numargs = tally(n_atypes);
  if (numargs >= max_cif_args) {
    apush(makefault("?arg limit exceeded"));
    goto error_exit;
  }

  rtype = get_ffi_type((unsigned int)intval(n_rtype));
  if (rtype == NULL) {
    apush(makefault("?invalid rtype"));
    goto error_exit;
  }

  for (i = 0; i < numargs; i++) {
    ffi_type *ft = get_ffi_type(fetch_int(n_atypes, i));

    if (ft == NULL) {
      apush(makefault("?invalid atype"));
      goto error_exit;
    } else {
      atypes[i] = ft;
    }
  }

  if (ffi_prep_cif(cifp, get_ffi_abi(intval(n_abi)), numargs, rtype, atypes) == FFI_OK) {
    int ri = alloc_cref(NFFI_CIF);

    if (ri != -1) {
      crefs[ri]->ref_ptr = cifp;
      crefs[ri]->ref_data = n_atypes;
      crefs[ri]->ref_code = intval(n_rtype);
      crefs[ri]->ref_flags = NFFI_LOCAL_MALLOC;
      incrrefcnt(n_atypes);
      apush(createint(ri));
    } else {
      free(cifp);
      apush(makefault("no free crefs"));
    }
    
  } else {
    apush(makefault("?ffi_pref_cif failed"));
    goto error_exit;
  }
      

  freeup(x);
  return;

 error_exit:
  freeup(x);
  return;
}


/**
 * Convert nial type to a suitable C type for the call
 */
static nffi_val *nial_val_to_ffi(nffi_val *res, nialptr n_val, nialptr n_type) {
  switch(intval(n_type)) {

  case 0: /* void */
    return NULL;
    
  case 1: /* uint8_t */
    if (kind(n_val) == inttype) {
      res->uint8_val = (uint8_t)intval(n_val);
      return res;
    } else {
	return NULL;
    }
    
  case 2: /* int8_t */
    if (kind(n_val) == inttype) {
      res->sint8_val = (int8_t)intval(n_val);
      return res;
    } else {
	return NULL;
    }

  case 3: /* uint16 */
    if (kind(n_val) == inttype) {
      res->uint16_val = (uint16_t)intval(n_val);
      return res;
    } else {
	return NULL;
    }

  case 4: /* int16 */
    if (kind(n_val) == inttype) {
      res->sint16_val = (int16_t)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 5: /* uint32_t */
    if (kind(n_val) == inttype) {
      res->uint32_val = (uint32_t)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 6: /* int32_t */
    if (kind(n_val) == inttype) {
      res->sint32_val = (int32_t)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 7: /* uint64_t */
    if (kind(n_val) == inttype) {
      res->uint64_val = (uint64_t)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 8: /* int64_t */
    if (kind(n_val) == inttype) {
      res->sint64_val = (int64_t)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 9: /* float */
    if (kind(n_val) == realtype) {
      res->float_val = (float)realval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 10: /* double */
    if (kind(n_val) == realtype) {
      res->double_val = realval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 11: /* uchar */
    if (kind(n_val) == chartype) {
      res->uchar_val = (unsigned char)charval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 12: /* char */
    if (kind(n_val) == chartype) {
      res->schar_val = charval(n_val);
      return res;
    } else { 
	return NULL;
    }
    
  case 13: /* unsigned short */
    if (kind(n_val) == inttype) {
      res->ushort_val = (unsigned short)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 14: /* short */
    if (kind(n_val) == inttype) {
      res->sshort_val = (short)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

    
  case 15: /* unsigned int */
    if (kind(n_val) == inttype) {
      res->uint_val = (unsigned int)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 16: /* int */
    if (kind(n_val) == inttype) {
      res->sint_val = (int)intval(n_val);
      return res;
    } else { 
	return NULL;
    }
    
  case 17: /* unsigned long */
    if (kind(n_val) == inttype) {
      res->ulong_val = (unsigned long)intval(n_val);
      return res;
    } else { 
	return NULL;
    }
    
  case 18: /* long */
    if (kind(n_val) == inttype) {
      res->slong_val = (long)intval(n_val);
      return res;
    } else { 
	return NULL;
    }
    
  case 19: /* long double */
    if (kind(n_val) == realtype) {
      res->long_double_val = (long double)intval(n_val);
      return res;
    } else { 
	return NULL;
    }

  case 20: /* pointer */
    if (kind(n_val) == chartype) {
      res->pointer_val = pfirstchar(n_val);
    } else if (kind(n_val) == realtype) {
      res->pointer_val = pfirstreal(n_val); 
    } else if (kind(n_val) == inttype) {
      if (valence(n_val) == 0 && intval(n_val) >= num_basic_types) {
	nialint idx = intval(n_val) - num_basic_types;
	res->pointer_val = crefs[idx]->ref_ptr;
	return res;
      } else {
	res->pointer_val = pfirstint(n_val);
      }
    } else { 
	return NULL;
    }
  }
}


/**
 * Convert a value from ffi to nial format
 */
static nialptr ffi_val_to_nial(nffi_val *rval, int rtype) {
  return createint(rtype);
}




/**
 * Fetch a value from an array of arbitrary type
 */
static nialptr fetch_value(nialptr x, nialint i) {
  switch(kind(x)) {
  case atype:
    return fetch_array(x, i);

  case inttype:
    return createint(fetch_int(x,i));

  case realtype:
    return createreal(fetch_real(x, i));

  case chartype:
    return createchar(fetch_char(x, i));

  case booltype:
    return createint(fetch_bool(x, i));

  default:
    return invalidptr;
  }

}



/** 
 * Call a function 
 *
 *  rtype = nffi_call cif fn avalues
 *
 */

void inffi_call(void) {
  nialptr x = apop();
  nialptr n_cif, n_fn, n_avalues, n_atypes;
  nffi_val rval, avals[max_cif_args];
  int id_cif, id_fn, rtype;
  void *avalues[max_cif_args];
  int i, numargs = 0;

  if (kind(x) != atype || tally(x) != 3) {
    apush(makefault("?invalid args"));
    goto error_exit;
  }

  n_cif = fetch_array(x, 0);
  n_fn = fetch_array(x, 1);
  n_avalues = fetch_array(x, 2);

  if (kind(n_cif) != inttype || !VALID_CREF(intval(n_cif), NFFI_CIF)
      || kind(n_fn) != inttype || !VALID_CREF(intval(n_fn), NFFI_FUNC)) {
    apush(makefault("?arg_types invalid"));
    goto error_exit;
  }

  id_cif = intval(n_cif);
  id_fn = intval(n_fn);
  
  /* Get the arg count from the cif */
  numargs = tally(crefs[id_cif]->ref_data);
  if (numargs != tally(n_avalues)) {
    apush(makefault("?arg count invalid"));
    goto error_exit;
  }

  n_atypes = crefs[id_cif]->ref_data;
  rtype = crefs[id_cif]->ref_code;

  /* Build the argument values vector */
  for (i = 0; i < numargs; i++) {
    avalues[i] = nial_val_to_ffi(&avals[i], fetch_value(n_avalues, i), fetch_value(n_atypes, i));
  }


  /* Make the call */
  ffi_call(crefs[id_cif]->ref_ptr, crefs[id_fn]->ref_ptr, &rval, avalues);

  /* Get the return value */
  apush(ffi_val_to_nial(&rval, rtype));
  freeup(x);
  return;

 error_exit:
  freeup(x);
  return;
}


/**
 * Load a dynamic library into the Nial address space.
 */
void inffi_load_lib(void) {
  nialptr x = apop();
  
  /* verify atom or char array supplied as name */
  if (istext(x)) {
    char *libname = pfirstchar(x);
    void *dlp = dlopen(libname, RTLD_NOW|RTLD_GLOBAL);
    
    /* Check the open succeeded */
    if(dlp == NULL) {
    	apush(makefault("?dlopen failed"));
    } else {
      int id = alloc_cref(NFFI_LIB);

      if (id != -1) {
	crefs[id]->ref_ptr = dlp;
	apush(createint(id));
      } else {
	apush(makefault("?no resources"));
      }
    }
  } else {
    apush(makefault("?nffi_load_lib requires library name"));
  }
  
  freeup(x);
  return;
}


/**
 * Find the function primitive corresponding to a supplied name
 */
void inffi_get_sym(void) {
  nialptr x = apop();
  
  if (kind(x) == atype && tally(x) == 2) {
    nialptr nlib, nsymn;
    
    /* Get library and symbol and validate */
    splitfb(x, &nlib, &nsymn);
    
    if (istext(nsymn) && kind(nlib) == inttype) {
      nialint lid = intval(nlib);

      if (VALID_CREF(lid, NFFI_LIB)) {
	void* psym = dlsym( crefs[lid]->ref_ptr, pfirstchar(nsymn));
      
	if(psym == NULL) {
	  apush(makefault("?nffi_get_sym failed"));
	} else {
	  int fi = alloc_cref(NFFI_FUNC);
	  
	  if (fi != -1) {
	    crefs[fi]->ref_ptr = psym;
	    apush(createint(fi));
	  } else {
	    apush(makefault("?no resources"));
	  }
	}
      } else {
	apush(makefault("?not a lib"));
      }
    
    } else {
      apush(makefault("?nffi_get_sym bad arguments"));
    }
  } else {
    apush(makefault("?nffi_get_sym expects two arguments"));
  }
  
  freeup(x);
  return;
}



/* ----------------------- Loading Libraries ----------------------- */

#ifdef WORKED



/**
 * Close a shared library
 */
void inffi_close(void) {
  nialptr nlib = apop();
  PointerCast libcast;

  if (NDL_RESTORE_STRUCT(PointerCast, &libcast, nlib) != -1 && libcast.castType == NDL_LIBRARY_CAST) {
    dlclose(libcast.ptrVal);                          /* Close the handle       */ 
    memset(pfirstint(nlib), 0, sizeof(PointerCast));   /* Clear cast and pointer */
    apush(True_val);
  } else {
    apush(makefault("?dynlib"));
  }
  
  freeup(nlib);
}


/** 
 * Call a function from a shared library
 */
void indlCall(void) {
  nialptr x = apop();
  nialptr fun;
  nialptr funargs;
  PointerCast funcast;

  if (tally(x) == 2) {
    splitfb(x, &fun, &funargs);

    if (NDL_RESTORE_STRUCT(PointerCast, &funcast, fun) != -1 && funcast.castType == NDL_FUNCTION_CAST) {
      /* invoke the externalfunction */
      NialPrimFunction nfptr = (NialPrimFunction)(funcast.ptrVal);
      apush(funargs);
      (nfptr)();
    } else {
      apush(makefault("?dynfun"));
    }

  } else {
    apush(makefault("?args"));
  }

  freeup(x);
  return;
}

#endif /* WORKED */

#endif /* NFFI */
