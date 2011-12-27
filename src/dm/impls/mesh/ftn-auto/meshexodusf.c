#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* meshexodus.c */
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
#define dmmeshexodusgetinfo_ DMMESHEXODUSGETINFO
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmmeshexodusgetinfo_ dmmeshexodusgetinfo
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmmeshexodusgetinfo_(DM dm,PetscInt *dim,PetscInt *numVertices,PetscInt *numCells,PetscInt *numCellBlocks,PetscInt *numVertexSets, int *__ierr ){
*__ierr = DMMeshExodusGetInfo(
	(DM)PetscToPointer((dm) ),dim,numVertices,numCells,numCellBlocks,numVertexSets);
}
#if defined(__cplusplus)
}
#endif
