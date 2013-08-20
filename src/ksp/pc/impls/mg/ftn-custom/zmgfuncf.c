#include <petsc-private/fortranimpl.h>
#include <petscpc.h>
#include <../src/ksp/pc/impls/mg/mgimpl.h>

#if defined(PETSC_HAVE_FORTRAN_CAPS)
#define pcmgsetresidual_           PCMGSETRESIDUAL
#define pcmgresidual_default_       PCMGRESIDUAL_DEFAULT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE)
#define pcmgsetresidual_           pcmgsetresidual
#define pcmgresidual_default_       pcmgresidual_default
#endif

#if defined(PETSC_HAVE_FORTRAN_UNDERSCORE_UNDERSCORE)
#  defined pcmgresidual_default_       pcmgresidual_default__
#endif

typedef PetscErrorCode (*MVVVV)(Mat,Vec,Vec,Vec);
static PetscErrorCode ourresidualfunction(Mat mat,Vec b,Vec x,Vec R)
{
  PetscErrorCode ierr = 0;
  (*(void (PETSC_STDCALL *)(Mat*,Vec*,Vec*,Vec*,PetscErrorCode*))(((PetscObject)mat)->fortran_func_pointers[0]))(&mat,&b,&x,&R,&ierr);
  return 0;
}

PETSC_EXTERN void pcmgresidual_default_(Mat *mat,Vec *b,Vec *x,Vec *r, PetscErrorCode *ierr)
{
  *ierr = PCMGResidual_Default(*mat,*b,*x,*r);
}

PETSC_EXTERN void PETSC_STDCALL pcmgsetresidual_(PC *pc,PetscInt *l,PetscErrorCode (*residual)(Mat*,Vec*,Vec*,Vec*,PetscErrorCode*),Mat *mat, PetscErrorCode *ierr)
{
  MVVVV rr;
  if ((PetscVoidFunction)residual == (PetscVoidFunction)pcmgresidual_default_) rr = PCMGResidual_Default;
  else {
    PetscObjectAllocateFortranPointers(*mat,1);
    /*  Attach the residual computer to the Mat, this is not ideal but the only object/context passed in the residual computer */
    ((PetscObject)*mat)->fortran_func_pointers[0] = (PetscVoidFunction)residual;

    rr = ourresidualfunction;
  }
  *ierr = PCMGSetResidual(*pc,*l,rr,*mat);
}

