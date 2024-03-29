#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* randomc.c */
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
#define petscrandomdestroy_ PETSCRANDOMDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscrandomdestroy_ petscrandomdestroy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscrandomgetseed_ PETSCRANDOMGETSEED
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscrandomgetseed_ petscrandomgetseed
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscrandomsetseed_ PETSCRANDOMSETSEED
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscrandomsetseed_ petscrandomsetseed
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscrandomsetfromoptions_ PETSCRANDOMSETFROMOPTIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscrandomsetfromoptions_ petscrandomsetfromoptions
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscrandomcreate_ PETSCRANDOMCREATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscrandomcreate_ petscrandomcreate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscrandomseed_ PETSCRANDOMSEED
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscrandomseed_ petscrandomseed
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscrandomdestroy_(PetscRandom *r, int *__ierr ){
*__ierr = PetscRandomDestroy(r);
}
void PETSC_STDCALL  petscrandomgetseed_(PetscRandom r,unsigned long *seed, int *__ierr ){
*__ierr = PetscRandomGetSeed(
	(PetscRandom)PetscToPointer((r) ),seed);
}
void PETSC_STDCALL  petscrandomsetseed_(PetscRandom r,unsigned long *seed, int *__ierr ){
*__ierr = PetscRandomSetSeed(
	(PetscRandom)PetscToPointer((r) ),*seed);
}
void PETSC_STDCALL  petscrandomsetfromoptions_(PetscRandom rnd, int *__ierr ){
*__ierr = PetscRandomSetFromOptions(
	(PetscRandom)PetscToPointer((rnd) ));
}

void PETSC_STDCALL  petscrandomcreate_(MPI_Fint * comm,PetscRandom *r, int *__ierr ){
*__ierr = PetscRandomCreate(
	MPI_Comm_f2c( *(comm) ),r);
}
void PETSC_STDCALL  petscrandomseed_(PetscRandom r, int *__ierr ){
*__ierr = PetscRandomSeed(
	(PetscRandom)PetscToPointer((r) ));
}
#if defined(__cplusplus)
}
#endif
