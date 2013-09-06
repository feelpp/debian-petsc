#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
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
#define vecgetdm_ VECGETDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define vecgetdm_ vecgetdm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define vecsetdm_ VECSETDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define vecsetdm_ vecsetdm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define matgetdm_ MATGETDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define matgetdm_ matgetdm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define matsetdm_ MATSETDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define matsetdm_ matsetdm
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
#define dmcreateinterpolation_ DMCREATEINTERPOLATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcreateinterpolation_ dmcreateinterpolation
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcreateinjection_ DMCREATEINJECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcreateinjection_ dmcreateinjection
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
#define dminterpolate_ DMINTERPOLATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dminterpolate_ dminterpolate
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
#define dmcoarsen_ DMCOARSEN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcoarsen_ dmcoarsen
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmrestrict_ DMRESTRICT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmrestrict_ dmrestrict
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsubdomainrestrict_ DMSUBDOMAINRESTRICT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsubdomainrestrict_ dmsubdomainrestrict
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetcoarsenlevel_ DMGETCOARSENLEVEL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetcoarsenlevel_ dmgetcoarsenlevel
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmcreateaggregates_ DMCREATEAGGREGATES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmcreateaggregates_ dmcreateaggregates
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
#define dmhasvariablebounds_ DMHASVARIABLEBOUNDS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmhasvariablebounds_ dmhasvariablebounds
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmhascoloring_ DMHASCOLORING
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmhascoloring_ dmhascoloring
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetdefaultsection_ DMGETDEFAULTSECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetdefaultsection_ dmgetdefaultsection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetdefaultsection_ DMSETDEFAULTSECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetdefaultsection_ dmsetdefaultsection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetdefaultglobalsection_ DMGETDEFAULTGLOBALSECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetdefaultglobalsection_ dmgetdefaultglobalsection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetdefaultglobalsection_ DMSETDEFAULTGLOBALSECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetdefaultglobalsection_ dmsetdefaultglobalsection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetdefaultsf_ DMGETDEFAULTSF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetdefaultsf_ dmgetdefaultsf
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetdefaultsf_ DMSETDEFAULTSF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetdefaultsf_ dmsetdefaultsf
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetpointsf_ DMGETPOINTSF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetpointsf_ dmgetpointsf
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetpointsf_ DMSETPOINTSF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetpointsf_ dmsetpointsf
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetcoordinates_ DMSETCOORDINATES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetcoordinates_ dmsetcoordinates
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmsetcoordinateslocal_ DMSETCOORDINATESLOCAL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmsetcoordinateslocal_ dmsetcoordinateslocal
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetcoordinates_ DMGETCOORDINATES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetcoordinates_ dmgetcoordinates
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetcoordinateslocal_ DMGETCOORDINATESLOCAL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetcoordinateslocal_ dmgetcoordinateslocal
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmgetcoordinatedm_ DMGETCOORDINATEDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmgetcoordinatedm_ dmgetcoordinatedm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmlocatepoints_ DMLOCATEPOINTS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmlocatepoints_ dmlocatepoints
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmcreate_(MPI_Fint * comm,DM *dm, int *__ierr ){
*__ierr = DMCreate(
	MPI_Comm_f2c( *(comm) ),dm);
}
void PETSC_STDCALL  vecgetdm_(Vec v,DM *dm, int *__ierr ){
*__ierr = VecGetDM(
	(Vec)PetscToPointer((v) ),dm);
}
void PETSC_STDCALL  vecsetdm_(Vec v,DM dm, int *__ierr ){
*__ierr = VecSetDM(
	(Vec)PetscToPointer((v) ),
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  matgetdm_(Mat A,DM *dm, int *__ierr ){
*__ierr = MatGetDM(
	(Mat)PetscToPointer((A) ),dm);
}
void PETSC_STDCALL  matsetdm_(Mat A,DM dm, int *__ierr ){
*__ierr = MatSetDM(
	(Mat)PetscToPointer((A) ),
	(DM)PetscToPointer((dm) ));
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
void PETSC_STDCALL  dmcreateinterpolation_(DM dm1,DM dm2,Mat *mat,Vec *vec, int *__ierr ){
*__ierr = DMCreateInterpolation(
	(DM)PetscToPointer((dm1) ),
	(DM)PetscToPointer((dm2) ),mat,vec);
}
void PETSC_STDCALL  dmcreateinjection_(DM dm1,DM dm2,VecScatter *ctx, int *__ierr ){
*__ierr = DMCreateInjection(
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
void PETSC_STDCALL  dminterpolate_(DM coarse,Mat interp,DM fine, int *__ierr ){
*__ierr = DMInterpolate(
	(DM)PetscToPointer((coarse) ),
	(Mat)PetscToPointer((interp) ),
	(DM)PetscToPointer((fine) ));
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
void PETSC_STDCALL  dmcoarsen_(DM dm,MPI_Fint * comm,DM *dmc, int *__ierr ){
*__ierr = DMCoarsen(
	(DM)PetscToPointer((dm) ),
	MPI_Comm_f2c( *(comm) ),dmc);
}
void PETSC_STDCALL  dmrestrict_(DM fine,Mat restrct,Vec rscale,Mat inject,DM coarse, int *__ierr ){
*__ierr = DMRestrict(
	(DM)PetscToPointer((fine) ),
	(Mat)PetscToPointer((restrct) ),
	(Vec)PetscToPointer((rscale) ),
	(Mat)PetscToPointer((inject) ),
	(DM)PetscToPointer((coarse) ));
}
void PETSC_STDCALL  dmsubdomainrestrict_(DM global,VecScatter oscatter,VecScatter gscatter,DM subdm, int *__ierr ){
*__ierr = DMSubDomainRestrict(
	(DM)PetscToPointer((global) ),
	(VecScatter)PetscToPointer((oscatter) ),
	(VecScatter)PetscToPointer((gscatter) ),
	(DM)PetscToPointer((subdm) ));
}
void PETSC_STDCALL  dmgetcoarsenlevel_(DM dm,PetscInt *level, int *__ierr ){
*__ierr = DMGetCoarsenLevel(
	(DM)PetscToPointer((dm) ),level);
}
void PETSC_STDCALL  dmcreateaggregates_(DM dmc,DM dmf,Mat *rest, int *__ierr ){
*__ierr = DMCreateAggregates(
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
void PETSC_STDCALL  dmhasvariablebounds_(DM dm,PetscBool  *flg, int *__ierr ){
*__ierr = DMHasVariableBounds(
	(DM)PetscToPointer((dm) ),flg);
}
void PETSC_STDCALL  dmhascoloring_(DM dm,PetscBool  *flg, int *__ierr ){
*__ierr = DMHasColoring(
	(DM)PetscToPointer((dm) ),flg);
}
void PETSC_STDCALL  dmgetdefaultsection_(DM dm,PetscSection *section, int *__ierr ){
*__ierr = DMGetDefaultSection(
	(DM)PetscToPointer((dm) ),section);
}
void PETSC_STDCALL  dmsetdefaultsection_(DM dm,PetscSection section, int *__ierr ){
*__ierr = DMSetDefaultSection(
	(DM)PetscToPointer((dm) ),
	(PetscSection)PetscToPointer((section) ));
}
void PETSC_STDCALL  dmgetdefaultglobalsection_(DM dm,PetscSection *section, int *__ierr ){
*__ierr = DMGetDefaultGlobalSection(
	(DM)PetscToPointer((dm) ),section);
}
void PETSC_STDCALL  dmsetdefaultglobalsection_(DM dm,PetscSection section, int *__ierr ){
*__ierr = DMSetDefaultGlobalSection(
	(DM)PetscToPointer((dm) ),
	(PetscSection)PetscToPointer((section) ));
}
void PETSC_STDCALL  dmgetdefaultsf_(DM dm,PetscSF *sf, int *__ierr ){
*__ierr = DMGetDefaultSF(
	(DM)PetscToPointer((dm) ),
	(PetscSF* )PetscToPointer((sf) ));
}
void PETSC_STDCALL  dmsetdefaultsf_(DM dm,PetscSF *sf, int *__ierr ){
*__ierr = DMSetDefaultSF(
	(DM)PetscToPointer((dm) ),*sf);
}
void PETSC_STDCALL  dmgetpointsf_(DM dm,PetscSF *sf, int *__ierr ){
*__ierr = DMGetPointSF(
	(DM)PetscToPointer((dm) ),
	(PetscSF* )PetscToPointer((sf) ));
}
void PETSC_STDCALL  dmsetpointsf_(DM dm,PetscSF *sf, int *__ierr ){
*__ierr = DMSetPointSF(
	(DM)PetscToPointer((dm) ),*sf);
}
void PETSC_STDCALL  dmsetcoordinates_(DM dm,Vec c, int *__ierr ){
*__ierr = DMSetCoordinates(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((c) ));
}
void PETSC_STDCALL  dmsetcoordinateslocal_(DM dm,Vec c, int *__ierr ){
*__ierr = DMSetCoordinatesLocal(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((c) ));
}
void PETSC_STDCALL  dmgetcoordinates_(DM dm,Vec *c, int *__ierr ){
*__ierr = DMGetCoordinates(
	(DM)PetscToPointer((dm) ),c);
}
void PETSC_STDCALL  dmgetcoordinateslocal_(DM dm,Vec *c, int *__ierr ){
*__ierr = DMGetCoordinatesLocal(
	(DM)PetscToPointer((dm) ),c);
}
void PETSC_STDCALL  dmgetcoordinatedm_(DM dm,DM *cdm, int *__ierr ){
*__ierr = DMGetCoordinateDM(
	(DM)PetscToPointer((dm) ),cdm);
}
void PETSC_STDCALL  dmlocatepoints_(DM dm,Vec v,IS *cells, int *__ierr ){
*__ierr = DMLocatePoints(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((v) ),cells);
}
#if defined(__cplusplus)
}
#endif
