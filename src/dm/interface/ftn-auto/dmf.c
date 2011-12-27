#include "petscsys.h"
#include "petscfix.h"
#include "private/fortranimpl.h"
/* dm.c */
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

#include "petscdm.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcreate_ DMCREATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcreate_ dmcreate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdestroy_ DMDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdestroy_ dmdestroy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetup_ DMSETUP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetup_ dmsetup
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetfromoptions_ DMSETFROMOPTIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetfromoptions_ dmsetfromoptions
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcreateglobalvector_ DMCREATEGLOBALVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcreateglobalvector_ dmcreateglobalvector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcreatelocalvector_ DMCREATELOCALVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcreatelocalvector_ dmcreatelocalvector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetlocaltoglobalmapping_ DMGETLOCALTOGLOBALMAPPING
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetlocaltoglobalmapping_ dmgetlocaltoglobalmapping
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetlocaltoglobalmappingblock_ DMGETLOCALTOGLOBALMAPPINGBLOCK
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetlocaltoglobalmappingblock_ dmgetlocaltoglobalmappingblock
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetblocksize_ DMGETBLOCKSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetblocksize_ dmgetblocksize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetinterpolation_ DMGETINTERPOLATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetinterpolation_ dmgetinterpolation
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetinjection_ DMGETINJECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetinjection_ dmgetinjection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetmatrixpreallocateonly_ DMSETMATRIXPREALLOCATEONLY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetmatrixpreallocateonly_ dmsetmatrixpreallocateonly
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmrefine_ DMREFINE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmrefine_ dmrefine
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetrefinelevel_ DMGETREFINELEVEL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetrefinelevel_ dmgetrefinelevel
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmglobaltolocalbegin_ DMGLOBALTOLOCALBEGIN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmglobaltolocalbegin_ dmglobaltolocalbegin
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmglobaltolocalend_ DMGLOBALTOLOCALEND
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmglobaltolocalend_ dmglobaltolocalend
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmlocaltoglobalbegin_ DMLOCALTOGLOBALBEGIN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmlocaltoglobalbegin_ dmlocaltoglobalbegin
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmlocaltoglobalend_ DMLOCALTOGLOBALEND
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmlocaltoglobalend_ dmlocaltoglobalend
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcomputejacobiandefault_ DMCOMPUTEJACOBIANDEFAULT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcomputejacobiandefault_ dmcomputejacobiandefault
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcoarsen_ DMCOARSEN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcoarsen_ dmcoarsen
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetaggregates_ DMGETAGGREGATES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetaggregates_ dmgetaggregates
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetapplicationcontext_ DMSETAPPLICATIONCONTEXT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetapplicationcontext_ dmsetapplicationcontext
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetapplicationcontext_ DMGETAPPLICATIONCONTEXT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetapplicationcontext_ dmgetapplicationcontext
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcomputeinitialguess_ DMCOMPUTEINITIALGUESS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcomputeinitialguess_ dmcomputeinitialguess
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmhasinitialguess_ DMHASINITIALGUESS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmhasinitialguess_ dmhasinitialguess
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmhasfunction_ DMHASFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmhasfunction_ dmhasfunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmhasjacobian_ DMHASJACOBIAN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmhasjacobian_ dmhasjacobian
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcomputefunction_ DMCOMPUTEFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcomputefunction_ dmcomputefunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcomputejacobian_ DMCOMPUTEJACOBIAN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcomputejacobian_ dmcomputejacobian
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmcreate_(MPI_Fint * comm,DM *dm, int *__ierr ){
*__ierr = DMCreate(
	MPI_Comm_f2c( *(comm) ),dm);
}
void PETSC_STDCALL  dmdestroy_(DM *dm, int *__ierr ){
*__ierr = DMDestroy(dm);
}
void PETSC_STDCALL  dmsetup_(DM dm, int *__ierr ){
*__ierr = DMSetUp(
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  dmsetfromoptions_(DM dm, int *__ierr ){
*__ierr = DMSetFromOptions(
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  dmcreateglobalvector_(DM dm,Vec *vec, int *__ierr ){
*__ierr = DMCreateGlobalVector(
	(DM)PetscToPointer((dm) ),vec);
}
void PETSC_STDCALL  dmcreatelocalvector_(DM dm,Vec *vec, int *__ierr ){
*__ierr = DMCreateLocalVector(
	(DM)PetscToPointer((dm) ),vec);
}
void PETSC_STDCALL  dmgetlocaltoglobalmapping_(DM dm,ISLocalToGlobalMapping *ltog, int *__ierr ){
*__ierr = DMGetLocalToGlobalMapping(
	(DM)PetscToPointer((dm) ),ltog);
}
void PETSC_STDCALL  dmgetlocaltoglobalmappingblock_(DM dm,ISLocalToGlobalMapping *ltog, int *__ierr ){
*__ierr = DMGetLocalToGlobalMappingBlock(
	(DM)PetscToPointer((dm) ),ltog);
}
void PETSC_STDCALL  dmgetblocksize_(DM dm,PetscInt *bs, int *__ierr ){
*__ierr = DMGetBlockSize(
	(DM)PetscToPointer((dm) ),bs);
}
void PETSC_STDCALL  dmgetinterpolation_(DM dm1,DM dm2,Mat *mat,Vec *vec, int *__ierr ){
*__ierr = DMGetInterpolation(
	(DM)PetscToPointer((dm1) ),
	(DM)PetscToPointer((dm2) ),mat,vec);
}
void PETSC_STDCALL  dmgetinjection_(DM dm1,DM dm2,VecScatter *ctx, int *__ierr ){
*__ierr = DMGetInjection(
	(DM)PetscToPointer((dm1) ),
	(DM)PetscToPointer((dm2) ),ctx);
}
void PETSC_STDCALL  dmsetmatrixpreallocateonly_(DM dm,PetscBool *only, int *__ierr ){
*__ierr = DMSetMatrixPreallocateOnly(
	(DM)PetscToPointer((dm) ),*only);
}
void PETSC_STDCALL  dmrefine_(DM dm,MPI_Fint * comm,DM *dmf, int *__ierr ){
*__ierr = DMRefine(
	(DM)PetscToPointer((dm) ),
	MPI_Comm_f2c( *(comm) ),dmf);
}
void PETSC_STDCALL  dmgetrefinelevel_(DM dm,PetscInt *level, int *__ierr ){
*__ierr = DMGetRefineLevel(
	(DM)PetscToPointer((dm) ),level);
}
void PETSC_STDCALL  dmglobaltolocalbegin_(DM dm,Vec g,InsertMode *mode,Vec l, int *__ierr ){
*__ierr = DMGlobalToLocalBegin(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((g) ),*mode,
	(Vec)PetscToPointer((l) ));
}
void PETSC_STDCALL  dmglobaltolocalend_(DM dm,Vec g,InsertMode *mode,Vec l, int *__ierr ){
*__ierr = DMGlobalToLocalEnd(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((g) ),*mode,
	(Vec)PetscToPointer((l) ));
}
void PETSC_STDCALL  dmlocaltoglobalbegin_(DM dm,Vec l,InsertMode *mode,Vec g, int *__ierr ){
*__ierr = DMLocalToGlobalBegin(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((l) ),*mode,
	(Vec)PetscToPointer((g) ));
}
void PETSC_STDCALL  dmlocaltoglobalend_(DM dm,Vec l,InsertMode *mode,Vec g, int *__ierr ){
*__ierr = DMLocalToGlobalEnd(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((l) ),*mode,
	(Vec)PetscToPointer((g) ));
}
void PETSC_STDCALL  dmcomputejacobiandefault_(DM dm,Vec x,Mat A,Mat B,MatStructure *stflag, int *__ierr ){
*__ierr = DMComputeJacobianDefault(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((x) ),
	(Mat)PetscToPointer((A) ),
	(Mat)PetscToPointer((B) ),stflag);
}
void PETSC_STDCALL  dmcoarsen_(DM dm,MPI_Fint * comm,DM *dmc, int *__ierr ){
*__ierr = DMCoarsen(
	(DM)PetscToPointer((dm) ),
	MPI_Comm_f2c( *(comm) ),dmc);
}
void PETSC_STDCALL  dmgetaggregates_(DM dmc,DM dmf,Mat *rest, int *__ierr ){
*__ierr = DMGetAggregates(
	(DM)PetscToPointer((dmc) ),
	(DM)PetscToPointer((dmf) ),rest);
}
void PETSC_STDCALL  dmsetapplicationcontext_(DM dm,void*ctx, int *__ierr ){
*__ierr = DMSetApplicationContext(
	(DM)PetscToPointer((dm) ),ctx);
}
void PETSC_STDCALL  dmgetapplicationcontext_(DM dm,void*ctx, int *__ierr ){
*__ierr = DMGetApplicationContext(
	(DM)PetscToPointer((dm) ),ctx);
}
void PETSC_STDCALL  dmcomputeinitialguess_(DM dm,Vec x, int *__ierr ){
*__ierr = DMComputeInitialGuess(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((x) ));
}
void PETSC_STDCALL  dmhasinitialguess_(DM dm,PetscBool  *flg, int *__ierr ){
*__ierr = DMHasInitialGuess(
	(DM)PetscToPointer((dm) ),flg);
}
void PETSC_STDCALL  dmhasfunction_(DM dm,PetscBool  *flg, int *__ierr ){
*__ierr = DMHasFunction(
	(DM)PetscToPointer((dm) ),flg);
}
void PETSC_STDCALL  dmhasjacobian_(DM dm,PetscBool  *flg, int *__ierr ){
*__ierr = DMHasJacobian(
	(DM)PetscToPointer((dm) ),flg);
}
void PETSC_STDCALL  dmcomputefunction_(DM dm,Vec x,Vec b, int *__ierr ){
*__ierr = DMComputeFunction(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((x) ),
	(Vec)PetscToPointer((b) ));
}
void PETSC_STDCALL  dmcomputejacobian_(DM dm,Vec x,Mat A,Mat B,MatStructure *stflag, int *__ierr ){
*__ierr = DMComputeJacobian(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((x) ),
	(Mat)PetscToPointer((A) ),
	(Mat)PetscToPointer((B) ),stflag);
}
#if defined(__cplusplus)
}
#endif
