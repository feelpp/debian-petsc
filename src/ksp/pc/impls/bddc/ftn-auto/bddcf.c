#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* bddc.c */
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

#include "petscpc.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetcoarseproblemtype_ PCBDDCSETCOARSEPROBLEMTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetcoarseproblemtype_ pcbddcsetcoarseproblemtype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetcoarseningratio_ PCBDDCSETCOARSENINGRATIO
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetcoarseningratio_ pcbddcsetcoarseningratio
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetmaxlevels_ PCBDDCSETMAXLEVELS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetmaxlevels_ pcbddcsetmaxlevels
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetnullspace_ PCBDDCSETNULLSPACE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetnullspace_ pcbddcsetnullspace
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetdirichletboundaries_ PCBDDCSETDIRICHLETBOUNDARIES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetdirichletboundaries_ pcbddcsetdirichletboundaries
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetneumannboundaries_ PCBDDCSETNEUMANNBOUNDARIES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetneumannboundaries_ pcbddcsetneumannboundaries
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcgetdirichletboundaries_ PCBDDCGETDIRICHLETBOUNDARIES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcgetdirichletboundaries_ pcbddcgetdirichletboundaries
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcgetneumannboundaries_ PCBDDCGETNEUMANNBOUNDARIES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcgetneumannboundaries_ pcbddcgetneumannboundaries
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetlocaladjacencygraph_ PCBDDCSETLOCALADJACENCYGRAPH
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetlocaladjacencygraph_ pcbddcsetlocaladjacencygraph
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcsetdofssplitting_ PCBDDCSETDOFSSPLITTING
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcsetdofssplitting_ pcbddcsetdofssplitting
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcmatfetidpgetrhs_ PCBDDCMATFETIDPGETRHS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcmatfetidpgetrhs_ pcbddcmatfetidpgetrhs
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddcmatfetidpgetsolution_ PCBDDCMATFETIDPGETSOLUTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddcmatfetidpgetsolution_ pcbddcmatfetidpgetsolution
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define pcbddccreatefetidpoperators_ PCBDDCCREATEFETIDPOPERATORS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define pcbddccreatefetidpoperators_ pcbddccreatefetidpoperators
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  pcbddcsetcoarseproblemtype_(PC pc,CoarseProblemType *CPT, int *__ierr ){
*__ierr = PCBDDCSetCoarseProblemType(
	(PC)PetscToPointer((pc) ),*CPT);
}
void PETSC_STDCALL  pcbddcsetcoarseningratio_(PC pc,PetscInt *k, int *__ierr ){
*__ierr = PCBDDCSetCoarseningRatio(
	(PC)PetscToPointer((pc) ),*k);
}
void PETSC_STDCALL  pcbddcsetmaxlevels_(PC pc,PetscInt *max_levels, int *__ierr ){
*__ierr = PCBDDCSetMaxLevels(
	(PC)PetscToPointer((pc) ),*max_levels);
}
void PETSC_STDCALL  pcbddcsetnullspace_(PC pc,MatNullSpace NullSpace, int *__ierr ){
*__ierr = PCBDDCSetNullSpace(
	(PC)PetscToPointer((pc) ),
	(MatNullSpace)PetscToPointer((NullSpace) ));
}
void PETSC_STDCALL  pcbddcsetdirichletboundaries_(PC pc,IS DirichletBoundaries, int *__ierr ){
*__ierr = PCBDDCSetDirichletBoundaries(
	(PC)PetscToPointer((pc) ),
	(IS)PetscToPointer((DirichletBoundaries) ));
}
void PETSC_STDCALL  pcbddcsetneumannboundaries_(PC pc,IS NeumannBoundaries, int *__ierr ){
*__ierr = PCBDDCSetNeumannBoundaries(
	(PC)PetscToPointer((pc) ),
	(IS)PetscToPointer((NeumannBoundaries) ));
}
void PETSC_STDCALL  pcbddcgetdirichletboundaries_(PC pc,IS *DirichletBoundaries, int *__ierr ){
*__ierr = PCBDDCGetDirichletBoundaries(
	(PC)PetscToPointer((pc) ),DirichletBoundaries);
}
void PETSC_STDCALL  pcbddcgetneumannboundaries_(PC pc,IS *NeumannBoundaries, int *__ierr ){
*__ierr = PCBDDCGetNeumannBoundaries(
	(PC)PetscToPointer((pc) ),NeumannBoundaries);
}
void PETSC_STDCALL  pcbddcsetlocaladjacencygraph_(PC pc,PetscInt *nvtxs, PetscInt xadj[], PetscInt adjncy[],PetscCopyMode *copymode, int *__ierr ){
*__ierr = PCBDDCSetLocalAdjacencyGraph(
	(PC)PetscToPointer((pc) ),*nvtxs,xadj,adjncy,*copymode);
}
void PETSC_STDCALL  pcbddcsetdofssplitting_(PC pc,PetscInt *n_is,IS ISForDofs[], int *__ierr ){
*__ierr = PCBDDCSetDofsSplitting(
	(PC)PetscToPointer((pc) ),*n_is,ISForDofs);
}
void PETSC_STDCALL  pcbddcmatfetidpgetrhs_(Mat fetidp_mat,Vec standard_rhs,Vec fetidp_flux_rhs, int *__ierr ){
*__ierr = PCBDDCMatFETIDPGetRHS(
	(Mat)PetscToPointer((fetidp_mat) ),
	(Vec)PetscToPointer((standard_rhs) ),
	(Vec)PetscToPointer((fetidp_flux_rhs) ));
}
void PETSC_STDCALL  pcbddcmatfetidpgetsolution_(Mat fetidp_mat,Vec fetidp_flux_sol,Vec standard_sol, int *__ierr ){
*__ierr = PCBDDCMatFETIDPGetSolution(
	(Mat)PetscToPointer((fetidp_mat) ),
	(Vec)PetscToPointer((fetidp_flux_sol) ),
	(Vec)PetscToPointer((standard_sol) ));
}
void PETSC_STDCALL  pcbddccreatefetidpoperators_(PC pc,Mat *fetidp_mat,PC *fetidp_pc, int *__ierr ){
*__ierr = PCBDDCCreateFETIDPOperators(
	(PC)PetscToPointer((pc) ),fetidp_mat,fetidp_pc);
}
#if defined(__cplusplus)
}
#endif
