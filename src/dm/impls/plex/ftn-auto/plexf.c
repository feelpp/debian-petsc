#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* plex.c */
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

#include "petscdmplex.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetdimension_ DMPLEXGETDIMENSION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetdimension_ dmplexgetdimension
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetdimension_ DMPLEXSETDIMENSION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetdimension_ dmplexsetdimension
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetchart_ DMPLEXGETCHART
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetchart_ dmplexgetchart
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetchart_ DMPLEXSETCHART
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetchart_ dmplexsetchart
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetconesize_ DMPLEXGETCONESIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetconesize_ dmplexgetconesize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetconesize_ DMPLEXSETCONESIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetconesize_ dmplexsetconesize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetcone_ DMPLEXSETCONE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetcone_ dmplexsetcone
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetconeorientation_ DMPLEXSETCONEORIENTATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetconeorientation_ dmplexsetconeorientation
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetsupportsize_ DMPLEXGETSUPPORTSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetsupportsize_ dmplexgetsupportsize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetsupportsize_ DMPLEXSETSUPPORTSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetsupportsize_ dmplexsetsupportsize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetsupport_ DMPLEXSETSUPPORT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetsupport_ dmplexsetsupport
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetmaxsizes_ DMPLEXGETMAXSIZES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetmaxsizes_ dmplexgetmaxsizes
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsymmetrize_ DMPLEXSYMMETRIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsymmetrize_ dmplexsymmetrize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexstratify_ DMPLEXSTRATIFY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexstratify_ dmplexstratify
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetdepth_ DMPLEXGETDEPTH
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetdepth_ dmplexgetdepth
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetdepthstratum_ DMPLEXGETDEPTHSTRATUM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetdepthstratum_ dmplexgetdepthstratum
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetheightstratum_ DMPLEXGETHEIGHTSTRATUM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetheightstratum_ dmplexgetheightstratum
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexgetcoordinatesection_ DMPLEXGETCOORDINATESECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexgetcoordinatesection_ dmplexgetcoordinatesection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmplexsetcoordinatesection_ DMPLEXSETCOORDINATESECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmplexsetcoordinatesection_ dmplexsetcoordinatesection
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmplexgetdimension_(DM dm,PetscInt *dim, int *__ierr ){
*__ierr = DMPlexGetDimension(
	(DM)PetscToPointer((dm) ),dim);
}
void PETSC_STDCALL  dmplexsetdimension_(DM dm,PetscInt *dim, int *__ierr ){
*__ierr = DMPlexSetDimension(
	(DM)PetscToPointer((dm) ),*dim);
}
void PETSC_STDCALL  dmplexgetchart_(DM dm,PetscInt *pStart,PetscInt *pEnd, int *__ierr ){
*__ierr = DMPlexGetChart(
	(DM)PetscToPointer((dm) ),pStart,pEnd);
}
void PETSC_STDCALL  dmplexsetchart_(DM dm,PetscInt *pStart,PetscInt *pEnd, int *__ierr ){
*__ierr = DMPlexSetChart(
	(DM)PetscToPointer((dm) ),*pStart,*pEnd);
}
void PETSC_STDCALL  dmplexgetconesize_(DM dm,PetscInt *p,PetscInt *size, int *__ierr ){
*__ierr = DMPlexGetConeSize(
	(DM)PetscToPointer((dm) ),*p,size);
}
void PETSC_STDCALL  dmplexsetconesize_(DM dm,PetscInt *p,PetscInt *size, int *__ierr ){
*__ierr = DMPlexSetConeSize(
	(DM)PetscToPointer((dm) ),*p,*size);
}
void PETSC_STDCALL  dmplexsetcone_(DM dm,PetscInt *p, PetscInt cone[], int *__ierr ){
*__ierr = DMPlexSetCone(
	(DM)PetscToPointer((dm) ),*p,cone);
}
void PETSC_STDCALL  dmplexsetconeorientation_(DM dm,PetscInt *p, PetscInt coneOrientation[], int *__ierr ){
*__ierr = DMPlexSetConeOrientation(
	(DM)PetscToPointer((dm) ),*p,coneOrientation);
}
void PETSC_STDCALL  dmplexgetsupportsize_(DM dm,PetscInt *p,PetscInt *size, int *__ierr ){
*__ierr = DMPlexGetSupportSize(
	(DM)PetscToPointer((dm) ),*p,size);
}
void PETSC_STDCALL  dmplexsetsupportsize_(DM dm,PetscInt *p,PetscInt *size, int *__ierr ){
*__ierr = DMPlexSetSupportSize(
	(DM)PetscToPointer((dm) ),*p,*size);
}
void PETSC_STDCALL  dmplexsetsupport_(DM dm,PetscInt *p, PetscInt support[], int *__ierr ){
*__ierr = DMPlexSetSupport(
	(DM)PetscToPointer((dm) ),*p,support);
}
void PETSC_STDCALL  dmplexgetmaxsizes_(DM dm,PetscInt *maxConeSize,PetscInt *maxSupportSize, int *__ierr ){
*__ierr = DMPlexGetMaxSizes(
	(DM)PetscToPointer((dm) ),maxConeSize,maxSupportSize);
}
void PETSC_STDCALL  dmplexsymmetrize_(DM dm, int *__ierr ){
*__ierr = DMPlexSymmetrize(
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  dmplexstratify_(DM dm, int *__ierr ){
*__ierr = DMPlexStratify(
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  dmplexgetdepth_(DM dm,PetscInt *depth, int *__ierr ){
*__ierr = DMPlexGetDepth(
	(DM)PetscToPointer((dm) ),depth);
}
void PETSC_STDCALL  dmplexgetdepthstratum_(DM dm,PetscInt *stratumValue,PetscInt *start,PetscInt *end, int *__ierr ){
*__ierr = DMPlexGetDepthStratum(
	(DM)PetscToPointer((dm) ),*stratumValue,start,end);
}
void PETSC_STDCALL  dmplexgetheightstratum_(DM dm,PetscInt *stratumValue,PetscInt *start,PetscInt *end, int *__ierr ){
*__ierr = DMPlexGetHeightStratum(
	(DM)PetscToPointer((dm) ),*stratumValue,start,end);
}
void PETSC_STDCALL  dmplexgetcoordinatesection_(DM dm,PetscSection *section, int *__ierr ){
*__ierr = DMPlexGetCoordinateSection(
	(DM)PetscToPointer((dm) ),section);
}
void PETSC_STDCALL  dmplexsetcoordinatesection_(DM dm,PetscSection section, int *__ierr ){
*__ierr = DMPlexSetCoordinateSection(
	(DM)PetscToPointer((dm) ),
	(PetscSection)PetscToPointer((section) ));
}
#if defined(__cplusplus)
}
#endif
