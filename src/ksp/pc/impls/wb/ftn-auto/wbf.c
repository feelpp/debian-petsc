#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* wb.c */
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
#include "petscksp.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcexoticsettype_ PCEXOTICSETTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcexoticsettype_ pcexoticsettype
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  pcexoticsettype_(PC pc,PCExoticType *type, int *__ierr ){
*__ierr = PCExoticSetType(
	(PC)PetscToPointer((pc) ),*type);
}
#if defined(__cplusplus)
}
#endif
