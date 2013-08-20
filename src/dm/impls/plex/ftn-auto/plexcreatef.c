#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* plexcreate.c */
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
#define dmplexcreateboxmesh_ DMPLEXCREATEBOXMESH
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexcreateboxmesh_ dmplexcreateboxmesh
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexcreate_ DMPLEXCREATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexcreate_ dmplexcreate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexclone_ DMPLEXCLONE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexclone_ dmplexclone
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmplexcreateboxmesh_(MPI_Fint * comm,PetscInt *dim,PetscBool *interpolate,DM *dm, int *__ierr ){
*__ierr = DMPlexCreateBoxMesh(
	MPI_Comm_f2c( *(comm) ),*dim,*interpolate,dm);
}
void PETSC_STDCALL  dmplexcreate_(MPI_Fint * comm,DM *mesh, int *__ierr ){
*__ierr = DMPlexCreate(
	MPI_Comm_f2c( *(comm) ),mesh);
}
void PETSC_STDCALL  dmplexclone_(DM dm,DM *newdm, int *__ierr ){
*__ierr = DMPlexClone(
	(DM)PetscToPointer((dm) ),newdm);
}
#if defined(__cplusplus)
}
#endif
