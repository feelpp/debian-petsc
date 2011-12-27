#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* mesh.c */
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
#define dmmeshload_ DMMESHLOAD
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshload_ dmmeshload
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshcreatevector_ DMMESHCREATEVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshcreatevector_ dmmeshcreatevector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshcreateglobalscatter_ DMMESHCREATEGLOBALSCATTER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshcreateglobalscatter_ dmmeshcreateglobalscatter
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshgetglobalscatter_ DMMESHGETGLOBALSCATTER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshgetglobalscatter_ dmmeshgetglobalscatter
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshgetdimension_ DMMESHGETDIMENSION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshgetdimension_ dmmeshgetdimension
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshrestrictvector_ DMMESHRESTRICTVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshrestrictvector_ dmmeshrestrictvector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshassemblevectorcomplete_ DMMESHASSEMBLEVECTORCOMPLETE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshassemblevectorcomplete_ dmmeshassemblevectorcomplete
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshassemblevector_ DMMESHASSEMBLEVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshassemblevector_ dmmeshassemblevector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshsetmaxdof_ DMMESHSETMAXDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshsetmaxdof_ dmmeshsetmaxdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmmeshassemblematrix_ DMMESHASSEMBLEMATRIX
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshassemblematrix_ dmmeshassemblematrix
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmmeshload_(PetscViewer viewer,DM dm, int *__ierr ){
*__ierr = DMMeshLoad(
	(PetscViewer)PetscToPointer((viewer) ),
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  dmmeshcreatevector_(DM mesh,SectionReal section,Vec *vec, int *__ierr ){
*__ierr = DMMeshCreateVector(
	(DM)PetscToPointer((mesh) ),
	(SectionReal)PetscToPointer((section) ),vec);
}
void PETSC_STDCALL  dmmeshcreateglobalscatter_(DM dm,SectionReal section,VecScatter *scatter, int *__ierr ){
*__ierr = DMMeshCreateGlobalScatter(
	(DM)PetscToPointer((dm) ),
	(SectionReal)PetscToPointer((section) ),scatter);
}
void PETSC_STDCALL  dmmeshgetglobalscatter_(DM dm,VecScatter *scatter, int *__ierr ){
*__ierr = DMMeshGetGlobalScatter(
	(DM)PetscToPointer((dm) ),scatter);
}
void PETSC_STDCALL  dmmeshgetdimension_(DM dm,PetscInt *dim, int *__ierr ){
*__ierr = DMMeshGetDimension(
	(DM)PetscToPointer((dm) ),dim);
}
void PETSC_STDCALL  dmmeshrestrictvector_(Vec g,Vec l,InsertMode *mode, int *__ierr ){
*__ierr = DMMeshRestrictVector(
	(Vec)PetscToPointer((g) ),
	(Vec)PetscToPointer((l) ),*mode);
}
void PETSC_STDCALL  dmmeshassemblevectorcomplete_(Vec g,Vec l,InsertMode *mode, int *__ierr ){
*__ierr = DMMeshAssembleVectorComplete(
	(Vec)PetscToPointer((g) ),
	(Vec)PetscToPointer((l) ),*mode);
}
void PETSC_STDCALL  dmmeshassemblevector_(Vec b,PetscInt *e,PetscScalar v[],InsertMode *mode, int *__ierr ){
*__ierr = DMMeshAssembleVector(
	(Vec)PetscToPointer((b) ),*e,v,*mode);
}
void PETSC_STDCALL  dmmeshsetmaxdof_(DM dm,PetscInt *maxDof, int *__ierr ){
*__ierr = DMMeshSetMaxDof(
	(DM)PetscToPointer((dm) ),*maxDof);
}
void PETSC_STDCALL  dmmeshassemblematrix_(Mat A,DM dm,SectionReal section,PetscInt *e,PetscScalar v[],InsertMode *mode, int *__ierr ){
*__ierr = DMMeshAssembleMatrix(
	(Mat)PetscToPointer((A) ),
	(DM)PetscToPointer((dm) ),
	(SectionReal)PetscToPointer((section) ),*e,v,*mode);
}
#if defined(__cplusplus)
}
#endif
