#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* reg.c */
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

#include "petscsys.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscfunctionlistdestroy_ PETSCFUNCTIONLISTDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscfunctionlistdestroy_ petscfunctionlistdestroy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscfunctionlistview_ PETSCFUNCTIONLISTVIEW
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscfunctionlistview_ petscfunctionlistview
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscfunctionlistduplicate_ PETSCFUNCTIONLISTDUPLICATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscfunctionlistduplicate_ petscfunctionlistduplicate
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscfunctionlistdestroy_(PetscFunctionList *fl, int *__ierr ){
*__ierr = PetscFunctionListDestroy(
	(PetscFunctionList* )PetscToPointer((fl) ));
}
void PETSC_STDCALL  petscfunctionlistview_(PetscFunctionList *list,PetscViewer viewer, int *__ierr ){
*__ierr = PetscFunctionListView(*list,
	(PetscViewer)PetscToPointer((viewer) ));
}
void PETSC_STDCALL  petscfunctionlistduplicate_(PetscFunctionList *fl,PetscFunctionList *nl, int *__ierr ){
*__ierr = PetscFunctionListDuplicate(*fl,
	(PetscFunctionList* )PetscToPointer((nl) ));
}
#if defined(__cplusplus)
}
#endif
