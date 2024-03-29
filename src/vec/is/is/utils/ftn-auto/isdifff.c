#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* isdiff.c */
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

#include "petscis.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isdifference_ ISDIFFERENCE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isdifference_ isdifference
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define issum_ ISSUM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define issum_ issum
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isexpand_ ISEXPAND
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isexpand_ isexpand
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isconcatenate_ ISCONCATENATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isconcatenate_ isconcatenate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define islisttopair_ ISLISTTOPAIR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define islisttopair_ islisttopair
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define ispairtolist_ ISPAIRTOLIST
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define ispairtolist_ ispairtolist
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isembed_ ISEMBED
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isembed_ isembed
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  isdifference_(IS is1,IS is2,IS *isout, int *__ierr ){
*__ierr = ISDifference(
	(IS)PetscToPointer((is1) ),
	(IS)PetscToPointer((is2) ),isout);
}
void PETSC_STDCALL  issum_(IS is1,IS is2,IS *is3, int *__ierr ){
*__ierr = ISSum(
	(IS)PetscToPointer((is1) ),
	(IS)PetscToPointer((is2) ),is3);
}
void PETSC_STDCALL  isexpand_(IS is1,IS is2,IS *isout, int *__ierr ){
*__ierr = ISExpand(
	(IS)PetscToPointer((is1) ),
	(IS)PetscToPointer((is2) ),isout);
}
void PETSC_STDCALL  isconcatenate_(MPI_Fint * comm,PetscInt *len, IS islist[],IS *isout, int *__ierr ){
*__ierr = ISConcatenate(
	MPI_Comm_f2c( *(comm) ),*len,islist,isout);
}
#undef __FUNCT__
#define __FUNCT__ ""
void PETSC_STDCALL  islisttopair_(MPI_Fint * comm,PetscInt *listlen,IS islist[],IS *xis,IS *yis, int *__ierr ){
*__ierr = ISListToPair(
	MPI_Comm_f2c( *(comm) ),*listlen,islist,xis,yis);
}
#undef __FUNCT__
#define __FUNCT__ ""
void PETSC_STDCALL  ispairtolist_(IS xis,IS yis,PetscInt *listlen,IS **islist, int *__ierr ){
*__ierr = ISPairToList(
	(IS)PetscToPointer((xis) ),
	(IS)PetscToPointer((yis) ),listlen,islist);
}
#undef __FUNCT__
#define __FUNCT__ ""
void PETSC_STDCALL  isembed_(IS a,IS b,PetscBool *drop,IS *c, int *__ierr ){
*__ierr = ISEmbed(
	(IS)PetscToPointer((a) ),
	(IS)PetscToPointer((b) ),*drop,c);
}
#if defined(__cplusplus)
}
#endif
