#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* view.c */
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

#include "petscviewer.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscviewerdestroy_ PETSCVIEWERDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscviewerdestroy_ petscviewerdestroy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscviewersetup_ PETSCVIEWERSETUP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscviewersetup_ petscviewersetup
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscviewerdestroy_(PetscViewer *viewer, int *__ierr ){
*__ierr = PetscViewerDestroy(viewer);
}
void PETSC_STDCALL  petscviewersetup_(PetscViewer viewer, int *__ierr ){
*__ierr = PetscViewerSetUp(
	(PetscViewer)PetscToPointer((viewer) ));
}
#if defined(__cplusplus)
}
#endif
