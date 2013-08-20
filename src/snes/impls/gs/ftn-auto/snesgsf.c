#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* snesgs.c */
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

#include "petscsnes.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesgssettolerances_ SNESGSSETTOLERANCES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesgssettolerances_ snesgssettolerances
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesgsgettolerances_ SNESGSGETTOLERANCES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesgsgettolerances_ snesgsgettolerances
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesgssetsweeps_ SNESGSSETSWEEPS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesgssetsweeps_ snesgssetsweeps
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesgsgetsweeps_ SNESGSGETSWEEPS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesgsgetsweeps_ snesgsgetsweeps
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  snesgssettolerances_(SNES snes,PetscReal *abstol,PetscReal *rtol,PetscReal *stol,PetscInt *maxit, int *__ierr ){
*__ierr = SNESGSSetTolerances(
	(SNES)PetscToPointer((snes) ),*abstol,*rtol,*stol,*maxit);
}
void PETSC_STDCALL  snesgsgettolerances_(SNES snes,PetscReal *atol,PetscReal *rtol,PetscReal *stol,PetscInt *maxit, int *__ierr ){
*__ierr = SNESGSGetTolerances(
	(SNES)PetscToPointer((snes) ),atol,rtol,stol,maxit);
}

void PETSC_STDCALL  snesgssetsweeps_(SNES snes,PetscInt *sweeps, int *__ierr ){
*__ierr = SNESGSSetSweeps(
	(SNES)PetscToPointer((snes) ),*sweeps);
}
void PETSC_STDCALL  snesgsgetsweeps_(SNES snes,PetscInt * sweeps, int *__ierr ){
*__ierr = SNESGSGetSweeps(
	(SNES)PetscToPointer((snes) ),sweeps);
}
#if defined(__cplusplus)
}
#endif
