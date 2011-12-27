#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* meshcreate.c */
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

#include "petscdmmesh.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshcreate_ DMMESHCREATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshcreate_ dmmeshcreate
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmmeshcreate_(MPI_Fint * comm,DM *mesh, int *__ierr ){
*__ierr = DMMeshCreate(
	MPI_Comm_f2c( *(comm) ),mesh);
}
#if defined(__cplusplus)
}
#endif
