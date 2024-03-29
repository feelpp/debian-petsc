#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* dline.c */
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

#include "petscdraw.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawgetboundingbox_ PETSCDRAWGETBOUNDINGBOX
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawgetboundingbox_ petscdrawgetboundingbox
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawgetcurrentpoint_ PETSCDRAWGETCURRENTPOINT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawgetcurrentpoint_ petscdrawgetcurrentpoint
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawsetcurrentpoint_ PETSCDRAWSETCURRENTPOINT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawsetcurrentpoint_ petscdrawsetcurrentpoint
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawpushcurrentpoint_ PETSCDRAWPUSHCURRENTPOINT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawpushcurrentpoint_ petscdrawpushcurrentpoint
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawpopcurrentpoint_ PETSCDRAWPOPCURRENTPOINT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawpopcurrentpoint_ petscdrawpopcurrentpoint
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawline_ PETSCDRAWLINE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawline_ petscdrawline
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscdrawarrow_ PETSCDRAWARROW
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscdrawarrow_ petscdrawarrow
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscdrawgetboundingbox_(PetscDraw draw,PetscReal *xl,PetscReal *yl,PetscReal *xr,PetscReal *yr, int *__ierr ){
*__ierr = PetscDrawGetBoundingBox(
	(PetscDraw)PetscToPointer((draw) ),xl,yl,xr,yr);
}
void PETSC_STDCALL  petscdrawgetcurrentpoint_(PetscDraw draw,PetscReal *x,PetscReal *y, int *__ierr ){
*__ierr = PetscDrawGetCurrentPoint(
	(PetscDraw)PetscToPointer((draw) ),x,y);
}
void PETSC_STDCALL  petscdrawsetcurrentpoint_(PetscDraw draw,PetscReal *x,PetscReal *y, int *__ierr ){
*__ierr = PetscDrawSetCurrentPoint(
	(PetscDraw)PetscToPointer((draw) ),*x,*y);
}
void PETSC_STDCALL  petscdrawpushcurrentpoint_(PetscDraw draw,PetscReal *x,PetscReal *y, int *__ierr ){
*__ierr = PetscDrawPushCurrentPoint(
	(PetscDraw)PetscToPointer((draw) ),*x,*y);
}
void PETSC_STDCALL  petscdrawpopcurrentpoint_(PetscDraw draw, int *__ierr ){
*__ierr = PetscDrawPopCurrentPoint(
	(PetscDraw)PetscToPointer((draw) ));
}
void PETSC_STDCALL  petscdrawline_(PetscDraw draw,PetscReal *xl,PetscReal *yl,PetscReal *xr,PetscReal *yr,int *cl, int *__ierr ){
*__ierr = PetscDrawLine(
	(PetscDraw)PetscToPointer((draw) ),*xl,*yl,*xr,*yr,*cl);
}
void PETSC_STDCALL  petscdrawarrow_(PetscDraw draw,PetscReal *xl,PetscReal *yl,PetscReal *xr,PetscReal *yr,int *cl, int *__ierr ){
*__ierr = PetscDrawArrow(
	(PetscDraw)PetscToPointer((draw) ),*xl,*yl,*xr,*yr,*cl);
}
#if defined(__cplusplus)
}
#endif
