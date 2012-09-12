#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* index.c */
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
#define isidentity_ ISIDENTITY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isidentity_ isidentity
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define issetidentity_ ISSETIDENTITY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define issetidentity_ issetidentity
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define iscontiguouslocal_ ISCONTIGUOUSLOCAL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define iscontiguouslocal_ iscontiguouslocal
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define ispermutation_ ISPERMUTATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define ispermutation_ ispermutation
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define issetpermutation_ ISSETPERMUTATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define issetpermutation_ issetpermutation
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isdestroy_ ISDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isdestroy_ isdestroy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isinvertpermutation_ ISINVERTPERMUTATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isinvertpermutation_ isinvertpermutation
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isgetsize_ ISGETSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isgetsize_ isgetsize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isgetlocalsize_ ISGETLOCALSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isgetlocalsize_ isgetlocalsize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isgetnonlocalis_ ISGETNONLOCALIS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isgetnonlocalis_ isgetnonlocalis
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isrestorenonlocalis_ ISRESTORENONLOCALIS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isrestorenonlocalis_ isrestorenonlocalis
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define issort_ ISSORT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define issort_ issort
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define istogeneral_ ISTOGENERAL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define istogeneral_ istogeneral
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define issorted_ ISSORTED
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define issorted_ issorted
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isduplicate_ ISDUPLICATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isduplicate_ isduplicate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define iscopy_ ISCOPY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define iscopy_ iscopy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isoncomm_ ISONCOMM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isoncomm_ isoncomm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define issetblocksize_ ISSETBLOCKSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define issetblocksize_ issetblocksize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define isgetblocksize_ ISGETBLOCKSIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define isgetblocksize_ isgetblocksize
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  isidentity_(IS is,PetscBool  *ident, int *__ierr ){
*__ierr = ISIdentity(
	(IS)PetscToPointer((is) ),ident);
}
void PETSC_STDCALL  issetidentity_(IS is, int *__ierr ){
*__ierr = ISSetIdentity(
	(IS)PetscToPointer((is) ));
}
void PETSC_STDCALL  iscontiguouslocal_(IS is,PetscInt *gstart,PetscInt *gend,PetscInt *start,PetscBool *contig, int *__ierr ){
*__ierr = ISContiguousLocal(
	(IS)PetscToPointer((is) ),*gstart,*gend,start,contig);
}
void PETSC_STDCALL  ispermutation_(IS is,PetscBool  *perm, int *__ierr ){
*__ierr = ISPermutation(
	(IS)PetscToPointer((is) ),perm);
}
void PETSC_STDCALL  issetpermutation_(IS is, int *__ierr ){
*__ierr = ISSetPermutation(
	(IS)PetscToPointer((is) ));
}
void PETSC_STDCALL  isdestroy_(IS *is, int *__ierr ){
*__ierr = ISDestroy(is);
}
void PETSC_STDCALL  isinvertpermutation_(IS is,PetscInt *nlocal,IS *isout, int *__ierr ){
*__ierr = ISInvertPermutation(
	(IS)PetscToPointer((is) ),*nlocal,isout);
}
void PETSC_STDCALL  isgetsize_(IS is,PetscInt *size, int *__ierr ){
*__ierr = ISGetSize(
	(IS)PetscToPointer((is) ),size);
}
void PETSC_STDCALL  isgetlocalsize_(IS is,PetscInt *size, int *__ierr ){
*__ierr = ISGetLocalSize(
	(IS)PetscToPointer((is) ),size);
}
void PETSC_STDCALL  isgetnonlocalis_(IS is,IS *complement, int *__ierr ){
*__ierr = ISGetNonlocalIS(
	(IS)PetscToPointer((is) ),complement);
}
void PETSC_STDCALL  isrestorenonlocalis_(IS is,IS *complement, int *__ierr ){
*__ierr = ISRestoreNonlocalIS(
	(IS)PetscToPointer((is) ),complement);
}
void PETSC_STDCALL  issort_(IS is, int *__ierr ){
*__ierr = ISSort(
	(IS)PetscToPointer((is) ));
}
void PETSC_STDCALL  istogeneral_(IS is, int *__ierr ){
*__ierr = ISToGeneral(
	(IS)PetscToPointer((is) ));
}
void PETSC_STDCALL  issorted_(IS is,PetscBool  *flg, int *__ierr ){
*__ierr = ISSorted(
	(IS)PetscToPointer((is) ),flg);
}
void PETSC_STDCALL  isduplicate_(IS is,IS *newIS, int *__ierr ){
*__ierr = ISDuplicate(
	(IS)PetscToPointer((is) ),newIS);
}
void PETSC_STDCALL  iscopy_(IS is,IS isy, int *__ierr ){
*__ierr = ISCopy(
	(IS)PetscToPointer((is) ),
	(IS)PetscToPointer((isy) ));
}
void PETSC_STDCALL  isoncomm_(IS is,MPI_Fint * comm,PetscCopyMode *mode,IS *newis, int *__ierr ){
*__ierr = ISOnComm(
	(IS)PetscToPointer((is) ),
	MPI_Comm_f2c( *(comm) ),*mode,newis);
}
void PETSC_STDCALL  issetblocksize_(IS is,PetscInt *bs, int *__ierr ){
*__ierr = ISSetBlockSize(
	(IS)PetscToPointer((is) ),*bs);
}
void PETSC_STDCALL  isgetblocksize_(IS is,PetscInt *size, int *__ierr ){
*__ierr = ISGetBlockSize(
	(IS)PetscToPointer((is) ),size);
}
#if defined(__cplusplus)
}
#endif
