#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* da.c */
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
#define dmdasetdim_ DMDASETDIM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetdim_ dmdasetdim
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetsizes_ DMDASETSIZES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetsizes_ dmdasetsizes
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetnumprocs_ DMDASETNUMPROCS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetnumprocs_ dmdasetnumprocs
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetboundarytype_ DMDASETBOUNDARYTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetboundarytype_ dmdasetboundarytype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetdof_ DMDASETDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetdof_ dmdasetdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdagetoverlap_ DMDAGETOVERLAP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdagetoverlap_ dmdagetoverlap
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetoverlap_ DMDASETOVERLAP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetoverlap_ dmdasetoverlap
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdagetnumlocalsubdomains_ DMDAGETNUMLOCALSUBDOMAINS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdagetnumlocalsubdomains_ dmdagetnumlocalsubdomains
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetnumlocalsubdomains_ DMDASETNUMLOCALSUBDOMAINS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetnumlocalsubdomains_ dmdasetnumlocalsubdomains
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetoffset_ DMDASETOFFSET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetoffset_ dmdasetoffset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdagetoffset_ DMDAGETOFFSET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdagetoffset_ dmdagetoffset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdagetnonoverlappingregion_ DMDAGETNONOVERLAPPINGREGION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdagetnonoverlappingregion_ dmdagetnonoverlappingregion
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetnonoverlappingregion_ DMDASETNONOVERLAPPINGREGION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetnonoverlappingregion_ dmdasetnonoverlappingregion
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetstenciltype_ DMDASETSTENCILTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetstenciltype_ dmdasetstenciltype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetstencilwidth_ DMDASETSTENCILWIDTH
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetstencilwidth_ dmdasetstencilwidth
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetownershipranges_ DMDASETOWNERSHIPRANGES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetownershipranges_ dmdasetownershipranges
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetinterpolationtype_ DMDASETINTERPOLATIONTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetinterpolationtype_ dmdasetinterpolationtype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdagetinterpolationtype_ DMDAGETINTERPOLATIONTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdagetinterpolationtype_ dmdagetinterpolationtype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmdasetrefinementfactor_ DMDASETREFINEMENTFACTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmdasetrefinementfactor_ dmdasetrefinementfactor
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmdasetdim_(DM da,PetscInt *dim, int *__ierr ){
*__ierr = DMDASetDim(
	(DM)PetscToPointer((da) ),*dim);
}
void PETSC_STDCALL  dmdasetsizes_(DM da,PetscInt *M,PetscInt *N,PetscInt *P, int *__ierr ){
*__ierr = DMDASetSizes(
	(DM)PetscToPointer((da) ),*M,*N,*P);
}
void PETSC_STDCALL  dmdasetnumprocs_(DM da,PetscInt *m,PetscInt *n,PetscInt *p, int *__ierr ){
*__ierr = DMDASetNumProcs(
	(DM)PetscToPointer((da) ),*m,*n,*p);
}
void PETSC_STDCALL  dmdasetboundarytype_(DM da,DMDABoundaryType *bx,DMDABoundaryType *by,DMDABoundaryType *bz, int *__ierr ){
*__ierr = DMDASetBoundaryType(
	(DM)PetscToPointer((da) ),*bx,*by,*bz);
}
void PETSC_STDCALL  dmdasetdof_(DM da,PetscInt *dof, int *__ierr ){
*__ierr = DMDASetDof(
	(DM)PetscToPointer((da) ),*dof);
}
void PETSC_STDCALL  dmdagetoverlap_(DM da,PetscInt *x,PetscInt *y,PetscInt *z, int *__ierr ){
*__ierr = DMDAGetOverlap(
	(DM)PetscToPointer((da) ),x,y,z);
}
void PETSC_STDCALL  dmdasetoverlap_(DM da,PetscInt *x,PetscInt *y,PetscInt *z, int *__ierr ){
*__ierr = DMDASetOverlap(
	(DM)PetscToPointer((da) ),*x,*y,*z);
}
void PETSC_STDCALL  dmdagetnumlocalsubdomains_(DM da,PetscInt *Nsub, int *__ierr ){
*__ierr = DMDAGetNumLocalSubDomains(
	(DM)PetscToPointer((da) ),Nsub);
}
void PETSC_STDCALL  dmdasetnumlocalsubdomains_(DM da,PetscInt *Nsub, int *__ierr ){
*__ierr = DMDASetNumLocalSubDomains(
	(DM)PetscToPointer((da) ),*Nsub);
}
void PETSC_STDCALL  dmdasetoffset_(DM da,PetscInt *xo,PetscInt *yo,PetscInt *zo,PetscInt *Mo,PetscInt *No,PetscInt *Po, int *__ierr ){
*__ierr = DMDASetOffset(
	(DM)PetscToPointer((da) ),*xo,*yo,*zo,*Mo,*No,*Po);
}
void PETSC_STDCALL  dmdagetoffset_(DM da,PetscInt *xo,PetscInt *yo,PetscInt *zo,PetscInt *Mo,PetscInt *No,PetscInt *Po, int *__ierr ){
*__ierr = DMDAGetOffset(
	(DM)PetscToPointer((da) ),xo,yo,zo,Mo,No,Po);
}
void PETSC_STDCALL  dmdagetnonoverlappingregion_(DM da,PetscInt *xs,PetscInt *ys,PetscInt *zs,PetscInt *xm,PetscInt *ym,PetscInt *zm, int *__ierr ){
*__ierr = DMDAGetNonOverlappingRegion(
	(DM)PetscToPointer((da) ),xs,ys,zs,xm,ym,zm);
}
void PETSC_STDCALL  dmdasetnonoverlappingregion_(DM da,PetscInt *xs,PetscInt *ys,PetscInt *zs,PetscInt *xm,PetscInt *ym,PetscInt *zm, int *__ierr ){
*__ierr = DMDASetNonOverlappingRegion(
	(DM)PetscToPointer((da) ),*xs,*ys,*zs,*xm,*ym,*zm);
}
void PETSC_STDCALL  dmdasetstenciltype_(DM da,DMDAStencilType *stype, int *__ierr ){
*__ierr = DMDASetStencilType(
	(DM)PetscToPointer((da) ),*stype);
}
void PETSC_STDCALL  dmdasetstencilwidth_(DM da,PetscInt *width, int *__ierr ){
*__ierr = DMDASetStencilWidth(
	(DM)PetscToPointer((da) ),*width);
}
void PETSC_STDCALL  dmdasetownershipranges_(DM da, PetscInt lx[], PetscInt ly[], PetscInt lz[], int *__ierr ){
*__ierr = DMDASetOwnershipRanges(
	(DM)PetscToPointer((da) ),lx,ly,lz);
}
void PETSC_STDCALL  dmdasetinterpolationtype_(DM da,DMDAInterpolationType *ctype, int *__ierr ){
*__ierr = DMDASetInterpolationType(
	(DM)PetscToPointer((da) ),*ctype);
}
void PETSC_STDCALL  dmdagetinterpolationtype_(DM da,DMDAInterpolationType *ctype, int *__ierr ){
*__ierr = DMDAGetInterpolationType(
	(DM)PetscToPointer((da) ),
	(DMDAInterpolationType* )PetscToPointer((ctype) ));
}
void PETSC_STDCALL  dmdasetrefinementfactor_(DM da,PetscInt *refine_x,PetscInt *refine_y,PetscInt *refine_z, int *__ierr ){
*__ierr = DMDASetRefinementFactor(
	(DM)PetscToPointer((da) ),*refine_x,*refine_y,*refine_z);
}
#if defined(__cplusplus)
}
#endif
