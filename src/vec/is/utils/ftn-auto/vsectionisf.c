#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* vsectionis.c */
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

#include "petscvec.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectioncreate_ PETSCSECTIONCREATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectioncreate_ petscsectioncreate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionclone_ PETSCSECTIONCLONE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionclone_ petscsectionclone
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetnumfields_ PETSCSECTIONGETNUMFIELDS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetnumfields_ petscsectiongetnumfields
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetnumfields_ PETSCSECTIONSETNUMFIELDS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetnumfields_ petscsectionsetnumfields
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetfieldcomponents_ PETSCSECTIONGETFIELDCOMPONENTS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetfieldcomponents_ petscsectiongetfieldcomponents
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetfieldcomponents_ PETSCSECTIONSETFIELDCOMPONENTS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetfieldcomponents_ petscsectionsetfieldcomponents
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetchart_ PETSCSECTIONGETCHART
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetchart_ petscsectiongetchart
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetchart_ PETSCSECTIONSETCHART
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetchart_ petscsectionsetchart
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetdof_ PETSCSECTIONGETDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetdof_ petscsectiongetdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetdof_ PETSCSECTIONSETDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetdof_ petscsectionsetdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionadddof_ PETSCSECTIONADDDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionadddof_ petscsectionadddof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetfielddof_ PETSCSECTIONGETFIELDDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetfielddof_ petscsectiongetfielddof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetfielddof_ PETSCSECTIONSETFIELDDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetfielddof_ petscsectionsetfielddof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionaddfielddof_ PETSCSECTIONADDFIELDDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionaddfielddof_ petscsectionaddfielddof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetconstraintdof_ PETSCSECTIONGETCONSTRAINTDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetconstraintdof_ petscsectiongetconstraintdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetconstraintdof_ PETSCSECTIONSETCONSTRAINTDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetconstraintdof_ petscsectionsetconstraintdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionaddconstraintdof_ PETSCSECTIONADDCONSTRAINTDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionaddconstraintdof_ petscsectionaddconstraintdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetfieldconstraintdof_ PETSCSECTIONGETFIELDCONSTRAINTDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetfieldconstraintdof_ petscsectiongetfieldconstraintdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetfieldconstraintdof_ PETSCSECTIONSETFIELDCONSTRAINTDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetfieldconstraintdof_ petscsectionsetfieldconstraintdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionaddfieldconstraintdof_ PETSCSECTIONADDFIELDCONSTRAINTDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionaddfieldconstraintdof_ petscsectionaddfieldconstraintdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetup_ PETSCSECTIONSETUP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetup_ petscsectionsetup
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetmaxdof_ PETSCSECTIONGETMAXDOF
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetmaxdof_ petscsectiongetmaxdof
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetstoragesize_ PETSCSECTIONGETSTORAGESIZE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetstoragesize_ petscsectiongetstoragesize
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectioncreateglobalsection_ PETSCSECTIONCREATEGLOBALSECTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectioncreateglobalsection_ petscsectioncreateglobalsection
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectioncreateglobalsectioncensored_ PETSCSECTIONCREATEGLOBALSECTIONCENSORED
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectioncreateglobalsectioncensored_ petscsectioncreateglobalsectioncensored
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetoffset_ PETSCSECTIONGETOFFSET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetoffset_ petscsectiongetoffset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetoffset_ PETSCSECTIONSETOFFSET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetoffset_ petscsectionsetoffset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetfieldoffset_ PETSCSECTIONGETFIELDOFFSET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetfieldoffset_ petscsectiongetfieldoffset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionsetfieldoffset_ PETSCSECTIONSETFIELDOFFSET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionsetfieldoffset_ petscsectionsetfieldoffset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiongetoffsetrange_ PETSCSECTIONGETOFFSETRANGE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiongetoffsetrange_ petscsectiongetoffsetrange
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectionreset_ PETSCSECTIONRESET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectionreset_ petscsectionreset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define petscsectiondestroy_ PETSCSECTIONDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define petscsectiondestroy_ petscsectiondestroy
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  petscsectioncreate_(MPI_Fint * comm,PetscSection *s, int *__ierr ){
*__ierr = PetscSectionCreate(
	MPI_Comm_f2c( *(comm) ),s);
}
void PETSC_STDCALL  petscsectionclone_(PetscSection section,PetscSection *newSection, int *__ierr ){
*__ierr = PetscSectionClone(
	(PetscSection)PetscToPointer((section) ),newSection);
}
void PETSC_STDCALL  petscsectiongetnumfields_(PetscSection s,PetscInt *numFields, int *__ierr ){
*__ierr = PetscSectionGetNumFields(
	(PetscSection)PetscToPointer((s) ),numFields);
}
void PETSC_STDCALL  petscsectionsetnumfields_(PetscSection s,PetscInt *numFields, int *__ierr ){
*__ierr = PetscSectionSetNumFields(
	(PetscSection)PetscToPointer((s) ),*numFields);
}
void PETSC_STDCALL  petscsectiongetfieldcomponents_(PetscSection s,PetscInt *field,PetscInt *numComp, int *__ierr ){
*__ierr = PetscSectionGetFieldComponents(
	(PetscSection)PetscToPointer((s) ),*field,numComp);
}
void PETSC_STDCALL  petscsectionsetfieldcomponents_(PetscSection s,PetscInt *field,PetscInt *numComp, int *__ierr ){
*__ierr = PetscSectionSetFieldComponents(
	(PetscSection)PetscToPointer((s) ),*field,*numComp);
}
void PETSC_STDCALL  petscsectiongetchart_(PetscSection s,PetscInt *pStart,PetscInt *pEnd, int *__ierr ){
*__ierr = PetscSectionGetChart(
	(PetscSection)PetscToPointer((s) ),pStart,pEnd);
}
void PETSC_STDCALL  petscsectionsetchart_(PetscSection s,PetscInt *pStart,PetscInt *pEnd, int *__ierr ){
*__ierr = PetscSectionSetChart(
	(PetscSection)PetscToPointer((s) ),*pStart,*pEnd);
}
void PETSC_STDCALL  petscsectiongetdof_(PetscSection s,PetscInt *point,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionGetDof(
	(PetscSection)PetscToPointer((s) ),*point,numDof);
}
void PETSC_STDCALL  petscsectionsetdof_(PetscSection s,PetscInt *point,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionSetDof(
	(PetscSection)PetscToPointer((s) ),*point,*numDof);
}
void PETSC_STDCALL  petscsectionadddof_(PetscSection s,PetscInt *point,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionAddDof(
	(PetscSection)PetscToPointer((s) ),*point,*numDof);
}
void PETSC_STDCALL  petscsectiongetfielddof_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionGetFieldDof(
	(PetscSection)PetscToPointer((s) ),*point,*field,numDof);
}
void PETSC_STDCALL  petscsectionsetfielddof_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionSetFieldDof(
	(PetscSection)PetscToPointer((s) ),*point,*field,*numDof);
}
void PETSC_STDCALL  petscsectionaddfielddof_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionAddFieldDof(
	(PetscSection)PetscToPointer((s) ),*point,*field,*numDof);
}
void PETSC_STDCALL  petscsectiongetconstraintdof_(PetscSection s,PetscInt *point,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionGetConstraintDof(
	(PetscSection)PetscToPointer((s) ),*point,numDof);
}
void PETSC_STDCALL  petscsectionsetconstraintdof_(PetscSection s,PetscInt *point,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionSetConstraintDof(
	(PetscSection)PetscToPointer((s) ),*point,*numDof);
}
void PETSC_STDCALL  petscsectionaddconstraintdof_(PetscSection s,PetscInt *point,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionAddConstraintDof(
	(PetscSection)PetscToPointer((s) ),*point,*numDof);
}
void PETSC_STDCALL  petscsectiongetfieldconstraintdof_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionGetFieldConstraintDof(
	(PetscSection)PetscToPointer((s) ),*point,*field,numDof);
}
void PETSC_STDCALL  petscsectionsetfieldconstraintdof_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionSetFieldConstraintDof(
	(PetscSection)PetscToPointer((s) ),*point,*field,*numDof);
}
void PETSC_STDCALL  petscsectionaddfieldconstraintdof_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *numDof, int *__ierr ){
*__ierr = PetscSectionAddFieldConstraintDof(
	(PetscSection)PetscToPointer((s) ),*point,*field,*numDof);
}
void PETSC_STDCALL  petscsectionsetup_(PetscSection s, int *__ierr ){
*__ierr = PetscSectionSetUp(
	(PetscSection)PetscToPointer((s) ));
}
void PETSC_STDCALL  petscsectiongetmaxdof_(PetscSection s,PetscInt *maxDof, int *__ierr ){
*__ierr = PetscSectionGetMaxDof(
	(PetscSection)PetscToPointer((s) ),maxDof);
}
void PETSC_STDCALL  petscsectiongetstoragesize_(PetscSection s,PetscInt *size, int *__ierr ){
*__ierr = PetscSectionGetStorageSize(
	(PetscSection)PetscToPointer((s) ),size);
}
void PETSC_STDCALL  petscsectioncreateglobalsection_(PetscSection s,PetscSF *sf,PetscBool *includeConstraints,PetscSection *gsection, int *__ierr ){
*__ierr = PetscSectionCreateGlobalSection(
	(PetscSection)PetscToPointer((s) ),*sf,*includeConstraints,gsection);
}
void PETSC_STDCALL  petscsectioncreateglobalsectioncensored_(PetscSection s,PetscSF *sf,PetscBool *includeConstraints,PetscInt *numExcludes, PetscInt excludes[],PetscSection *gsection, int *__ierr ){
*__ierr = PetscSectionCreateGlobalSectionCensored(
	(PetscSection)PetscToPointer((s) ),*sf,*includeConstraints,*numExcludes,excludes,gsection);
}
void PETSC_STDCALL  petscsectiongetoffset_(PetscSection s,PetscInt *point,PetscInt *offset, int *__ierr ){
*__ierr = PetscSectionGetOffset(
	(PetscSection)PetscToPointer((s) ),*point,offset);
}
void PETSC_STDCALL  petscsectionsetoffset_(PetscSection s,PetscInt *point,PetscInt *offset, int *__ierr ){
*__ierr = PetscSectionSetOffset(
	(PetscSection)PetscToPointer((s) ),*point,*offset);
}
void PETSC_STDCALL  petscsectiongetfieldoffset_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *offset, int *__ierr ){
*__ierr = PetscSectionGetFieldOffset(
	(PetscSection)PetscToPointer((s) ),*point,*field,offset);
}
void PETSC_STDCALL  petscsectionsetfieldoffset_(PetscSection s,PetscInt *point,PetscInt *field,PetscInt *offset, int *__ierr ){
*__ierr = PetscSectionSetFieldOffset(
	(PetscSection)PetscToPointer((s) ),*point,*field,*offset);
}
void PETSC_STDCALL  petscsectiongetoffsetrange_(PetscSection s,PetscInt *start,PetscInt *end, int *__ierr ){
*__ierr = PetscSectionGetOffsetRange(
	(PetscSection)PetscToPointer((s) ),start,end);
}
void PETSC_STDCALL  petscsectionreset_(PetscSection s, int *__ierr ){
*__ierr = PetscSectionReset(
	(PetscSection)PetscToPointer((s) ));
}
void PETSC_STDCALL  petscsectiondestroy_(PetscSection *s, int *__ierr ){
*__ierr = PetscSectionDestroy(s);
}
#if defined(__cplusplus)
}
#endif
