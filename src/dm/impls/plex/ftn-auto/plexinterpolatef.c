#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* plexinterpolate.c */
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

#include "petscdmplex.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexinterpolate_ DMPLEXINTERPOLATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexinterpolate_ dmplexinterpolate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexcopycoordinates_ DMPLEXCOPYCOORDINATES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexcopycoordinates_ dmplexcopycoordinates
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmplexinterpolate_(DM dm,DM *dmInt, int *__ierr ){
*__ierr = DMPlexInterpolate(
	(DM)PetscToPointer((dm) ),dmInt);
}
void PETSC_STDCALL  dmplexcopycoordinates_(DM dmA,DM dmB, int *__ierr ){
*__ierr = DMPlexCopyCoordinates(
	(DM)PetscToPointer((dmA) ),
	(DM)PetscToPointer((dmB) ));
}
#if defined(__cplusplus)
}
#endif
