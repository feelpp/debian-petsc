#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* hdf5v.c */
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
#include "petscviewerhdf5.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscviewerhdf5popgroup_ PETSCVIEWERHDF5POPGROUP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscviewerhdf5popgroup_ petscviewerhdf5popgroup
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscviewerhdf5incrementtimestep_ PETSCVIEWERHDF5INCREMENTTIMESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscviewerhdf5incrementtimestep_ petscviewerhdf5incrementtimestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscviewerhdf5settimestep_ PETSCVIEWERHDF5SETTIMESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscviewerhdf5settimestep_ petscviewerhdf5settimestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscviewerhdf5gettimestep_ PETSCVIEWERHDF5GETTIMESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscviewerhdf5gettimestep_ petscviewerhdf5gettimestep
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscviewerhdf5popgroup_(PetscViewer viewer, int *__ierr ){
*__ierr = PetscViewerHDF5PopGroup(
	(PetscViewer)PetscToPointer((viewer) ));
}
void PETSC_STDCALL  petscviewerhdf5incrementtimestep_(PetscViewer viewer, int *__ierr ){
*__ierr = PetscViewerHDF5IncrementTimestep(
	(PetscViewer)PetscToPointer((viewer) ));
}
void PETSC_STDCALL  petscviewerhdf5settimestep_(PetscViewer viewer,PetscInt *timestep, int *__ierr ){
*__ierr = PetscViewerHDF5SetTimestep(
	(PetscViewer)PetscToPointer((viewer) ),*timestep);
}
void PETSC_STDCALL  petscviewerhdf5gettimestep_(PetscViewer viewer,PetscInt *timestep, int *__ierr ){
*__ierr = PetscViewerHDF5GetTimestep(
	(PetscViewer)PetscToPointer((viewer) ),timestep);
}
#if defined(__cplusplus)
}
#endif
