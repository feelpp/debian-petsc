#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* da2.c */
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

#include "petscdmda.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdaformfunction_ DMDAFORMFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdaformfunction_ dmdaformfunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdaformfunction1_ DMDAFORMFUNCTION1
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdaformfunction1_ dmdaformfunction1
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdaformfunctioni1_ DMDAFORMFUNCTIONI1
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdaformfunctioni1_ dmdaformfunctioni1
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdaformfunctionib1_ DMDAFORMFUNCTIONIB1
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdaformfunctionib1_ dmdaformfunctionib1
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdacomputejacobian1_ DMDACOMPUTEJACOBIAN1
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdacomputejacobian1_ dmdacomputejacobian1
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmdaformfunction_(DM da,PetscErrorCode (*lf)(void),Vec vu,Vec vfu,void*w, int *__ierr ){
*__ierr = DMDAFormFunction(
	(DM)PetscToPointer((da) ),lf,
	(Vec)PetscToPointer((vu) ),
	(Vec)PetscToPointer((vfu) ),w);
}
void PETSC_STDCALL  dmdaformfunction1_(DM da,Vec vu,Vec vfu,void*w, int *__ierr ){
*__ierr = DMDAFormFunction1(
	(DM)PetscToPointer((da) ),
	(Vec)PetscToPointer((vu) ),
	(Vec)PetscToPointer((vfu) ),w);
}
void PETSC_STDCALL  dmdaformfunctioni1_(DM da,PetscInt *i,Vec vu,PetscScalar *vfu,void*w, int *__ierr ){
*__ierr = DMDAFormFunctioni1(
	(DM)PetscToPointer((da) ),*i,
	(Vec)PetscToPointer((vu) ),vfu,w);
}
void PETSC_STDCALL  dmdaformfunctionib1_(DM da,PetscInt *i,Vec vu,PetscScalar *vfu,void*w, int *__ierr ){
*__ierr = DMDAFormFunctionib1(
	(DM)PetscToPointer((da) ),*i,
	(Vec)PetscToPointer((vu) ),vfu,w);
}
void PETSC_STDCALL  dmdacomputejacobian1_(DM da,Vec vu,Mat J,void*w, int *__ierr ){
*__ierr = DMDAComputeJacobian1(
	(DM)PetscToPointer((da) ),
	(Vec)PetscToPointer((vu) ),
	(Mat)PetscToPointer((J) ),w);
}
#if defined(__cplusplus)
}
#endif
