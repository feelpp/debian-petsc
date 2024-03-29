#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* veccusp.cu */
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

#include "petscvec.h"


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  veccreateseqcusp_(MPI_Fint * comm,PetscInt *n,Vec *v, int *__ierr ){
*__ierr = VecCreateSeqCUSP(
	MPI_Comm_f2c( *(comm) ),*n,v);
}
#if defined(__cplusplus)
}
#endif
