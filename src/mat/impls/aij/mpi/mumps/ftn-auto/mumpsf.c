#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* mumps.c */
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

#include "petscmat.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define matmumpsseticntl_ MATMUMPSSETICNTL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define matmumpsseticntl_ matmumpsseticntl
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define matmumpssetcntl_ MATMUMPSSETCNTL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define matmumpssetcntl_ matmumpssetcntl
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  matmumpsseticntl_(Mat F,PetscInt *icntl,PetscInt *ival, int *__ierr ){
*__ierr = MatMumpsSetIcntl(
	(Mat)PetscToPointer((F) ),*icntl,*ival);
}
void PETSC_STDCALL  matmumpssetcntl_(Mat F,PetscInt *icntl,PetscReal *val, int *__ierr ){
*__ierr = MatMumpsSetCntl(
	(Mat)PetscToPointer((F) ),*icntl,*val);
}
#if defined(__cplusplus)
}
#endif
