#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* plexlabel.c */
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
#define dmlabelhaspoint_ DMLABELHASPOINT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmlabelhaspoint_ dmlabelhaspoint
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetnumlabels_ DMPLEXGETNUMLABELS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetnumlabels_ dmplexgetnumlabels
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmlabelhaspoint_(DMLabel *label,PetscInt *point,PetscBool *contains, int *__ierr ){
*__ierr = DMLabelHasPoint(*label,*point,contains);
}
void PETSC_STDCALL  dmplexgetnumlabels_(DM dm,PetscInt *numLabels, int *__ierr ){
*__ierr = DMPlexGetNumLabels(
	(DM)PetscToPointer((dm) ),numLabels);
}
#if defined(__cplusplus)
}
#endif
