#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* cheby.c */
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

#include "petscksp.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define kspchebychevseteigenvalues_ KSPCHEBYCHEVSETEIGENVALUES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define kspchebychevseteigenvalues_ kspchebychevseteigenvalues
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define kspchebychevsetestimateeigenvalues_ KSPCHEBYCHEVSETESTIMATEEIGENVALUES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define kspchebychevsetestimateeigenvalues_ kspchebychevsetestimateeigenvalues
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  kspchebychevseteigenvalues_(KSP ksp,PetscReal *emax,PetscReal *emin, int *__ierr ){
*__ierr = KSPChebychevSetEigenvalues(
	(KSP)PetscToPointer((ksp) ),*emax,*emin);
}
void PETSC_STDCALL  kspchebychevsetestimateeigenvalues_(KSP ksp,PetscReal *a,PetscReal *b,PetscReal *c,PetscReal *d, int *__ierr ){
*__ierr = KSPChebychevSetEstimateEigenvalues(
	(KSP)PetscToPointer((ksp) ),*a,*b,*c,*d);
}
#if defined(__cplusplus)
}
#endif
