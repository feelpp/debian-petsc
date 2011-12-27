#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* dainterp.c */
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

#include "petscdmda.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetinterpolationscale_ DMGETINTERPOLATIONSCALE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetinterpolationscale_ dmgetinterpolationscale
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmgetinterpolationscale_(DM dac,DM daf,Mat mat,Vec *scale, int *__ierr ){
*__ierr = DMGetInterpolationScale(
	(DM)PetscToPointer((dac) ),
	(DM)PetscToPointer((daf) ),
	(Mat)PetscToPointer((mat) ),scale);
}
#if defined(__cplusplus)
}
#endif
