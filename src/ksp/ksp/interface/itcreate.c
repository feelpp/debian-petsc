
/*
     The basic KSP routines, Create, View etc. are here.
*/
#include <petsc-private/kspimpl.h>      /*I "petscksp.h" I*/

/* Logging support */
PetscClassId  KSP_CLASSID;
PetscClassId  DMKSP_CLASSID;
PetscLogEvent KSP_GMRESOrthogonalization, KSP_SetUp, KSP_Solve;

/*
   Contains the list of registered KSP routines
*/
PetscFunctionList KSPList              = 0;
PetscBool         KSPRegisterAllCalled = PETSC_FALSE;

#undef __FUNCT__
#define __FUNCT__ "KSPLoad"
/*@C
  KSPLoad - Loads a KSP that has been stored in binary  with KSPView().

  Collective on PetscViewer

  Input Parameters:
+ newdm - the newly loaded KSP, this needs to have been created with KSPCreate() or
           some related function before a call to KSPLoad().
- viewer - binary file viewer, obtained from PetscViewerBinaryOpen()

   Level: intermediate

  Notes:
   The type is determined by the data in the file, any type set into the KSP before this call is ignored.

  Notes for advanced users:
  Most users should not need to know the details of the binary storage
  format, since KSPLoad() and KSPView() completely hide these details.
  But for anyone who's interested, the standard binary matrix storage
  format is
.vb
     has not yet been determined
.ve

.seealso: PetscViewerBinaryOpen(), KSPView(), MatLoad(), VecLoad()
@*/
PetscErrorCode  KSPLoad(KSP newdm, PetscViewer viewer)
{
  PetscErrorCode ierr;
  PetscBool      isbinary;
  PetscInt       classid;
  char           type[256];
  PC             pc;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(newdm,KSP_CLASSID,1);
  PetscValidHeaderSpecific(viewer,PETSC_VIEWER_CLASSID,2);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERBINARY,&isbinary);CHKERRQ(ierr);
  if (!isbinary) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONG,"Invalid viewer; open viewer with PetscViewerBinaryOpen()");

  ierr = PetscViewerBinaryRead(viewer,&classid,1,PETSC_INT);CHKERRQ(ierr);
  if (classid != KSP_FILE_CLASSID) SETERRQ(PetscObjectComm((PetscObject)newdm),PETSC_ERR_ARG_WRONG,"Not KSP next in file");
  ierr = PetscViewerBinaryRead(viewer,type,256,PETSC_CHAR);CHKERRQ(ierr);
  ierr = KSPSetType(newdm, type);CHKERRQ(ierr);
  if (newdm->ops->load) {
    ierr = (*newdm->ops->load)(newdm,viewer);CHKERRQ(ierr);
  }
  ierr = KSPGetPC(newdm,&pc);CHKERRQ(ierr);
  ierr = PCLoad(pc,viewer);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#include <petscdraw.h>
#if defined(PETSC_HAVE_AMS)
#include <petscviewerams.h>
#endif
#undef __FUNCT__
#define __FUNCT__ "KSPView"
/*@C
   KSPView - Prints the KSP data structure.

   Collective on KSP

   Input Parameters:
+  ksp - the Krylov space context
-  viewer - visualization context

   Options Database Keys:
.  -ksp_view - print the ksp data structure at the end of a KSPSolve call

   Note:
   The available visualization contexts include
+     PETSC_VIEWER_STDOUT_SELF - standard output (default)
-     PETSC_VIEWER_STDOUT_WORLD - synchronized standard
         output where only the first processor opens
         the file.  All other processors send their
         data to the first processor to print.

   The user can open an alternative visualization context with
   PetscViewerASCIIOpen() - output to a specified file.

   Level: beginner

.keywords: KSP, view

.seealso: PCView(), PetscViewerASCIIOpen()
@*/
PetscErrorCode  KSPView(KSP ksp,PetscViewer viewer)
{
  PetscErrorCode ierr;
  PetscBool      iascii,isbinary,isdraw;
#if defined(PETSC_HAVE_AMS)
  PetscBool      isams;
#endif

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  if (!viewer) viewer = PETSC_VIEWER_STDOUT_(PetscObjectComm((PetscObject)ksp));
  PetscValidHeaderSpecific(viewer,PETSC_VIEWER_CLASSID,2);
  PetscCheckSameComm(ksp,1,viewer,2);

  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERASCII,&iascii);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERBINARY,&isbinary);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERDRAW,&isdraw);CHKERRQ(ierr);
#if defined(PETSC_HAVE_AMS)
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERAMS,&isams);CHKERRQ(ierr);
#endif
  if (iascii) {
    ierr = PetscObjectPrintClassNamePrefixType((PetscObject)ksp,viewer,"KSP Object");CHKERRQ(ierr);
    if (ksp->ops->view) {
      ierr = PetscViewerASCIIPushTab(viewer);CHKERRQ(ierr);
      ierr = (*ksp->ops->view)(ksp,viewer);CHKERRQ(ierr);
      ierr = PetscViewerASCIIPopTab(viewer);CHKERRQ(ierr);
    }
    if (ksp->guess_zero) {
      ierr = PetscViewerASCIIPrintf(viewer,"  maximum iterations=%D, initial guess is zero\n",ksp->max_it);CHKERRQ(ierr);
    } else {
      ierr = PetscViewerASCIIPrintf(viewer,"  maximum iterations=%D\n", ksp->max_it);CHKERRQ(ierr);
    }
    if (ksp->guess_knoll) {ierr = PetscViewerASCIIPrintf(viewer,"  using preconditioner applied to right hand side for initial guess\n");CHKERRQ(ierr);}
    ierr = PetscViewerASCIIPrintf(viewer,"  tolerances:  relative=%G, absolute=%G, divergence=%G\n",ksp->rtol,ksp->abstol,ksp->divtol);CHKERRQ(ierr);
    if (ksp->pc_side == PC_RIGHT) {
      ierr = PetscViewerASCIIPrintf(viewer,"  right preconditioning\n");CHKERRQ(ierr);
    } else if (ksp->pc_side == PC_SYMMETRIC) {
      ierr = PetscViewerASCIIPrintf(viewer,"  symmetric preconditioning\n");CHKERRQ(ierr);
    } else {
      ierr = PetscViewerASCIIPrintf(viewer,"  left preconditioning\n");CHKERRQ(ierr);
    }
    if (ksp->guess) {ierr = PetscViewerASCIIPrintf(viewer,"  using Fischers initial guess method %D with size %D\n",ksp->guess->method,ksp->guess->maxl);CHKERRQ(ierr);}
    if (ksp->dscale) {ierr = PetscViewerASCIIPrintf(viewer,"  diagonally scaled system\n");CHKERRQ(ierr);}
    if (ksp->nullsp) {ierr = PetscViewerASCIIPrintf(viewer,"  has attached null space\n");CHKERRQ(ierr);}
    if (!ksp->guess_zero) {ierr = PetscViewerASCIIPrintf(viewer,"  using nonzero initial guess\n");CHKERRQ(ierr);}
    ierr = PetscViewerASCIIPrintf(viewer,"  using %s norm type for convergence test\n",KSPNormTypes[ksp->normtype]);CHKERRQ(ierr);
  } else if (isbinary) {
    PetscInt    classid = KSP_FILE_CLASSID;
    MPI_Comm    comm;
    PetscMPIInt rank;
    char        type[256];

    ierr = PetscObjectGetComm((PetscObject)ksp,&comm);CHKERRQ(ierr);
    ierr = MPI_Comm_rank(comm,&rank);CHKERRQ(ierr);
    if (!rank) {
      ierr = PetscViewerBinaryWrite(viewer,&classid,1,PETSC_INT,PETSC_FALSE);CHKERRQ(ierr);
      ierr = PetscStrncpy(type,((PetscObject)ksp)->type_name,256);CHKERRQ(ierr);
      ierr = PetscViewerBinaryWrite(viewer,type,256,PETSC_CHAR,PETSC_FALSE);CHKERRQ(ierr);
    }
    if (ksp->ops->view) {
      ierr = (*ksp->ops->view)(ksp,viewer);CHKERRQ(ierr);
    }
  } else if (isdraw) {
    PetscDraw draw;
    char      str[36];
    PetscReal x,y,bottom,h;
    PetscBool flg;

    ierr = PetscViewerDrawGetDraw(viewer,0,&draw);CHKERRQ(ierr);
    ierr = PetscDrawGetCurrentPoint(draw,&x,&y);CHKERRQ(ierr);
    ierr = PetscObjectTypeCompare((PetscObject)ksp,KSPPREONLY,&flg);CHKERRQ(ierr);
    if (!flg) {
      ierr   = PetscStrcpy(str,"KSP: ");CHKERRQ(ierr);
      ierr   = PetscStrcat(str,((PetscObject)ksp)->type_name);CHKERRQ(ierr);
      ierr   = PetscDrawBoxedString(draw,x,y,PETSC_DRAW_RED,PETSC_DRAW_BLACK,str,NULL,&h);CHKERRQ(ierr);
      bottom = y - h;
    } else {
      bottom = y;
    }
    ierr = PetscDrawPushCurrentPoint(draw,x,bottom);CHKERRQ(ierr);
#if defined(PETSC_HAVE_AMS)
  } else if (isams) {
    if (((PetscObject)ksp)->amsmem == -1) {
      ierr = PetscObjectViewAMS((PetscObject)ksp,viewer);CHKERRQ(ierr);
      PetscStackCallAMS(AMS_Memory_take_access,(((PetscObject)ksp)->amsmem));
      PetscStackCallAMS(AMS_Memory_add_field,(((PetscObject)ksp)->amsmem,"its",&ksp->its,1,AMS_INT,AMS_READ,AMS_COMMON,AMS_REDUCT_UNDEF));
      if (!ksp->res_hist) {
        ierr = KSPSetResidualHistory(ksp,NULL,PETSC_DECIDE,PETSC_FALSE);CHKERRQ(ierr);
      }
      PetscStackCallAMS(AMS_Memory_add_field,(((PetscObject)ksp)->amsmem,"res_hist",ksp->res_hist,10,AMS_DOUBLE,AMS_READ,AMS_COMMON,AMS_REDUCT_UNDEF));
      PetscStackCallAMS(AMS_Memory_grant_access,(((PetscObject)ksp)->amsmem));
    }
#endif
  } else if (ksp->ops->view) {
    ierr = (*ksp->ops->view)(ksp,viewer);CHKERRQ(ierr);
  }
  if (!ksp->pc) {ierr = KSPGetPC(ksp,&ksp->pc);CHKERRQ(ierr);}
  ierr = PCView(ksp->pc,viewer);CHKERRQ(ierr);
  if (isdraw) {
    PetscDraw draw;
    ierr = PetscViewerDrawGetDraw(viewer,0,&draw);CHKERRQ(ierr);
    ierr = PetscDrawPopCurrentPoint(draw);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "KSPSetNormType"
/*@
   KSPSetNormType - Sets the norm that is used for convergence testing.

   Logically Collective on KSP

   Input Parameter:
+  ksp - Krylov solver context
-  normtype - one of
$   KSP_NORM_NONE - skips computing the norm, this should only be used if you are using
$                 the Krylov method as a smoother with a fixed small number of iterations.
$                 Implicitly sets KSPSkipConverged as KSP convergence test.
$                 Supported only by CG, Richardson, Bi-CG-stab, CR, and CGS methods.
$   KSP_NORM_PRECONDITIONED - the default for left preconditioned solves, uses the l2 norm
$                 of the preconditioned residual
$   KSP_NORM_UNPRECONDITIONED - uses the l2 norm of the true b - Ax residual, supported only by
$                 CG, CHEBYSHEV, and RICHARDSON, automatically true for right (see KSPSetPCSide())
$                 preconditioning..
$   KSP_NORM_NATURAL - supported  by KSPCG, KSPCR, KSPCGNE, KSPCGS


   Options Database Key:
.   -ksp_norm_type <none,preconditioned,unpreconditioned,natural>

   Notes:
   Currently only works with the CG, Richardson, Bi-CG-stab, CR, and CGS methods.

   Level: advanced

.keywords: KSP, create, context, norms

.seealso: KSPSetUp(), KSPSolve(), KSPDestroy(), KSPSkipConverged(), KSPSetCheckNormIteration()
@*/
PetscErrorCode  KSPSetNormType(KSP ksp,KSPNormType normtype)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidLogicalCollectiveEnum(ksp,normtype,2);
  ksp->normtype = normtype;
  if (normtype == KSP_NORM_NONE) {
    ierr = KSPSetConvergenceTest(ksp,KSPSkipConverged,0,0);CHKERRQ(ierr);
    ierr = PetscInfo(ksp,"Warning: setting KSPNormType to skip computing the norm\n\
 KSP convergence test is implicitly set to KSPSkipConverged\n");CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetCheckNormIteration"
/*@
   KSPSetCheckNormIteration - Sets the first iteration at which the norm of the residual will be
     computed and used in the convergence test.

   Logically Collective on KSP

   Input Parameter:
+  ksp - Krylov solver context
-  it  - use -1 to check at all iterations

   Notes:
   Currently only works with KSPCG, KSPBCGS and KSPIBCGS

   Use KSPSetNormType(ksp,KSP_NORM_NONE) to never check the norm

   On steps where the norm is not computed, the previous norm is still in the variable, so if you run with, for example,
    -ksp_monitor the residual norm will appear to be unchanged for several iterations (though it is not really unchanged).
   Level: advanced

.keywords: KSP, create, context, norms

.seealso: KSPSetUp(), KSPSolve(), KSPDestroy(), KSPSkipConverged(), KSPSetNormType()
@*/
PetscErrorCode  KSPSetCheckNormIteration(KSP ksp,PetscInt it)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidLogicalCollectiveInt(ksp,it,2);
  ksp->chknorm = it;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetLagNorm"
/*@
   KSPSetLagNorm - Lags the residual norm calculation so that it is computed as part of the MPI_Allreduce() for
   computing the inner products for the next iteration.  This can reduce communication costs at the expense of doing
   one additional iteration.


   Logically Collective on KSP

   Input Parameter:
+  ksp - Krylov solver context
-  flg - PETSC_TRUE or PETSC_FALSE

   Options Database Keys:
.  -ksp_lag_norm - lag the calculated residual norm

   Notes:
   Currently only works with KSPIBCGS.

   Use KSPSetNormType(ksp,KSP_NORM_NONE) to never check the norm

   If you lag the norm and run with, for example, -ksp_monitor, the residual norm reported will be the lagged one.
   Level: advanced

.keywords: KSP, create, context, norms

.seealso: KSPSetUp(), KSPSolve(), KSPDestroy(), KSPSkipConverged(), KSPSetNormType(), KSPSetCheckNormIteration()
@*/
PetscErrorCode  KSPSetLagNorm(KSP ksp,PetscBool flg)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidLogicalCollectiveBool(ksp,flg,2);
  ksp->lagnorm = flg;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetSupportedNorm"
/*@
   KSPSetSupportedNorm - Sets a norm and preconditioner side supported by a KSP

   Logically Collective

   Input Arguments:
+  ksp - Krylov method
.  normtype - supported norm type
.  pcside - preconditioner side that can be used with this norm
-  preference - integer preference for this combination, larger values have higher priority

   Level: developer

   Notes:
   This function should be called from the implementation files KSPCreate_XXX() to declare
   which norms and preconditioner sides are supported. Users should not need to call this
   function.

   KSP_NORM_NONE is supported by default with all KSP methods and any PC side. If a KSP explicitly does not support
   KSP_NORM_NONE, it should set this by setting priority=0.

.seealso: KSPSetNormType(), KSPSetPCSide()
@*/
PetscErrorCode KSPSetSupportedNorm(KSP ksp,KSPNormType normtype,PCSide pcside,PetscInt priority)
{

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  ksp->normsupporttable[normtype][pcside] = priority;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPNormSupportTableReset_Private"
PetscErrorCode KSPNormSupportTableReset_Private(KSP ksp)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscMemzero(ksp->normsupporttable,sizeof(ksp->normsupporttable));CHKERRQ(ierr);
  ierr = KSPSetSupportedNorm(ksp,KSP_NORM_NONE,PC_LEFT,1);CHKERRQ(ierr);
  ierr = KSPSetSupportedNorm(ksp,KSP_NORM_NONE,PC_RIGHT,1);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetUpNorms_Private"
PetscErrorCode KSPSetUpNorms_Private(KSP ksp,KSPNormType *normtype,PCSide *pcside)
{
  PetscInt i,j,best,ibest = 0,jbest = 0;

  PetscFunctionBegin;
  best = 0;
  for (i=0; i<KSP_NORM_MAX; i++) {
    for (j=0; j<PC_SIDE_MAX; j++) {
      if ((ksp->normtype == KSP_NORM_DEFAULT || ksp->normtype == i)
          && (ksp->pc_side == PC_SIDE_DEFAULT || ksp->pc_side == j)
          && (ksp->normsupporttable[i][j] > best)) {
        if (ksp->normtype == KSP_NORM_DEFAULT && i == KSP_NORM_NONE && ksp->normsupporttable[i][j] <= 1) {
          continue; /* Skip because we don't want to default to no norms unless set by the KSP (preonly). */
        }
        best  = ksp->normsupporttable[i][j];
        ibest = i;
        jbest = j;
      }
    }
  }
  if (best < 1) {
    if (ksp->normtype == KSP_NORM_DEFAULT && ksp->pc_side == PC_SIDE_DEFAULT) SETERRQ1(PetscObjectComm((PetscObject)ksp),PETSC_ERR_PLIB,"The %s KSP implementation did not call KSPSetSupportedNorm()",((PetscObject)ksp)->type_name);
    if (ksp->normtype == KSP_NORM_DEFAULT) SETERRQ2(PetscObjectComm((PetscObject)ksp),PETSC_ERR_SUP,"KSP %s does not support %s",((PetscObject)ksp)->type_name,PCSides[ksp->pc_side]);
    if (ksp->pc_side == PC_SIDE_DEFAULT) SETERRQ2(PetscObjectComm((PetscObject)ksp),PETSC_ERR_SUP,"KSP %s does not support %s",((PetscObject)ksp)->type_name,KSPNormTypes[ksp->normtype]);
    SETERRQ3(PetscObjectComm((PetscObject)ksp),PETSC_ERR_SUP,"KSP %s does not support %s with %s",((PetscObject)ksp)->type_name,KSPNormTypes[ksp->normtype],PCSides[ksp->pc_side]);
  }
  *normtype = (KSPNormType)ibest;
  *pcside   = (PCSide)jbest;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPGetNormType"
/*@
   KSPGetNormType - Gets the norm that is used for convergence testing.

   Not Collective

   Input Parameter:
.  ksp - Krylov solver context

   Output Parameter:
.  normtype - norm that is used for convergence testing

   Level: advanced

.keywords: KSP, create, context, norms

.seealso: KSPNormType, KSPSetNormType(), KSPSkipConverged()
@*/
PetscErrorCode  KSPGetNormType(KSP ksp, KSPNormType *normtype)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidPointer(normtype,2);
  ierr      = KSPSetUpNorms_Private(ksp,&ksp->normtype,&ksp->pc_side);CHKERRQ(ierr);
  *normtype = ksp->normtype;
  PetscFunctionReturn(0);
}

#if defined(PETSC_HAVE_AMS)
#include <petscviewerams.h>
#endif

#undef __FUNCT__
#define __FUNCT__ "KSPSetOperators"
/*@
   KSPSetOperators - Sets the matrix associated with the linear system
   and a (possibly) different one associated with the preconditioner.

   Collective on KSP and Mat

   Input Parameters:
+  ksp - the KSP context
.  Amat - the matrix that defines the linear system
.  Pmat - the matrix to be used in constructing the preconditioner, usually the same as Amat.
-  flag - flag indicating information about the preconditioner matrix structure
   during successive linear solves.  This flag is ignored the first time a
   linear system is solved, and thus is irrelevant when solving just one linear
   system.

   Notes:
   The flag can be used to eliminate unnecessary work in the preconditioner
   during the repeated solution of linear systems of the same size.  The
   available options are
$    SAME_PRECONDITIONER -
$      Pmat is identical during successive linear solves.
$      This option is intended for folks who are using
$      different Amat and Pmat matrices and want to reuse the
$      same preconditioner matrix.  For example, this option
$      saves work by not recomputing incomplete factorization
$      for ILU/ICC preconditioners.
$    SAME_NONZERO_PATTERN -
$      Pmat has the same nonzero structure during
$      successive linear solves.
$    DIFFERENT_NONZERO_PATTERN -
$      Pmat does not have the same nonzero structure.

    All future calls to KSPSetOperators() must use the same size matrices!

    Passing a NULL for Amat or Pmat removes the matrix that is currently used.

    If you wish to replace either Amat or Pmat but leave the other one untouched then
    first call KSPGetOperators() to get the one you wish to keep, call PetscObjectReference()
    on it and then pass it back in in your call to KSPSetOperators().

    Caution:
    If you specify SAME_NONZERO_PATTERN, PETSc believes your assertion
    and does not check the structure of the matrix.  If you erroneously
    claim that the structure is the same when it actually is not, the new
    preconditioner will not function correctly.  Thus, use this optimization
    feature carefully!

    If in doubt about whether your preconditioner matrix has changed
    structure or not, use the flag DIFFERENT_NONZERO_PATTERN.

    Level: beginner

   Alternative usage: If the operators have NOT been set with KSP/PCSetOperators() then the operators
      are created in PC and returned to the user. In this case, if both operators
      mat and pmat are requested, two DIFFERENT operators will be returned. If
      only one is requested both operators in the PC will be the same (i.e. as
      if one had called KSP/PCSetOperators() with the same argument for both Mats).
      The user must set the sizes of the returned matrices and their type etc just
      as if the user created them with MatCreate(). For example,

$         KSP/PCGetOperators(ksp/pc,&mat,NULL,NULL); is equivalent to
$           set size, type, etc of mat

$         MatCreate(comm,&mat);
$         KSP/PCSetOperators(ksp/pc,mat,mat,SAME_NONZERO_PATTERN);
$         PetscObjectDereference((PetscObject)mat);
$           set size, type, etc of mat

     and

$         KSP/PCGetOperators(ksp/pc,&mat,&pmat,NULL); is equivalent to
$           set size, type, etc of mat and pmat

$         MatCreate(comm,&mat);
$         MatCreate(comm,&pmat);
$         KSP/PCSetOperators(ksp/pc,mat,pmat,SAME_NONZERO_PATTERN);
$         PetscObjectDereference((PetscObject)mat);
$         PetscObjectDereference((PetscObject)pmat);
$           set size, type, etc of mat and pmat

    The rational for this support is so that when creating a TS, SNES, or KSP the hierarchy
    of underlying objects (i.e. SNES, KSP, PC, Mat) and their livespans can be completely
    managed by the top most level object (i.e. the TS, SNES, or KSP). Another way to look
    at this is when you create a SNES you do not NEED to create a KSP and attach it to
    the SNES object (the SNES object manages it for you). Similarly when you create a KSP
    you do not need to attach a PC to it (the KSP object manages the PC object for you).
    Thus, why should YOU have to create the Mat and attach it to the SNES/KSP/PC, when
    it can be created for you?

.keywords: KSP, set, operators, matrix, preconditioner, linear system

.seealso: KSPSolve(), KSPGetPC(), PCGetOperators(), PCSetOperators(), KSPGetOperators()
@*/
PetscErrorCode  KSPSetOperators(KSP ksp,Mat Amat,Mat Pmat,MatStructure flag)
{
  MatNullSpace   nullsp;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  if (Amat) PetscValidHeaderSpecific(Amat,MAT_CLASSID,2);
  if (Pmat) PetscValidHeaderSpecific(Pmat,MAT_CLASSID,3);
  if (Amat) PetscCheckSameComm(ksp,1,Amat,2);
  if (Pmat) PetscCheckSameComm(ksp,1,Pmat,3);
  if (!ksp->pc) {ierr = KSPGetPC(ksp,&ksp->pc);CHKERRQ(ierr);}
  ierr = PCSetOperators(ksp->pc,Amat,Pmat,flag);CHKERRQ(ierr);
  if (ksp->setupstage == KSP_SETUP_NEWRHS) ksp->setupstage = KSP_SETUP_NEWMATRIX;  /* so that next solve call will call PCSetUp() on new matrix */
  if (ksp->guess) {
    ierr = KSPFischerGuessReset(ksp->guess);CHKERRQ(ierr);
  }
  if (Pmat) {
    ierr = MatGetNullSpace(Pmat, &nullsp);CHKERRQ(ierr);
    if (nullsp) {
      ierr = KSPSetNullSpace(ksp, nullsp);CHKERRQ(ierr);
    }
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPGetOperators"
/*@
   KSPGetOperators - Gets the matrix associated with the linear system
   and a (possibly) different one associated with the preconditioner.

   Collective on KSP and Mat

   Input Parameter:
.  ksp - the KSP context

   Output Parameters:
+  Amat - the matrix that defines the linear system
.  Pmat - the matrix to be used in constructing the preconditioner, usually the same as Amat.
-  flag - flag indicating information about the preconditioner matrix structure
   during successive linear solves.  This flag is ignored the first time a
   linear system is solved, and thus is irrelevant when solving just one linear
   system.

    Level: intermediate

   Notes: DOES NOT increase the reference counts of the matrix, so you should NOT destroy them.

.keywords: KSP, set, get, operators, matrix, preconditioner, linear system

.seealso: KSPSolve(), KSPGetPC(), PCGetOperators(), PCSetOperators(), KSPSetOperators(), KSPGetOperatorsSet()
@*/
PetscErrorCode  KSPGetOperators(KSP ksp,Mat *Amat,Mat *Pmat,MatStructure *flag)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  if (!ksp->pc) {ierr = KSPGetPC(ksp,&ksp->pc);CHKERRQ(ierr);}
  ierr = PCGetOperators(ksp->pc,Amat,Pmat,flag);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPGetOperatorsSet"
/*@C
   KSPGetOperatorsSet - Determines if the matrix associated with the linear system and
   possibly a different one associated with the preconditioner have been set in the KSP.

   Not collective, though the results on all processes should be the same

   Input Parameter:
.  pc - the KSP context

   Output Parameters:
+  mat - the matrix associated with the linear system was set
-  pmat - matrix associated with the preconditioner was set, usually the same

   Level: intermediate

.keywords: KSP, get, operators, matrix, linear system

.seealso: PCSetOperators(), KSPGetOperators(), KSPSetOperators(), PCGetOperators(), PCGetOperatorsSet()
@*/
PetscErrorCode  KSPGetOperatorsSet(KSP ksp,PetscBool  *mat,PetscBool  *pmat)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  if (!ksp->pc) {ierr = KSPGetPC(ksp,&ksp->pc);CHKERRQ(ierr);}
  ierr = PCGetOperatorsSet(ksp->pc,mat,pmat);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetPreSolve"
/*@C
   KSPSetPreSolve - Sets a function that is called before every KSPSolve() is started

   Logically Collective on KSP

   Input Parameters:
+   ksp - the solver object
.   presolve - the function to call before the solve
-   prectx - any context needed by the function

   Level: developer

.keywords: KSP, create, context

.seealso: KSPSetUp(), KSPSolve(), KSPDestroy(), KSP, KSPSetPostSolve()
@*/
PetscErrorCode  KSPSetPreSolve(KSP ksp,PetscErrorCode (*presolve)(KSP,Vec,Vec,void*),void *prectx)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  ksp->presolve = presolve;
  ksp->prectx   = prectx;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetPostSolve"
/*@C
   KSPSetPostSolve - Sets a function that is called after every KSPSolve() completes (whether it converges or not)

   Logically Collective on KSP

   Input Parameters:
+   ksp - the solver object
.   postsolve - the function to call after the solve
-   postctx - any context needed by the function

   Level: developer

.keywords: KSP, create, context

.seealso: KSPSetUp(), KSPSolve(), KSPDestroy(), KSP, KSPSetPreSolve()
@*/
PetscErrorCode  KSPSetPostSolve(KSP ksp,PetscErrorCode (*postsolve)(KSP,Vec,Vec,void*),void *postctx)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  ksp->postsolve = postsolve;
  ksp->postctx   = postctx;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPCreate"
/*@
   KSPCreate - Creates the default KSP context.

   Collective on MPI_Comm

   Input Parameter:
.  comm - MPI communicator

   Output Parameter:
.  ksp - location to put the KSP context

   Notes:
   The default KSP type is GMRES with a restart of 30, using modified Gram-Schmidt
   orthogonalization.

   Level: beginner

.keywords: KSP, create, context

.seealso: KSPSetUp(), KSPSolve(), KSPDestroy(), KSP
@*/
PetscErrorCode  KSPCreate(MPI_Comm comm,KSP *inksp)
{
  KSP            ksp;
  PetscErrorCode ierr;
  void           *ctx;

  PetscFunctionBegin;
  PetscValidPointer(inksp,2);
  *inksp = 0;
#if !defined(PETSC_USE_DYNAMIC_LIBRARIES)
  ierr = KSPInitializePackage();CHKERRQ(ierr);
#endif

  ierr = PetscHeaderCreate(ksp,_p_KSP,struct _KSPOps,KSP_CLASSID,"KSP","Krylov Method","KSP",comm,KSPDestroy,KSPView);CHKERRQ(ierr);

  ksp->max_it  = 10000;
  ksp->pc_side = PC_SIDE_DEFAULT;
  ksp->rtol    = 1.e-5;
  ksp->abstol  = 1.e-50;
  ksp->divtol  = 1.e4;

  ksp->chknorm        = -1;
  ksp->normtype       = KSP_NORM_DEFAULT;
  ksp->rnorm          = 0.0;
  ksp->its            = 0;
  ksp->guess_zero     = PETSC_TRUE;
  ksp->calc_sings     = PETSC_FALSE;
  ksp->res_hist       = NULL;
  ksp->res_hist_alloc = NULL;
  ksp->res_hist_len   = 0;
  ksp->res_hist_max   = 0;
  ksp->res_hist_reset = PETSC_TRUE;
  ksp->numbermonitors = 0;

  ierr                    = KSPDefaultConvergedCreate(&ctx);CHKERRQ(ierr);
  ierr                    = KSPSetConvergenceTest(ksp,KSPDefaultConverged,ctx,KSPDefaultConvergedDestroy);CHKERRQ(ierr);
  ksp->ops->buildsolution = KSPBuildSolutionDefault;
  ksp->ops->buildresidual = KSPBuildResidualDefault;

  ksp->vec_sol    = 0;
  ksp->vec_rhs    = 0;
  ksp->pc         = 0;
  ksp->data       = 0;
  ksp->nwork      = 0;
  ksp->work       = 0;
  ksp->reason     = KSP_CONVERGED_ITERATING;
  ksp->setupstage = KSP_SETUP_NEW;

  ierr = KSPNormSupportTableReset_Private(ksp);CHKERRQ(ierr);

  *inksp = ksp;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetType"
/*@C
   KSPSetType - Builds KSP for a particular solver.

   Logically Collective on KSP

   Input Parameters:
+  ksp      - the Krylov space context
-  type - a known method

   Options Database Key:
.  -ksp_type  <method> - Sets the method; use -help for a list
    of available methods (for instance, cg or gmres)

   Notes:
   See "petsc/include/petscksp.h" for available methods (for instance,
   KSPCG or KSPGMRES).

  Normally, it is best to use the KSPSetFromOptions() command and
  then set the KSP type from the options database rather than by using
  this routine.  Using the options database provides the user with
  maximum flexibility in evaluating the many different Krylov methods.
  The KSPSetType() routine is provided for those situations where it
  is necessary to set the iterative solver independently of the command
  line or options database.  This might be the case, for example, when
  the choice of iterative solver changes during the execution of the
  program, and the user's application is taking responsibility for
  choosing the appropriate method.  In other words, this routine is
  not for beginners.

  Level: intermediate

  Developer Note: KSPRegister() is used to add Krylov types to KSPList from which they
  are accessed by KSPSetType().

.keywords: KSP, set, method

.seealso: PCSetType(), KSPType, KSPRegister(), KSPCreate()

@*/
PetscErrorCode  KSPSetType(KSP ksp, KSPType type)
{
  PetscErrorCode ierr,(*r)(KSP);
  PetscBool      match;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidCharPointer(type,2);

  ierr = PetscObjectTypeCompare((PetscObject)ksp,type,&match);CHKERRQ(ierr);
  if (match) PetscFunctionReturn(0);

  ierr =  PetscFunctionListFind(KSPList,type,&r);CHKERRQ(ierr);
  if (!r) SETERRQ1(PetscObjectComm((PetscObject)ksp),PETSC_ERR_ARG_UNKNOWN_TYPE,"Unable to find requested KSP type %s",type);
  /* Destroy the previous private KSP context */
  if (ksp->ops->destroy) {
    ierr              = (*ksp->ops->destroy)(ksp);CHKERRQ(ierr);
    ksp->ops->destroy = NULL;
  }
  /* Reinitialize function pointers in KSPOps structure */
  ierr                    = PetscMemzero(ksp->ops,sizeof(struct _KSPOps));CHKERRQ(ierr);
  ksp->ops->buildsolution = KSPBuildSolutionDefault;
  ksp->ops->buildresidual = KSPBuildResidualDefault;
  ierr                    = KSPNormSupportTableReset_Private(ksp);CHKERRQ(ierr);
  /* Call the KSPCreate_XXX routine for this particular Krylov solver */
  ksp->setupstage = KSP_SETUP_NEW;
  ierr            = PetscObjectChangeTypeName((PetscObject)ksp,type);CHKERRQ(ierr);
  ierr            = (*r)(ksp);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPGetType"
/*@C
   KSPGetType - Gets the KSP type as a string from the KSP object.

   Not Collective

   Input Parameter:
.  ksp - Krylov context

   Output Parameter:
.  name - name of KSP method

   Level: intermediate

.keywords: KSP, get, method, name

.seealso: KSPSetType()
@*/
PetscErrorCode  KSPGetType(KSP ksp,KSPType *type)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidPointer(type,2);
  *type = ((PetscObject)ksp)->type_name;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPRegister"
/*@C
  KSPRegister -  Adds a method to the Krylov subspace solver package.

   Not Collective

   Input Parameters:
+  name_solver - name of a new user-defined solver
-  routine_create - routine to create method context

   Notes:
   KSPRegister() may be called multiple times to add several user-defined solvers.

   Sample usage:
.vb
   KSPRegister("my_solver",MySolverCreate);
.ve

   Then, your solver can be chosen with the procedural interface via
$     KSPSetType(ksp,"my_solver")
   or at runtime via the option
$     -ksp_type my_solver

   Level: advanced

.keywords: KSP, register

.seealso: KSPRegisterAll(), KSPRegisterDestroy()

@*/
PetscErrorCode  KSPRegister(const char sname[],PetscErrorCode (*function)(KSP))
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFunctionListAdd(&KSPList,sname,function);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPSetNullSpace"
/*@
  KSPSetNullSpace - Sets the null space of the operator

  Logically Collective on KSP

  Input Parameters:
+  ksp - the Krylov space object
-  nullsp - the null space of the operator

  Level: advanced

.seealso: KSPSetOperators(), MatNullSpaceCreate(), KSPGetNullSpace(), MatSetNullSpace()
@*/
PetscErrorCode  KSPSetNullSpace(KSP ksp,MatNullSpace nullsp)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidHeaderSpecific(nullsp,MAT_NULLSPACE_CLASSID,2);
  ierr = PetscObjectReference((PetscObject)nullsp);CHKERRQ(ierr);
  if (ksp->nullsp) { ierr = MatNullSpaceDestroy(&ksp->nullsp);CHKERRQ(ierr); }
  ksp->nullsp = nullsp;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "KSPGetNullSpace"
/*@
  KSPGetNullSpace - Gets the null space of the operator

  Not Collective

  Input Parameters:
+  ksp - the Krylov space object
-  nullsp - the null space of the operator

  Level: advanced

.seealso: KSPSetOperators(), MatNullSpaceCreate(), KSPSetNullSpace()
@*/
PetscErrorCode  KSPGetNullSpace(KSP ksp,MatNullSpace *nullsp)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ksp,KSP_CLASSID,1);
  PetscValidPointer(nullsp,2);
  *nullsp = ksp->nullsp;
  PetscFunctionReturn(0);
}

