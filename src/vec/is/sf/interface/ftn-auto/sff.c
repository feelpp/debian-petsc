#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* sf.c */
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

#include "petscsf.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsfsetup_ PETSCSFSETUP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsfsetup_ petscsfsetup
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsfduplicate_ PETSCSFDUPLICATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsfduplicate_ petscsfduplicate
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscsfsetup_(PetscSF *sf, int *__ierr ){
*__ierr = PetscSFSetUp(*sf);
}
void PETSC_STDCALL  petscsfduplicate_(PetscSF *sf,PetscSFDuplicateOption *opt,PetscSF *newsf, int *__ierr ){
*__ierr = PetscSFDuplicate(*sf,*opt,
	(PetscSF* )PetscToPointer((newsf) ));
}
#if defined(__cplusplus)
}
#endif
