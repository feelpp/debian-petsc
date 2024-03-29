#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* dmshell.c */
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

#include "petscdmshell.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmglobaltolocalbegindefaultshell_ DMGLOBALTOLOCALBEGINDEFAULTSHELL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmglobaltolocalbegindefaultshell_ dmglobaltolocalbegindefaultshell
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmglobaltolocalenddefaultshell_ DMGLOBALTOLOCALENDDEFAULTSHELL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmglobaltolocalenddefaultshell_ dmglobaltolocalenddefaultshell
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmlocaltoglobalbegindefaultshell_ DMLOCALTOGLOBALBEGINDEFAULTSHELL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmlocaltoglobalbegindefaultshell_ dmlocaltoglobalbegindefaultshell
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmlocaltoglobalenddefaultshell_ DMLOCALTOGLOBALENDDEFAULTSHELL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmlocaltoglobalenddefaultshell_ dmlocaltoglobalenddefaultshell
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmshellsetmatrix_ DMSHELLSETMATRIX
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmshellsetmatrix_ dmshellsetmatrix
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmshellsetglobalvector_ DMSHELLSETGLOBALVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmshellsetglobalvector_ dmshellsetglobalvector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmshellsetlocalvector_ DMSHELLSETLOCALVECTOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmshellsetlocalvector_ dmshellsetlocalvector
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmshellsetglobaltolocalvecscatter_ DMSHELLSETGLOBALTOLOCALVECSCATTER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmshellsetglobaltolocalvecscatter_ dmshellsetglobaltolocalvecscatter
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmshellsetlocaltoglobalvecscatter_ DMSHELLSETLOCALTOGLOBALVECSCATTER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmshellsetlocaltoglobalvecscatter_ dmshellsetlocaltoglobalvecscatter
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define dmshellcreate_ DMSHELLCREATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define dmshellcreate_ dmshellcreate
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  dmglobaltolocalbegindefaultshell_(DM dm,Vec g,InsertMode *mode,Vec l, int *__ierr ){
*__ierr = DMGlobalToLocalBeginDefaultShell(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((g) ),*mode,
	(Vec)PetscToPointer((l) ));
}
void PETSC_STDCALL  dmglobaltolocalenddefaultshell_(DM dm,Vec g,InsertMode *mode,Vec l, int *__ierr ){
*__ierr = DMGlobalToLocalEndDefaultShell(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((g) ),*mode,
	(Vec)PetscToPointer((l) ));
}
void PETSC_STDCALL  dmlocaltoglobalbegindefaultshell_(DM dm,Vec l,InsertMode *mode,Vec g, int *__ierr ){
*__ierr = DMLocalToGlobalBeginDefaultShell(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((l) ),*mode,
	(Vec)PetscToPointer((g) ));
}
void PETSC_STDCALL  dmlocaltoglobalenddefaultshell_(DM dm,Vec l,InsertMode *mode,Vec g, int *__ierr ){
*__ierr = DMLocalToGlobalEndDefaultShell(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((l) ),*mode,
	(Vec)PetscToPointer((g) ));
}
void PETSC_STDCALL  dmshellsetmatrix_(DM dm,Mat J, int *__ierr ){
*__ierr = DMShellSetMatrix(
	(DM)PetscToPointer((dm) ),
	(Mat)PetscToPointer((J) ));
}
void PETSC_STDCALL  dmshellsetglobalvector_(DM dm,Vec X, int *__ierr ){
*__ierr = DMShellSetGlobalVector(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((X) ));
}
void PETSC_STDCALL  dmshellsetlocalvector_(DM dm,Vec X, int *__ierr ){
*__ierr = DMShellSetLocalVector(
	(DM)PetscToPointer((dm) ),
	(Vec)PetscToPointer((X) ));
}
void PETSC_STDCALL  dmshellsetglobaltolocalvecscatter_(DM dm,VecScatter gtol, int *__ierr ){
*__ierr = DMShellSetGlobalToLocalVecScatter(
	(DM)PetscToPointer((dm) ),
	(VecScatter)PetscToPointer((gtol) ));
}
void PETSC_STDCALL  dmshellsetlocaltoglobalvecscatter_(DM dm,VecScatter ltog, int *__ierr ){
*__ierr = DMShellSetLocalToGlobalVecScatter(
	(DM)PetscToPointer((dm) ),
	(VecScatter)PetscToPointer((ltog) ));
}
void PETSC_STDCALL  dmshellcreate_(MPI_Fint * comm,DM *dm, int *__ierr ){
*__ierr = DMShellCreate(
	MPI_Comm_f2c( *(comm) ),dm);
}
#if defined(__cplusplus)
}
#endif
