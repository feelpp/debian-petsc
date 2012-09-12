#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* iga.c */
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

#include "petscdmiga.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmigavecrestorearray_ DMIGAVECRESTOREARRAY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmigavecrestorearray_ dmigavecrestorearray
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmigavecrestorearray_(DM dm,Vec vec,void*array, int *__ierr ){
*__ierr = DMIGAVecRestoreArray(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((vec) ),array);
}
#if defined(__cplusplus)
}
#endif
