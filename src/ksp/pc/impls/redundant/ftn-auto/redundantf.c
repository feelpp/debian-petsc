#include "petscsys.h"
#include "petscfix.h"
/* redundant.c */
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

#include "petscpc.h"
#include "petscmat.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcredundantsetnumber_ PCREDUNDANTSETNUMBER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcredundantsetnumber_ pcredundantsetnumber
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcredundantsetscatter_ PCREDUNDANTSETSCATTER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcredundantsetscatter_ pcredundantsetscatter
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcredundantgetpc_ PCREDUNDANTGETPC
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcredundantgetpc_ pcredundantgetpc
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcredundantgetoperators_ PCREDUNDANTGETOPERATORS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcredundantgetoperators_ pcredundantgetoperators
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL   pcredundantsetnumber_(PC pc,PetscInt *nredundant, int *__ierr ){
*__ierr = PCRedundantSetNumber(
	(PC)PetscToPointer((pc) ),*nredundant);
}
void PETSC_STDCALL   pcredundantsetscatter_(PC pc,VecScatter in,VecScatter out, int *__ierr ){
*__ierr = PCRedundantSetScatter(
	(PC)PetscToPointer((pc) ),
	(VecScatter)PetscToPointer((in) ),
	(VecScatter)PetscToPointer((out) ));
}
void PETSC_STDCALL   pcredundantgetpc_(PC pc,PC *innerpc, int *__ierr ){
*__ierr = PCRedundantGetPC(
	(PC)PetscToPointer((pc) ),innerpc);
}
void PETSC_STDCALL   pcredundantgetoperators_(PC pc,Mat *mat,Mat *pmat, int *__ierr ){
*__ierr = PCRedundantGetOperators(
	(PC)PetscToPointer((pc) ),mat,pmat);
}
#if defined(__cplusplus)
}
#endif
