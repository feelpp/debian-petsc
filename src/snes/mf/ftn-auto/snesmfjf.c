#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* snesmfj.c */
/* Fortran interface file */

/*
* This file was generated automatically by bfort from the C source
* file.  
 */

#ifdef PETSC_USE_POINTER_CONVERSION
#if defined(__cplusplus)
extern "C" { 
#endif 
extern void *PetscToPointer(void*);
extern int PetscFromPointer(void *);
extern void PetscRmPointer(void*);
#if defined(__cplusplus)
} 
#endif 

#else

#define PetscToPointer(a) (*(long *)(a))
#define PetscFromPointer(a) (long)(a)
#define PetscRmPointer(a)
#endif

#include "petscsnes.h"
#include "petscdm.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define matcreatesnesmf_ MATCREATESNESMF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define matcreatesnesmf_ matcreatesnesmf
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  matcreatesnesmf_(SNES snes,Mat *J, int *__ierr ){
*__ierr = MatCreateSNESMF(
	(SNES)PetscToPointer((snes) ),J);
}
#if defined(__cplusplus)
}
#endif
