#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* nasm.c */
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
#define snesnasmsetsubdomains_ SNESNASMSETSUBDOMAINS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesnasmsetsubdomains_ snesnasmsetsubdomains
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesnasmgetsubdomains_ SNESNASMGETSUBDOMAINS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesnasmgetsubdomains_ snesnasmgetsubdomains
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesnasmgetsubdomainvecs_ SNESNASMGETSUBDOMAINVECS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesnasmgetsubdomainvecs_ snesnasmgetsubdomainvecs
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesnasmsetcomputefinaljacobian_ SNESNASMSETCOMPUTEFINALJACOBIAN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesnasmsetcomputefinaljacobian_ snesnasmsetcomputefinaljacobian
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesnasmsetdamping_ SNESNASMSETDAMPING
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesnasmsetdamping_ snesnasmsetdamping
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snesnasmgetdamping_ SNESNASMGETDAMPING
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snesnasmgetdamping_ snesnasmgetdamping
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  snesnasmsetsubdomains_(SNES snes,PetscInt *n,SNES subsnes[],VecScatter iscatter[],VecScatter oscatter[],VecScatter gscatter[], int *__ierr ){
*__ierr = SNESNASMSetSubdomains(
	(SNES)PetscToPointer((snes) ),*n,subsnes,iscatter,oscatter,gscatter);
}
void PETSC_STDCALL  snesnasmgetsubdomains_(SNES snes,PetscInt *n,SNES *subsnes[],VecScatter *iscatter[],VecScatter *oscatter[],VecScatter *gscatter[], int *__ierr ){
*__ierr = SNESNASMGetSubdomains(
	(SNES)PetscToPointer((snes) ),n,subsnes,iscatter,oscatter,gscatter);
}
void PETSC_STDCALL  snesnasmgetsubdomainvecs_(SNES snes,PetscInt *n,Vec **x,Vec **y,Vec **b,Vec **xl, int *__ierr ){
*__ierr = SNESNASMGetSubdomainVecs(
	(SNES)PetscToPointer((snes) ),n,x,y,b,xl);
}
void PETSC_STDCALL  snesnasmsetcomputefinaljacobian_(SNES snes,PetscBool *flg, int *__ierr ){
*__ierr = SNESNASMSetComputeFinalJacobian(
	(SNES)PetscToPointer((snes) ),*flg);
}
void PETSC_STDCALL  snesnasmsetdamping_(SNES snes,PetscReal *dmp, int *__ierr ){
*__ierr = SNESNASMSetDamping(
	(SNES)PetscToPointer((snes) ),*dmp);
}
void PETSC_STDCALL  snesnasmgetdamping_(SNES snes,PetscReal *dmp, int *__ierr ){
*__ierr = SNESNASMGetDamping(
	(SNES)PetscToPointer((snes) ),dmp);
}
#if defined(__cplusplus)
}
#endif
