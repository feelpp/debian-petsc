
#include <petsc-private/tsimpl.h>        /*I "petscts.h"  I*/
#include <petscdmshell.h>
#include <petscdmda.h>
#include <petscviewer.h>
#include <petscdraw.h>

/* Logging support */
PetscClassId  TS_CLASSID, DMTS_CLASSID;
PetscLogEvent TS_Step, TS_PseudoComputeTimeStep, TS_FunctionEval, TS_JacobianEval;

const char *const TSExactFinalTimeOptions[] = {"STEPOVER","INTERPOLATE","MATCHSTEP","TSExactFinalTimeOption","TS_EXACTFINALTIME_",0};

#undef __FUNCT__
#define __FUNCT__ "TSSetTypeFromOptions"
/*
  TSSetTypeFromOptions - Sets the type of ts from user options.

  Collective on TS

  Input Parameter:
. ts - The ts

  Level: intermediate

.keywords: TS, set, options, database, type
.seealso: TSSetFromOptions(), TSSetType()
*/
static PetscErrorCode TSSetTypeFromOptions(TS ts)
{
  PetscBool      opt;
  const char     *defaultType;
  char           typeName[256];
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (((PetscObject)ts)->type_name) defaultType = ((PetscObject)ts)->type_name;
  else defaultType = TSEULER;

  if (!TSRegisterAllCalled) {ierr = TSRegisterAll();CHKERRQ(ierr);}
  ierr = PetscOptionsList("-ts_type", "TS method"," TSSetType", TSList, defaultType, typeName, 256, &opt);CHKERRQ(ierr);
  if (opt) {
    ierr = TSSetType(ts, typeName);CHKERRQ(ierr);
  } else {
    ierr = TSSetType(ts, defaultType);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

struct _n_TSMonitorDrawCtx {
  PetscViewer   viewer;
  PetscDrawAxis axis;
  Vec           initialsolution;
  PetscBool     showinitial;
  PetscInt      howoften;  /* when > 0 uses step % howoften, when negative only final solution plotted */
  PetscBool     showtimestepandtime;
  int           color;
};

#undef __FUNCT__
#define __FUNCT__ "TSSetFromOptions"
/*@
   TSSetFromOptions - Sets various TS parameters from user options.

   Collective on TS

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Options Database Keys:
+  -ts_type <type> - TSEULER, TSBEULER, TSSUNDIALS, TSPSEUDO, TSCN, TSRK, TSTHETA, TSGL, TSSSP
.  -ts_max_steps maxsteps - maximum number of time-steps to take
.  -ts_final_time time - maximum time to compute to
.  -ts_dt dt - initial time step
.  -ts_monitor - print information at each timestep
.  -ts_monitor_lg_timestep - Monitor timestep size graphically
.  -ts_monitor_lg_solution - Monitor solution graphically
.  -ts_monitor_lg_error - Monitor error graphically
.  -ts_monitor_lg_snes_iterations - Monitor number nonlinear iterations for each timestep graphically
.  -ts_monitor_lg_ksp_iterations - Monitor number nonlinear iterations for each timestep graphically
.  -ts_monitor_sp_eig - Monitor eigenvalues of linearized operator graphically
.  -ts_monitor_draw_solution - Monitor solution graphically
.  -ts_monitor_draw_solution_phase - Monitor solution graphically with phase diagram
.  -ts_monitor_draw_error - Monitor error graphically
.  -ts_monitor_solution_binary <filename> - Save each solution to a binary file
-  -ts_monitor_solution_vtk <filename.vts> - Save each time step to a binary file, use filename-%%03D.vts

   Developer Note: We should unify all the -ts_monitor options in the way that -xxx_view has been unified

   Level: beginner

.keywords: TS, timestep, set, options, database

.seealso: TSGetType()
@*/
PetscErrorCode  TSSetFromOptions(TS ts)
{
  PetscBool              opt,flg;
  PetscErrorCode         ierr;
  PetscViewer            monviewer;
  char                   monfilename[PETSC_MAX_PATH_LEN];
  SNES                   snes;
  TSAdapt                adapt;
  PetscReal              time_step;
  TSExactFinalTimeOption eftopt;
  char                   dir[16];

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  ierr = PetscObjectOptionsBegin((PetscObject)ts);CHKERRQ(ierr);
  /* Handle TS type options */
  ierr = TSSetTypeFromOptions(ts);CHKERRQ(ierr);

  /* Handle generic TS options */
  ierr = PetscOptionsInt("-ts_max_steps","Maximum number of time steps","TSSetDuration",ts->max_steps,&ts->max_steps,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsReal("-ts_final_time","Time to run to","TSSetDuration",ts->max_time,&ts->max_time,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsReal("-ts_init_time","Initial time","TSSetTime",ts->ptime,&ts->ptime,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsReal("-ts_dt","Initial time step","TSSetTimeStep",ts->time_step,&time_step,&flg);CHKERRQ(ierr);
  if (flg) {
    ierr = TSSetTimeStep(ts,time_step);CHKERRQ(ierr);
  }
  ierr = PetscOptionsEnum("-ts_exact_final_time","Option for handling of final time step","TSSetExactFinalTime",TSExactFinalTimeOptions,(PetscEnum)ts->exact_final_time,(PetscEnum*)&eftopt,&flg);CHKERRQ(ierr);
  if (flg) {ierr = TSSetExactFinalTime(ts,eftopt);CHKERRQ(ierr);}
  ierr = PetscOptionsInt("-ts_max_snes_failures","Maximum number of nonlinear solve failures","TSSetMaxSNESFailures",ts->max_snes_failures,&ts->max_snes_failures,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsInt("-ts_max_reject","Maximum number of step rejections before step fails","TSSetMaxStepRejections",ts->max_reject,&ts->max_reject,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsBool("-ts_error_if_step_fails","Error if no step succeeds","TSSetErrorIfStepFails",ts->errorifstepfailed,&ts->errorifstepfailed,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsReal("-ts_rtol","Relative tolerance for local truncation error","TSSetTolerances",ts->rtol,&ts->rtol,NULL);CHKERRQ(ierr);
  ierr = PetscOptionsReal("-ts_atol","Absolute tolerance for local truncation error","TSSetTolerances",ts->atol,&ts->atol,NULL);CHKERRQ(ierr);

  /* Monitor options */
  ierr = PetscOptionsString("-ts_monitor","Monitor timestep size","TSMonitorDefault","stdout",monfilename,PETSC_MAX_PATH_LEN,&flg);CHKERRQ(ierr);
  if (flg) {
    ierr = PetscViewerASCIIOpen(PetscObjectComm((PetscObject)ts),monfilename,&monviewer);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorDefault,monviewer,(PetscErrorCode (*)(void**))PetscViewerDestroy);CHKERRQ(ierr);
  }
  ierr = PetscOptionsString("-ts_monitor_python","Use Python function","TSMonitorSet",0,monfilename,PETSC_MAX_PATH_LEN,&flg);CHKERRQ(ierr);
  if (flg) {ierr = PetscPythonMonitorSet((PetscObject)ts,monfilename);CHKERRQ(ierr);}

  ierr = PetscOptionsName("-ts_monitor_lg_timestep","Monitor timestep size graphically","TSMonitorLGTimeStep",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorLGCtx ctx;
    PetscInt       howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_lg_timestep","Monitor timestep size graphically","TSMonitorLGTimeStep",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorLGCtxCreate(PetscObjectComm((PetscObject)ts),0,0,PETSC_DECIDE,PETSC_DECIDE,300,300,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorLGTimeStep,ctx,(PetscErrorCode (*)(void**))TSMonitorLGCtxDestroy);CHKERRQ(ierr);
  }
  ierr = PetscOptionsName("-ts_monitor_lg_solution","Monitor solution graphically","TSMonitorLGSolution",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorLGCtx ctx;
    PetscInt       howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_lg_solution","Monitor solution graphically","TSMonitorLGSolution",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorLGCtxCreate(PETSC_COMM_SELF,0,0,PETSC_DECIDE,PETSC_DECIDE,600,400,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorLGSolution,ctx,(PetscErrorCode (*)(void**))TSMonitorLGCtxDestroy);CHKERRQ(ierr);
  }
  ierr = PetscOptionsName("-ts_monitor_lg_error","Monitor error graphically","TSMonitorLGError",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorLGCtx ctx;
    PetscInt       howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_lg_error","Monitor error graphically","TSMonitorLGError",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorLGCtxCreate(PETSC_COMM_SELF,0,0,PETSC_DECIDE,PETSC_DECIDE,600,400,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorLGError,ctx,(PetscErrorCode (*)(void**))TSMonitorLGCtxDestroy);CHKERRQ(ierr);
  }
  ierr = PetscOptionsName("-ts_monitor_lg_snes_iterations","Monitor number nonlinear iterations for each timestep graphically","TSMonitorLGSNESIterations",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorLGCtx ctx;
    PetscInt       howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_lg_snes_iterations","Monitor number nonlinear iterations for each timestep graphically","TSMonitorLGSNESIterations",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorLGCtxCreate(PETSC_COMM_SELF,0,0,PETSC_DECIDE,PETSC_DECIDE,300,300,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorLGSNESIterations,ctx,(PetscErrorCode (*)(void**))TSMonitorLGCtxDestroy);CHKERRQ(ierr);
  }
  ierr = PetscOptionsName("-ts_monitor_lg_ksp_iterations","Monitor number nonlinear iterations for each timestep graphically","TSMonitorLGKSPIterations",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorLGCtx ctx;
    PetscInt       howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_lg_ksp_iterations","Monitor number nonlinear iterations for each timestep graphically","TSMonitorLGKSPIterations",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorLGCtxCreate(PETSC_COMM_SELF,0,0,PETSC_DECIDE,PETSC_DECIDE,300,300,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorLGKSPIterations,ctx,(PetscErrorCode (*)(void**))TSMonitorLGCtxDestroy);CHKERRQ(ierr);
  }
  ierr = PetscOptionsName("-ts_monitor_sp_eig","Monitor eigenvalues of linearized operator graphically","TSMonitorSPEig",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorSPEigCtx ctx;
    PetscInt          howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_sp_eig","Monitor eigenvalues of linearized operator graphically","TSMonitorSPEig",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorSPEigCtxCreate(PETSC_COMM_SELF,0,0,PETSC_DECIDE,PETSC_DECIDE,600,400,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorSPEig,ctx,(PetscErrorCode (*)(void**))TSMonitorSPEigCtxDestroy);CHKERRQ(ierr);
  }
  opt  = PETSC_FALSE;
  ierr = PetscOptionsName("-ts_monitor_draw_solution","Monitor solution graphically","TSMonitorDrawSolution",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorDrawCtx ctx;
    PetscInt         howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_draw_solution","Monitor solution graphically","TSMonitorDrawSolution",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorDrawCtxCreate(PetscObjectComm((PetscObject)ts),0,0,PETSC_DECIDE,PETSC_DECIDE,600,400,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorDrawSolution,ctx,(PetscErrorCode (*)(void**))TSMonitorDrawCtxDestroy);CHKERRQ(ierr);
  }
  opt  = PETSC_FALSE;
  ierr = PetscOptionsName("-ts_monitor_draw_solution_phase","Monitor solution graphically","TSMonitorDrawSolutionPhase",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorDrawCtx ctx;
    PetscReal        bounds[4];
    PetscInt         n = 4;
    PetscDraw        draw;

    ierr = PetscOptionsRealArray("-ts_monitor_draw_solution_phase","Monitor solution graphically","TSMonitorDrawSolutionPhase",bounds,&n,NULL);CHKERRQ(ierr);
    if (n != 4) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_ARG_WRONG,"Must provide bounding box of phase field");
    ierr = TSMonitorDrawCtxCreate(PetscObjectComm((PetscObject)ts),0,0,PETSC_DECIDE,PETSC_DECIDE,600,400,1,&ctx);CHKERRQ(ierr);
    ierr = PetscViewerDrawGetDraw(ctx->viewer,0,&draw);CHKERRQ(ierr);
    ierr = PetscDrawClear(draw);CHKERRQ(ierr);
    ierr = PetscDrawAxisCreate(draw,&ctx->axis);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLimits(ctx->axis,bounds[0],bounds[2],bounds[1],bounds[3]);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLabels(ctx->axis,"Phase Diagram","Variable 1","Variable 2");CHKERRQ(ierr);
    ierr = PetscDrawAxisDraw(ctx->axis);CHKERRQ(ierr);
    /* ierr = PetscDrawSetCoordinates(draw,bounds[0],bounds[1],bounds[2],bounds[3]);CHKERRQ(ierr); */
    ierr = TSMonitorSet(ts,TSMonitorDrawSolutionPhase,ctx,(PetscErrorCode (*)(void**))TSMonitorDrawCtxDestroy);CHKERRQ(ierr);
  }
  opt  = PETSC_FALSE;
  ierr = PetscOptionsName("-ts_monitor_draw_error","Monitor error graphically","TSMonitorDrawError",&opt);CHKERRQ(ierr);
  if (opt) {
    TSMonitorDrawCtx ctx;
    PetscInt         howoften = 1;

    ierr = PetscOptionsInt("-ts_monitor_draw_error","Monitor error graphically","TSMonitorDrawError",howoften,&howoften,NULL);CHKERRQ(ierr);
    ierr = TSMonitorDrawCtxCreate(PetscObjectComm((PetscObject)ts),0,0,PETSC_DECIDE,PETSC_DECIDE,600,400,howoften,&ctx);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorDrawError,ctx,(PetscErrorCode (*)(void**))TSMonitorDrawCtxDestroy);CHKERRQ(ierr);
  }
  opt  = PETSC_FALSE;
  ierr = PetscOptionsString("-ts_monitor_solution_binary","Save each solution to a binary file","TSMonitorSolutionBinary",0,monfilename,PETSC_MAX_PATH_LEN,&flg);CHKERRQ(ierr);
  if (flg) {
    PetscViewer ctx;
    if (monfilename[0]) {
      ierr = PetscViewerBinaryOpen(PetscObjectComm((PetscObject)ts),monfilename,FILE_MODE_WRITE,&ctx);CHKERRQ(ierr);
      ierr = TSMonitorSet(ts,TSMonitorSolutionBinary,ctx,(PetscErrorCode (*)(void**))PetscViewerDestroy);CHKERRQ(ierr);
    } else {
      ctx = PETSC_VIEWER_BINARY_(PetscObjectComm((PetscObject)ts));
      ierr = TSMonitorSet(ts,TSMonitorSolutionBinary,ctx,(PetscErrorCode (*)(void**))NULL);CHKERRQ(ierr);
    }
  }
  opt  = PETSC_FALSE;
  ierr = PetscOptionsString("-ts_monitor_solution_vtk","Save each time step to a binary file, use filename-%%03D.vts","TSMonitorSolutionVTK",0,monfilename,PETSC_MAX_PATH_LEN,&flg);CHKERRQ(ierr);
  if (flg) {
    const char *ptr,*ptr2;
    char       *filetemplate;
    if (!monfilename[0]) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"-ts_monitor_solution_vtk requires a file template, e.g. filename-%%03D.vts");
    /* Do some cursory validation of the input. */
    ierr = PetscStrstr(monfilename,"%",(char**)&ptr);CHKERRQ(ierr);
    if (!ptr) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"-ts_monitor_solution_vtk requires a file template, e.g. filename-%%03D.vts");
    for (ptr++; ptr && *ptr; ptr++) {
      ierr = PetscStrchr("DdiouxX",*ptr,(char**)&ptr2);CHKERRQ(ierr);
      if (!ptr2 && (*ptr < '0' || '9' < *ptr)) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"Invalid file template argument to -ts_monitor_solution_vtk, should look like filename-%%03D.vts");
      if (ptr2) break;
    }
    ierr = PetscStrallocpy(monfilename,&filetemplate);CHKERRQ(ierr);
    ierr = TSMonitorSet(ts,TSMonitorSolutionVTK,filetemplate,(PetscErrorCode (*)(void**))TSMonitorSolutionVTKDestroy);CHKERRQ(ierr);
  }

  ierr = PetscOptionsString("-ts_monitor_dmda_ray","Display a ray of the solution","None","y=0",dir,16,&flg);CHKERRQ(ierr);
  if (flg) {
    TSMonitorDMDARayCtx *rayctx;
    int                 ray = 0;
    DMDADirection       ddir;
    DM                  da;
    PetscMPIInt         rank;

    if (dir[1] != '=') SETERRQ1(PetscObjectComm((PetscObject)ts),PETSC_ERR_ARG_WRONG,"Unknown ray %s",dir);
    if (dir[0] == 'x') ddir = DMDA_X;
    else if (dir[0] == 'y') ddir = DMDA_Y;
    else SETERRQ1(PetscObjectComm((PetscObject)ts),PETSC_ERR_ARG_WRONG,"Unknown ray %s",dir);
    sscanf(dir+2,"%d",&ray);

    ierr = PetscInfo2(((PetscObject)ts),"Displaying DMDA ray %c = %D\n",dir[0],ray);CHKERRQ(ierr);
    ierr = PetscNew(TSMonitorDMDARayCtx,&rayctx);CHKERRQ(ierr);
    ierr = TSGetDM(ts,&da);CHKERRQ(ierr);
    ierr = DMDAGetRay(da,ddir,ray,&rayctx->ray,&rayctx->scatter);CHKERRQ(ierr);
    ierr = MPI_Comm_rank(PetscObjectComm((PetscObject)ts),&rank);CHKERRQ(ierr);
    if (!rank) {
      ierr = PetscViewerDrawOpen(PETSC_COMM_SELF,0,0,0,0,600,300,&rayctx->viewer);CHKERRQ(ierr);
    }
    ierr = TSMonitorSet(ts,TSMonitorDMDARay,rayctx,TSMonitorDMDARayDestroy);CHKERRQ(ierr);
  }

  ierr = TSGetAdapt(ts,&adapt);CHKERRQ(ierr);
  ierr = TSAdaptSetFromOptions(adapt);CHKERRQ(ierr);

  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  if (ts->problem_type == TS_LINEAR) {ierr = SNESSetType(snes,SNESKSPONLY);CHKERRQ(ierr);}

  /* Handle specific TS options */
  if (ts->ops->setfromoptions) {
    ierr = (*ts->ops->setfromoptions)(ts);CHKERRQ(ierr);
  }

  /* process any options handlers added with PetscObjectAddOptionsHandler() */
  ierr = PetscObjectProcessOptionsHandlers((PetscObject)ts);CHKERRQ(ierr);
  ierr = PetscOptionsEnd();CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#undef __FUNCT__
#define __FUNCT__ "TSComputeRHSJacobian"
/*@
   TSComputeRHSJacobian - Computes the Jacobian matrix that has been
      set with TSSetRHSJacobian().

   Collective on TS and Vec

   Input Parameters:
+  ts - the TS context
.  t - current timestep
-  U - input vector

   Output Parameters:
+  A - Jacobian matrix
.  B - optional preconditioning matrix
-  flag - flag indicating matrix structure

   Notes:
   Most users should not need to explicitly call this routine, as it
   is used internally within the nonlinear solvers.

   See KSPSetOperators() for important information about setting the
   flag parameter.

   Level: developer

.keywords: SNES, compute, Jacobian, matrix

.seealso:  TSSetRHSJacobian(), KSPSetOperators()
@*/
PetscErrorCode  TSComputeRHSJacobian(TS ts,PetscReal t,Vec U,Mat *A,Mat *B,MatStructure *flg)
{
  PetscErrorCode ierr;
  PetscInt       Ustate;
  DM             dm;
  DMTS           tsdm;
  TSRHSJacobian  rhsjacobianfunc;
  void           *ctx;
  TSIJacobian    ijacobianfunc;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  PetscCheckSameComm(ts,1,U,3);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMGetDMTS(dm,&tsdm);CHKERRQ(ierr);
  ierr = DMTSGetRHSJacobian(dm,&rhsjacobianfunc,&ctx);CHKERRQ(ierr);
  ierr = DMTSGetIJacobian(dm,&ijacobianfunc,NULL);CHKERRQ(ierr);
  ierr = PetscObjectStateQuery((PetscObject)U,&Ustate);CHKERRQ(ierr);
  if (ts->rhsjacobian.time == t && (ts->problem_type == TS_LINEAR || (ts->rhsjacobian.X == U && ts->rhsjacobian.Xstate == Ustate))) {
    *flg = ts->rhsjacobian.mstructure;
    PetscFunctionReturn(0);
  }

  if (!rhsjacobianfunc && !ijacobianfunc) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"Must call TSSetRHSJacobian() and / or TSSetIJacobian()");

  if (rhsjacobianfunc) {
    ierr = PetscLogEventBegin(TS_JacobianEval,ts,U,*A,*B);CHKERRQ(ierr);
    *flg = DIFFERENT_NONZERO_PATTERN;
    PetscStackPush("TS user Jacobian function");
    ierr = (*rhsjacobianfunc)(ts,t,U,A,B,flg,ctx);CHKERRQ(ierr);
    PetscStackPop;
    ierr = PetscLogEventEnd(TS_JacobianEval,ts,U,*A,*B);CHKERRQ(ierr);
    /* make sure user returned a correct Jacobian and preconditioner */
    PetscValidHeaderSpecific(*A,MAT_CLASSID,4);
    PetscValidHeaderSpecific(*B,MAT_CLASSID,5);
  } else {
    ierr = MatZeroEntries(*A);CHKERRQ(ierr);
    if (*A != *B) {ierr = MatZeroEntries(*B);CHKERRQ(ierr);}
    *flg = SAME_NONZERO_PATTERN;
  }
  ts->rhsjacobian.time       = t;
  ts->rhsjacobian.X          = U;
  ierr                       = PetscObjectStateQuery((PetscObject)U,&ts->rhsjacobian.Xstate);CHKERRQ(ierr);
  ts->rhsjacobian.mstructure = *flg;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeRHSFunction"
/*@
   TSComputeRHSFunction - Evaluates the right-hand-side function.

   Collective on TS and Vec

   Input Parameters:
+  ts - the TS context
.  t - current time
-  U - state vector

   Output Parameter:
.  y - right hand side

   Note:
   Most users should not need to explicitly call this routine, as it
   is used internally within the nonlinear solvers.

   Level: developer

.keywords: TS, compute

.seealso: TSSetRHSFunction(), TSComputeIFunction()
@*/
PetscErrorCode TSComputeRHSFunction(TS ts,PetscReal t,Vec U,Vec y)
{
  PetscErrorCode ierr;
  TSRHSFunction  rhsfunction;
  TSIFunction    ifunction;
  void           *ctx;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  PetscValidHeaderSpecific(y,VEC_CLASSID,4);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetRHSFunction(dm,&rhsfunction,&ctx);CHKERRQ(ierr);
  ierr = DMTSGetIFunction(dm,&ifunction,NULL);CHKERRQ(ierr);

  if (!rhsfunction && !ifunction) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"Must call TSSetRHSFunction() and / or TSSetIFunction()");

  ierr = PetscLogEventBegin(TS_FunctionEval,ts,U,y,0);CHKERRQ(ierr);
  if (rhsfunction) {
    PetscStackPush("TS user right-hand-side function");
    ierr = (*rhsfunction)(ts,t,U,y,ctx);CHKERRQ(ierr);
    PetscStackPop;
  } else {
    ierr = VecZeroEntries(y);CHKERRQ(ierr);
  }

  ierr = PetscLogEventEnd(TS_FunctionEval,ts,U,y,0);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeSolutionFunction"
/*@
   TSComputeSolutionFunction - Evaluates the solution function.

   Collective on TS and Vec

   Input Parameters:
+  ts - the TS context
-  t - current time

   Output Parameter:
.  U - the solution

   Note:
   Most users should not need to explicitly call this routine, as it
   is used internally within the nonlinear solvers.

   Level: developer

.keywords: TS, compute

.seealso: TSSetSolutionFunction(), TSSetRHSFunction(), TSComputeIFunction()
@*/
PetscErrorCode TSComputeSolutionFunction(TS ts,PetscReal t,Vec U)
{
  PetscErrorCode     ierr;
  TSSolutionFunction solutionfunction;
  void               *ctx;
  DM                 dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetSolutionFunction(dm,&solutionfunction,&ctx);CHKERRQ(ierr);

  if (solutionfunction) {
    PetscStackPush("TS user solution function");
    ierr = (*solutionfunction)(ts,t,U,ctx);CHKERRQ(ierr);
    PetscStackPop;
  }
  PetscFunctionReturn(0);
}
#undef __FUNCT__
#define __FUNCT__ "TSComputeForcingFunction"
/*@
   TSComputeForcingFunction - Evaluates the forcing function.

   Collective on TS and Vec

   Input Parameters:
+  ts - the TS context
-  t - current time

   Output Parameter:
.  U - the function value

   Note:
   Most users should not need to explicitly call this routine, as it
   is used internally within the nonlinear solvers.

   Level: developer

.keywords: TS, compute

.seealso: TSSetSolutionFunction(), TSSetRHSFunction(), TSComputeIFunction()
@*/
PetscErrorCode TSComputeForcingFunction(TS ts,PetscReal t,Vec U)
{
  PetscErrorCode     ierr, (*forcing)(TS,PetscReal,Vec,void*);
  void               *ctx;
  DM                 dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetForcingFunction(dm,&forcing,&ctx);CHKERRQ(ierr);

  if (forcing) {
    PetscStackPush("TS user forcing function");
    ierr = (*forcing)(ts,t,U,ctx);CHKERRQ(ierr);
    PetscStackPop;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetRHSVec_Private"
static PetscErrorCode TSGetRHSVec_Private(TS ts,Vec *Frhs)
{
  Vec            F;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  *Frhs = NULL;
  ierr  = TSGetIFunction(ts,&F,NULL,NULL);CHKERRQ(ierr);
  if (!ts->Frhs) {
    ierr = VecDuplicate(F,&ts->Frhs);CHKERRQ(ierr);
  }
  *Frhs = ts->Frhs;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetRHSMats_Private"
static PetscErrorCode TSGetRHSMats_Private(TS ts,Mat *Arhs,Mat *Brhs)
{
  Mat            A,B;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = TSGetIJacobian(ts,&A,&B,NULL,NULL);CHKERRQ(ierr);
  if (Arhs) {
    if (!ts->Arhs) {
      ierr = MatDuplicate(A,MAT_DO_NOT_COPY_VALUES,&ts->Arhs);CHKERRQ(ierr);
    }
    *Arhs = ts->Arhs;
  }
  if (Brhs) {
    if (!ts->Brhs) {
      ierr = MatDuplicate(B,MAT_DO_NOT_COPY_VALUES,&ts->Brhs);CHKERRQ(ierr);
    }
    *Brhs = ts->Brhs;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeIFunction"
/*@
   TSComputeIFunction - Evaluates the DAE residual written in implicit form F(t,U,Udot)=0

   Collective on TS and Vec

   Input Parameters:
+  ts - the TS context
.  t - current time
.  U - state vector
.  Udot - time derivative of state vector
-  imex - flag indicates if the method is IMEX so that the RHSFunction should be kept separate

   Output Parameter:
.  Y - right hand side

   Note:
   Most users should not need to explicitly call this routine, as it
   is used internally within the nonlinear solvers.

   If the user did did not write their equations in implicit form, this
   function recasts them in implicit form.

   Level: developer

.keywords: TS, compute

.seealso: TSSetIFunction(), TSComputeRHSFunction()
@*/
PetscErrorCode TSComputeIFunction(TS ts,PetscReal t,Vec U,Vec Udot,Vec Y,PetscBool imex)
{
  PetscErrorCode ierr;
  TSIFunction    ifunction;
  TSRHSFunction  rhsfunction;
  void           *ctx;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  PetscValidHeaderSpecific(Udot,VEC_CLASSID,4);
  PetscValidHeaderSpecific(Y,VEC_CLASSID,5);

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetIFunction(dm,&ifunction,&ctx);CHKERRQ(ierr);
  ierr = DMTSGetRHSFunction(dm,&rhsfunction,NULL);CHKERRQ(ierr);

  if (!rhsfunction && !ifunction) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"Must call TSSetRHSFunction() and / or TSSetIFunction()");

  ierr = PetscLogEventBegin(TS_FunctionEval,ts,U,Udot,Y);CHKERRQ(ierr);
  if (ifunction) {
    PetscStackPush("TS user implicit function");
    ierr = (*ifunction)(ts,t,U,Udot,Y,ctx);CHKERRQ(ierr);
    PetscStackPop;
  }
  if (imex) {
    if (!ifunction) {
      ierr = VecCopy(Udot,Y);CHKERRQ(ierr);
    }
  } else if (rhsfunction) {
    if (ifunction) {
      Vec Frhs;
      ierr = TSGetRHSVec_Private(ts,&Frhs);CHKERRQ(ierr);
      ierr = TSComputeRHSFunction(ts,t,U,Frhs);CHKERRQ(ierr);
      ierr = VecAXPY(Y,-1,Frhs);CHKERRQ(ierr);
    } else {
      ierr = TSComputeRHSFunction(ts,t,U,Y);CHKERRQ(ierr);
      ierr = VecAYPX(Y,-1,Udot);CHKERRQ(ierr);
    }
  }
  ierr = PetscLogEventEnd(TS_FunctionEval,ts,U,Udot,Y);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeIJacobian"
/*@
   TSComputeIJacobian - Evaluates the Jacobian of the DAE

   Collective on TS and Vec

   Input
      Input Parameters:
+  ts - the TS context
.  t - current timestep
.  U - state vector
.  Udot - time derivative of state vector
.  shift - shift to apply, see note below
-  imex - flag indicates if the method is IMEX so that the RHSJacobian should be kept separate

   Output Parameters:
+  A - Jacobian matrix
.  B - optional preconditioning matrix
-  flag - flag indicating matrix structure

   Notes:
   If F(t,U,Udot)=0 is the DAE, the required Jacobian is

   dF/dU + shift*dF/dUdot

   Most users should not need to explicitly call this routine, as it
   is used internally within the nonlinear solvers.

   Level: developer

.keywords: TS, compute, Jacobian, matrix

.seealso:  TSSetIJacobian()
@*/
PetscErrorCode TSComputeIJacobian(TS ts,PetscReal t,Vec U,Vec Udot,PetscReal shift,Mat *A,Mat *B,MatStructure *flg,PetscBool imex)
{
  PetscInt       Ustate, Udotstate;
  PetscErrorCode ierr;
  TSIJacobian    ijacobian;
  TSRHSJacobian  rhsjacobian;
  DM             dm;
  void           *ctx;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  PetscValidHeaderSpecific(Udot,VEC_CLASSID,4);
  PetscValidPointer(A,6);
  PetscValidHeaderSpecific(*A,MAT_CLASSID,6);
  PetscValidPointer(B,7);
  PetscValidHeaderSpecific(*B,MAT_CLASSID,7);
  PetscValidPointer(flg,8);

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetIJacobian(dm,&ijacobian,&ctx);CHKERRQ(ierr);
  ierr = DMTSGetRHSJacobian(dm,&rhsjacobian,NULL);CHKERRQ(ierr);

  ierr = PetscObjectStateQuery((PetscObject)U,&Ustate);CHKERRQ(ierr);
  ierr = PetscObjectStateQuery((PetscObject)Udot,&Udotstate);CHKERRQ(ierr);
  if (ts->ijacobian.time == t && (ts->problem_type == TS_LINEAR || (ts->ijacobian.X == U && ts->ijacobian.Xstate == Ustate && ts->ijacobian.Xdot == Udot && ts->ijacobian.Xdotstate == Udotstate && ts->ijacobian.imex == imex))) {
    *flg = ts->ijacobian.mstructure;
    ierr = MatScale(*A, shift / ts->ijacobian.shift);CHKERRQ(ierr);
    PetscFunctionReturn(0);
  }

  if (!rhsjacobian && !ijacobian) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_USER,"Must call TSSetRHSJacobian() and / or TSSetIJacobian()");

  *flg = SAME_NONZERO_PATTERN;  /* In case we're solving a linear problem in which case it wouldn't get initialized below. */
  ierr = PetscLogEventBegin(TS_JacobianEval,ts,U,*A,*B);CHKERRQ(ierr);
  if (ijacobian) {
    *flg = DIFFERENT_NONZERO_PATTERN;
    PetscStackPush("TS user implicit Jacobian");
    ierr = (*ijacobian)(ts,t,U,Udot,shift,A,B,flg,ctx);CHKERRQ(ierr);
    PetscStackPop;
    /* make sure user returned a correct Jacobian and preconditioner */
    PetscValidHeaderSpecific(*A,MAT_CLASSID,4);
    PetscValidHeaderSpecific(*B,MAT_CLASSID,5);
  }
  if (imex) {
    if (!ijacobian) {  /* system was written as Udot = G(t,U) */
      ierr = MatZeroEntries(*A);CHKERRQ(ierr);
      ierr = MatShift(*A,shift);CHKERRQ(ierr);
      if (*A != *B) {
        ierr = MatZeroEntries(*B);CHKERRQ(ierr);
        ierr = MatShift(*B,shift);CHKERRQ(ierr);
      }
      *flg = SAME_PRECONDITIONER;
    }
  } else {
    if (!ijacobian) {
      ierr = TSComputeRHSJacobian(ts,t,U,A,B,flg);CHKERRQ(ierr);
      ierr = MatScale(*A,-1);CHKERRQ(ierr);
      ierr = MatShift(*A,shift);CHKERRQ(ierr);
      if (*A != *B) {
        ierr = MatScale(*B,-1);CHKERRQ(ierr);
        ierr = MatShift(*B,shift);CHKERRQ(ierr);
      }
    } else if (rhsjacobian) {
      Mat          Arhs,Brhs;
      MatStructure axpy,flg2 = DIFFERENT_NONZERO_PATTERN;
      ierr = TSGetRHSMats_Private(ts,&Arhs,&Brhs);CHKERRQ(ierr);
      ierr = TSComputeRHSJacobian(ts,t,U,&Arhs,&Brhs,&flg2);CHKERRQ(ierr);
      axpy = (*flg == flg2) ? SAME_NONZERO_PATTERN : DIFFERENT_NONZERO_PATTERN;
      ierr = MatAXPY(*A,-1,Arhs,axpy);CHKERRQ(ierr);
      if (*A != *B) {
        ierr = MatAXPY(*B,-1,Brhs,axpy);CHKERRQ(ierr);
      }
      *flg = PetscMin(*flg,flg2);
    }
  }

  ts->ijacobian.time = t;
  ts->ijacobian.X    = U;
  ts->ijacobian.Xdot = Udot;

  ierr = PetscObjectStateQuery((PetscObject)U,&ts->ijacobian.Xstate);CHKERRQ(ierr);
  ierr = PetscObjectStateQuery((PetscObject)Udot,&ts->ijacobian.Xdotstate);CHKERRQ(ierr);

  ts->ijacobian.shift      = shift;
  ts->ijacobian.imex       = imex;
  ts->ijacobian.mstructure = *flg;

  ierr = PetscLogEventEnd(TS_JacobianEval,ts,U,*A,*B);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetRHSFunction"
/*@C
    TSSetRHSFunction - Sets the routine for evaluating the function,
    where U_t = G(t,u).

    Logically Collective on TS

    Input Parameters:
+   ts - the TS context obtained from TSCreate()
.   r - vector to put the computed right hand side (or NULL to have it created)
.   f - routine for evaluating the right-hand-side function
-   ctx - [optional] user-defined context for private data for the
          function evaluation routine (may be NULL)

    Calling sequence of func:
$     func (TS ts,PetscReal t,Vec u,Vec F,void *ctx);

+   t - current timestep
.   u - input vector
.   F - function vector
-   ctx - [optional] user-defined function context

    Level: beginner

.keywords: TS, timestep, set, right-hand-side, function

.seealso: TSSetRHSJacobian(), TSSetIJacobian()
@*/
PetscErrorCode  TSSetRHSFunction(TS ts,Vec r,PetscErrorCode (*f)(TS,PetscReal,Vec,Vec,void*),void *ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  Vec            ralloc = NULL;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (r) PetscValidHeaderSpecific(r,VEC_CLASSID,2);

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSSetRHSFunction(dm,f,ctx);CHKERRQ(ierr);
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  if (!r && !ts->dm && ts->vec_sol) {
    ierr = VecDuplicate(ts->vec_sol,&ralloc);CHKERRQ(ierr);
    r    = ralloc;
  }
  ierr = SNESSetFunction(snes,r,SNESTSFormFunction,ts);CHKERRQ(ierr);
  ierr = VecDestroy(&ralloc);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetSolutionFunction"
/*@C
    TSSetSolutionFunction - Provide a function that computes the solution of the ODE or DAE

    Logically Collective on TS

    Input Parameters:
+   ts - the TS context obtained from TSCreate()
.   f - routine for evaluating the solution
-   ctx - [optional] user-defined context for private data for the
          function evaluation routine (may be NULL)

    Calling sequence of func:
$     func (TS ts,PetscReal t,Vec u,void *ctx);

+   t - current timestep
.   u - output vector
-   ctx - [optional] user-defined function context

    Notes:
    This routine is used for testing accuracy of time integration schemes when you already know the solution.
    If analytic solutions are not known for your system, consider using the Method of Manufactured Solutions to
    create closed-form solutions with non-physical forcing terms.

    For low-dimensional problems solved in serial, such as small discrete systems, TSMonitorLGError() can be used to monitor the error history.

    Level: beginner

.keywords: TS, timestep, set, right-hand-side, function

.seealso: TSSetRHSJacobian(), TSSetIJacobian(), TSComputeSolutionFunction(), TSSetForcingFunction()
@*/
PetscErrorCode  TSSetSolutionFunction(TS ts,PetscErrorCode (*f)(TS,PetscReal,Vec,void*),void *ctx)
{
  PetscErrorCode ierr;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSSetSolutionFunction(dm,f,ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetForcingFunction"
/*@C
    TSSetForcingFunction - Provide a function that computes a forcing term for a ODE or PDE

    Logically Collective on TS

    Input Parameters:
+   ts - the TS context obtained from TSCreate()
.   f - routine for evaluating the forcing function
-   ctx - [optional] user-defined context for private data for the
          function evaluation routine (may be NULL)

    Calling sequence of func:
$     func (TS ts,PetscReal t,Vec u,void *ctx);

+   t - current timestep
.   u - output vector
-   ctx - [optional] user-defined function context

    Notes:
    This routine is useful for testing accuracy of time integration schemes when using the Method of Manufactured Solutions to
    create closed-form solutions with a non-physical forcing term.

    For low-dimensional problems solved in serial, such as small discrete systems, TSMonitorLGError() can be used to monitor the error history.

    Level: beginner

.keywords: TS, timestep, set, right-hand-side, function

.seealso: TSSetRHSJacobian(), TSSetIJacobian(), TSComputeSolutionFunction(), TSSetSolutionFunction()
@*/
PetscErrorCode  TSSetForcingFunction(TS ts,PetscErrorCode (*f)(TS,PetscReal,Vec,void*),void *ctx)
{
  PetscErrorCode ierr;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSSetForcingFunction(dm,f,ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetRHSJacobian"
/*@C
   TSSetRHSJacobian - Sets the function to compute the Jacobian of F,
   where U_t = G(U,t), as well as the location to store the matrix.

   Logically Collective on TS

   Input Parameters:
+  ts  - the TS context obtained from TSCreate()
.  Amat - (approximate) Jacobian matrix
.  Pmat - matrix from which preconditioner is to be constructed (usually the same as Amat)
.  f   - the Jacobian evaluation routine
-  ctx - [optional] user-defined context for private data for the
         Jacobian evaluation routine (may be NULL)

   Calling sequence of func:
$     func (TS ts,PetscReal t,Vec u,Mat *A,Mat *B,MatStructure *flag,void *ctx);

+  t - current timestep
.  u - input vector
.  Amat - (approximate) Jacobian matrix
.  Pmat - matrix from which preconditioner is to be constructed (usually the same as Amat)
.  flag - flag indicating information about the preconditioner matrix
          structure (same as flag in KSPSetOperators())
-  ctx - [optional] user-defined context for matrix evaluation routine

   Notes:
   See KSPSetOperators() for important information about setting the flag
   output parameter in the routine func().  Be sure to read this information!

   The routine func() takes Mat * as the matrix arguments rather than Mat.
   This allows the matrix evaluation routine to replace A and/or B with a
   completely new matrix structure (not just different matrix elements)
   when appropriate, for instance, if the nonzero structure is changing
   throughout the global iterations.

   Level: beginner

.keywords: TS, timestep, set, right-hand-side, Jacobian

.seealso: SNESComputeJacobianDefaultColor(), TSSetRHSFunction()

@*/
PetscErrorCode  TSSetRHSJacobian(TS ts,Mat Amat,Mat Pmat,TSRHSJacobian f,void *ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  DM             dm;
  TSIJacobian    ijacobian;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (Amat) PetscValidHeaderSpecific(Amat,MAT_CLASSID,2);
  if (Pmat) PetscValidHeaderSpecific(Pmat,MAT_CLASSID,3);
  if (Amat) PetscCheckSameComm(ts,1,Amat,2);
  if (Pmat) PetscCheckSameComm(ts,1,Pmat,3);

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSSetRHSJacobian(dm,f,ctx);CHKERRQ(ierr);
  ierr = DMTSGetIJacobian(dm,&ijacobian,NULL);CHKERRQ(ierr);

  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  if (!ijacobian) {
    ierr = SNESSetJacobian(snes,Amat,Pmat,SNESTSFormJacobian,ts);CHKERRQ(ierr);
  }
  if (Amat) {
    ierr = PetscObjectReference((PetscObject)Amat);CHKERRQ(ierr);
    ierr = MatDestroy(&ts->Arhs);CHKERRQ(ierr);

    ts->Arhs = Amat;
  }
  if (Pmat) {
    ierr = PetscObjectReference((PetscObject)Pmat);CHKERRQ(ierr);
    ierr = MatDestroy(&ts->Brhs);CHKERRQ(ierr);

    ts->Brhs = Pmat;
  }
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSSetIFunction"
/*@C
   TSSetIFunction - Set the function to compute F(t,U,U_t) where F() = 0 is the DAE to be solved.

   Logically Collective on TS

   Input Parameters:
+  ts  - the TS context obtained from TSCreate()
.  r   - vector to hold the residual (or NULL to have it created internally)
.  f   - the function evaluation routine
-  ctx - user-defined context for private data for the function evaluation routine (may be NULL)

   Calling sequence of f:
$  f(TS ts,PetscReal t,Vec u,Vec u_t,Vec F,ctx);

+  t   - time at step/stage being solved
.  u   - state vector
.  u_t - time derivative of state vector
.  F   - function vector
-  ctx - [optional] user-defined context for matrix evaluation routine

   Important:
   The user MUST call either this routine, TSSetRHSFunction().  This routine must be used when not solving an ODE, for example a DAE.

   Level: beginner

.keywords: TS, timestep, set, DAE, Jacobian

.seealso: TSSetRHSJacobian(), TSSetRHSFunction(), TSSetIJacobian()
@*/
PetscErrorCode  TSSetIFunction(TS ts,Vec res,TSIFunction f,void *ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  Vec            resalloc = NULL;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (res) PetscValidHeaderSpecific(res,VEC_CLASSID,2);

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSSetIFunction(dm,f,ctx);CHKERRQ(ierr);

  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  if (!res && !ts->dm && ts->vec_sol) {
    ierr = VecDuplicate(ts->vec_sol,&resalloc);CHKERRQ(ierr);
    res  = resalloc;
  }
  ierr = SNESSetFunction(snes,res,SNESTSFormFunction,ts);CHKERRQ(ierr);
  ierr = VecDestroy(&resalloc);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetIFunction"
/*@C
   TSGetIFunction - Returns the vector where the implicit residual is stored and the function/contex to compute it.

   Not Collective

   Input Parameter:
.  ts - the TS context

   Output Parameter:
+  r - vector to hold residual (or NULL)
.  func - the function to compute residual (or NULL)
-  ctx - the function context (or NULL)

   Level: advanced

.keywords: TS, nonlinear, get, function

.seealso: TSSetIFunction(), SNESGetFunction()
@*/
PetscErrorCode TSGetIFunction(TS ts,Vec *r,TSIFunction *func,void **ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESGetFunction(snes,r,NULL,NULL);CHKERRQ(ierr);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetIFunction(dm,func,ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetRHSFunction"
/*@C
   TSGetRHSFunction - Returns the vector where the right hand side is stored and the function/context to compute it.

   Not Collective

   Input Parameter:
.  ts - the TS context

   Output Parameter:
+  r - vector to hold computed right hand side (or NULL)
.  func - the function to compute right hand side (or NULL)
-  ctx - the function context (or NULL)

   Level: advanced

.keywords: TS, nonlinear, get, function

.seealso: TSSetRhsfunction(), SNESGetFunction()
@*/
PetscErrorCode TSGetRHSFunction(TS ts,Vec *r,TSRHSFunction *func,void **ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESGetFunction(snes,r,NULL,NULL);CHKERRQ(ierr);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetRHSFunction(dm,func,ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetIJacobian"
/*@C
   TSSetIJacobian - Set the function to compute the matrix dF/dU + a*dF/dU_t where F(t,U,U_t) is the function
        you provided with TSSetIFunction().

   Logically Collective on TS

   Input Parameters:
+  ts  - the TS context obtained from TSCreate()
.  Amat - (approximate) Jacobian matrix
.  Pmat - matrix used to compute preconditioner (usually the same as Amat)
.  f   - the Jacobian evaluation routine
-  ctx - user-defined context for private data for the Jacobian evaluation routine (may be NULL)

   Calling sequence of f:
$  f(TS ts,PetscReal t,Vec U,Vec U_t,PetscReal a,Mat *Amat,Mat *Pmat,MatStructure *flag,void *ctx);

+  t    - time at step/stage being solved
.  U    - state vector
.  U_t  - time derivative of state vector
.  a    - shift
.  Amat - (approximate) Jacobian of F(t,U,W+a*U), equivalent to dF/dU + a*dF/dU_t
.  Pmat - matrix used for constructing preconditioner, usually the same as Amat
.  flag - flag indicating information about the preconditioner matrix
          structure (same as flag in KSPSetOperators())
-  ctx  - [optional] user-defined context for matrix evaluation routine

   Notes:
   The matrices Amat and Pmat are exactly the matrices that are used by SNES for the nonlinear solve.

   The matrix dF/dU + a*dF/dU_t you provide turns out to be
   the Jacobian of F(t,U,W+a*U) where F(t,U,U_t) = 0 is the DAE to be solved.
   The time integrator internally approximates U_t by W+a*U where the positive "shift"
   a and vector W depend on the integration method, step size, and past states. For example with
   the backward Euler method a = 1/dt and W = -a*U(previous timestep) so
   W + a*U = a*(U - U(previous timestep)) = (U - U(previous timestep))/dt

   Level: beginner

.keywords: TS, timestep, DAE, Jacobian

.seealso: TSSetIFunction(), TSSetRHSJacobian(), SNESComputeJacobianDefaultColor(), SNESComputeJacobianDefault()

@*/
PetscErrorCode  TSSetIJacobian(TS ts,Mat Amat,Mat Pmat,TSIJacobian f,void *ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (Amat) PetscValidHeaderSpecific(Amat,MAT_CLASSID,2);
  if (Pmat) PetscValidHeaderSpecific(Pmat,MAT_CLASSID,3);
  if (Amat) PetscCheckSameComm(ts,1,Amat,2);
  if (Pmat) PetscCheckSameComm(ts,1,Pmat,3);

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSSetIJacobian(dm,f,ctx);CHKERRQ(ierr);

  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESSetJacobian(snes,Amat,Pmat,SNESTSFormJacobian,ts);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSLoad"
/*@C
  TSLoad - Loads a KSP that has been stored in binary  with KSPView().

  Collective on PetscViewer

  Input Parameters:
+ newdm - the newly loaded TS, this needs to have been created with TSCreate() or
           some related function before a call to TSLoad().
- viewer - binary file viewer, obtained from PetscViewerBinaryOpen()

   Level: intermediate

  Notes:
   The type is determined by the data in the file, any type set into the TS before this call is ignored.

  Notes for advanced users:
  Most users should not need to know the details of the binary storage
  format, since TSLoad() and TSView() completely hide these details.
  But for anyone who's interested, the standard binary matrix storage
  format is
.vb
     has not yet been determined
.ve

.seealso: PetscViewerBinaryOpen(), TSView(), MatLoad(), VecLoad()
@*/
PetscErrorCode  TSLoad(TS ts, PetscViewer viewer)
{
  PetscErrorCode ierr;
  PetscBool      isbinary;
  PetscInt       classid;
  char           type[256];
  DMTS           sdm;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(viewer,PETSC_VIEWER_CLASSID,2);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERBINARY,&isbinary);CHKERRQ(ierr);
  if (!isbinary) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONG,"Invalid viewer; open viewer with PetscViewerBinaryOpen()");

  ierr = PetscViewerBinaryRead(viewer,&classid,1,PETSC_INT);CHKERRQ(ierr);
  if (classid != TS_FILE_CLASSID) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_ARG_WRONG,"Not TS next in file");
  ierr = PetscViewerBinaryRead(viewer,type,256,PETSC_CHAR);CHKERRQ(ierr);
  ierr = TSSetType(ts, type);CHKERRQ(ierr);
  if (ts->ops->load) {
    ierr = (*ts->ops->load)(ts,viewer);CHKERRQ(ierr);
  }
  ierr = DMCreate(PetscObjectComm((PetscObject)ts),&dm);CHKERRQ(ierr);
  ierr = DMLoad(dm,viewer);CHKERRQ(ierr);
  ierr = TSSetDM(ts,dm);CHKERRQ(ierr);
  ierr = DMCreateGlobalVector(ts->dm,&ts->vec_sol);CHKERRQ(ierr);
  ierr = VecLoad(ts->vec_sol,viewer);CHKERRQ(ierr);
  ierr = DMGetDMTS(ts->dm,&sdm);CHKERRQ(ierr);
  ierr = DMTSLoad(sdm,viewer);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#include <petscdraw.h>
#if defined(PETSC_HAVE_AMS)
#include <petscviewerams.h>
#endif
#undef __FUNCT__
#define __FUNCT__ "TSView"
/*@C
    TSView - Prints the TS data structure.

    Collective on TS

    Input Parameters:
+   ts - the TS context obtained from TSCreate()
-   viewer - visualization context

    Options Database Key:
.   -ts_view - calls TSView() at end of TSStep()

    Notes:
    The available visualization contexts include
+     PETSC_VIEWER_STDOUT_SELF - standard output (default)
-     PETSC_VIEWER_STDOUT_WORLD - synchronized standard
         output where only the first processor opens
         the file.  All other processors send their
         data to the first processor to print.

    The user can open an alternative visualization context with
    PetscViewerASCIIOpen() - output to a specified file.

    Level: beginner

.keywords: TS, timestep, view

.seealso: PetscViewerASCIIOpen()
@*/
PetscErrorCode  TSView(TS ts,PetscViewer viewer)
{
  PetscErrorCode ierr;
  TSType         type;
  PetscBool      iascii,isstring,isundials,isbinary,isdraw;
  DMTS           sdm;
#if defined(PETSC_HAVE_AMS)
  PetscBool      isams;
#endif

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (!viewer) {
    ierr = PetscViewerASCIIGetStdout(PetscObjectComm((PetscObject)ts),&viewer);CHKERRQ(ierr);
  }
  PetscValidHeaderSpecific(viewer,PETSC_VIEWER_CLASSID,2);
  PetscCheckSameComm(ts,1,viewer,2);

  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERASCII,&iascii);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERSTRING,&isstring);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERBINARY,&isbinary);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERDRAW,&isdraw);CHKERRQ(ierr);
#if defined(PETSC_HAVE_AMS)
  ierr = PetscObjectTypeCompare((PetscObject)viewer,PETSCVIEWERAMS,&isams);CHKERRQ(ierr);
#endif
  if (iascii) {
    ierr = PetscObjectPrintClassNamePrefixType((PetscObject)ts,viewer,"TS Object");CHKERRQ(ierr);
    ierr = PetscViewerASCIIPrintf(viewer,"  maximum steps=%D\n",ts->max_steps);CHKERRQ(ierr);
    ierr = PetscViewerASCIIPrintf(viewer,"  maximum time=%G\n",ts->max_time);CHKERRQ(ierr);
    if (ts->problem_type == TS_NONLINEAR) {
      ierr = PetscViewerASCIIPrintf(viewer,"  total number of nonlinear solver iterations=%D\n",ts->snes_its);CHKERRQ(ierr);
      ierr = PetscViewerASCIIPrintf(viewer,"  total number of nonlinear solve failures=%D\n",ts->num_snes_failures);CHKERRQ(ierr);
    }
    ierr = PetscViewerASCIIPrintf(viewer,"  total number of linear solver iterations=%D\n",ts->ksp_its);CHKERRQ(ierr);
    ierr = PetscViewerASCIIPrintf(viewer,"  total number of rejected steps=%D\n",ts->reject);CHKERRQ(ierr);
    ierr = DMGetDMTS(ts->dm,&sdm);CHKERRQ(ierr);
    ierr = DMTSView(sdm,viewer);CHKERRQ(ierr);
    if (ts->ops->view) {
      ierr = PetscViewerASCIIPushTab(viewer);CHKERRQ(ierr);
      ierr = (*ts->ops->view)(ts,viewer);CHKERRQ(ierr);
      ierr = PetscViewerASCIIPopTab(viewer);CHKERRQ(ierr);
    }
  } else if (isstring) {
    ierr = TSGetType(ts,&type);CHKERRQ(ierr);
    ierr = PetscViewerStringSPrintf(viewer," %-7.7s",type);CHKERRQ(ierr);
  } else if (isbinary) {
    PetscInt    classid = TS_FILE_CLASSID;
    MPI_Comm    comm;
    PetscMPIInt rank;
    char        type[256];

    ierr = PetscObjectGetComm((PetscObject)ts,&comm);CHKERRQ(ierr);
    ierr = MPI_Comm_rank(comm,&rank);CHKERRQ(ierr);
    if (!rank) {
      ierr = PetscViewerBinaryWrite(viewer,&classid,1,PETSC_INT,PETSC_FALSE);CHKERRQ(ierr);
      ierr = PetscStrncpy(type,((PetscObject)ts)->type_name,256);CHKERRQ(ierr);
      ierr = PetscViewerBinaryWrite(viewer,type,256,PETSC_CHAR,PETSC_FALSE);CHKERRQ(ierr);
    }
    if (ts->ops->view) {
      ierr = (*ts->ops->view)(ts,viewer);CHKERRQ(ierr);
    }
    ierr = DMView(ts->dm,viewer);CHKERRQ(ierr);
    ierr = VecView(ts->vec_sol,viewer);CHKERRQ(ierr);
    ierr = DMGetDMTS(ts->dm,&sdm);CHKERRQ(ierr);
    ierr = DMTSView(sdm,viewer);CHKERRQ(ierr);
  } else if (isdraw) {
    PetscDraw draw;
    char      str[36];
    PetscReal x,y,bottom,h;

    ierr   = PetscViewerDrawGetDraw(viewer,0,&draw);CHKERRQ(ierr);
    ierr   = PetscDrawGetCurrentPoint(draw,&x,&y);CHKERRQ(ierr);
    ierr   = PetscStrcpy(str,"TS: ");CHKERRQ(ierr);
    ierr   = PetscStrcat(str,((PetscObject)ts)->type_name);CHKERRQ(ierr);
    ierr   = PetscDrawBoxedString(draw,x,y,PETSC_DRAW_BLACK,PETSC_DRAW_BLACK,str,NULL,&h);CHKERRQ(ierr);
    bottom = y - h;
    ierr   = PetscDrawPushCurrentPoint(draw,x,bottom);CHKERRQ(ierr);
    if (ts->ops->view) {
      ierr = (*ts->ops->view)(ts,viewer);CHKERRQ(ierr);
    }
    ierr = PetscDrawPopCurrentPoint(draw);CHKERRQ(ierr);
#if defined(PETSC_HAVE_AMS)
  } else if (isams) {
    if (((PetscObject)ts)->amsmem == -1) {
      ierr = PetscObjectViewAMS((PetscObject)ts,viewer);CHKERRQ(ierr);
      PetscStackCallAMS(AMS_Memory_take_access,(((PetscObject)ts)->amsmem));
      PetscStackCallAMS(AMS_Memory_add_field,(((PetscObject)ts)->amsmem,"time step",&ts->steps,1,AMS_INT,AMS_READ,AMS_COMMON,AMS_REDUCT_UNDEF));
      PetscStackCallAMS(AMS_Memory_add_field,(((PetscObject)ts)->amsmem,"time",&ts->ptime,1,AMS_DOUBLE,AMS_READ,AMS_COMMON,AMS_REDUCT_UNDEF));
      PetscStackCallAMS(AMS_Memory_grant_access,(((PetscObject)ts)->amsmem));
    }
    if (ts->ops->view) {
      ierr = (*ts->ops->view)(ts,viewer);CHKERRQ(ierr);
    }
#endif
  }

  ierr = PetscViewerASCIIPushTab(viewer);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)ts,TSSUNDIALS,&isundials);CHKERRQ(ierr);
  ierr = PetscViewerASCIIPopTab(viewer);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSSetApplicationContext"
/*@
   TSSetApplicationContext - Sets an optional user-defined context for
   the timesteppers.

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
-  usrP - optional user context

   Level: intermediate

.keywords: TS, timestep, set, application, context

.seealso: TSGetApplicationContext()
@*/
PetscErrorCode  TSSetApplicationContext(TS ts,void *usrP)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->user = usrP;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetApplicationContext"
/*@
    TSGetApplicationContext - Gets the user-defined context for the
    timestepper.

    Not Collective

    Input Parameter:
.   ts - the TS context obtained from TSCreate()

    Output Parameter:
.   usrP - user context

    Level: intermediate

.keywords: TS, timestep, get, application, context

.seealso: TSSetApplicationContext()
@*/
PetscErrorCode  TSGetApplicationContext(TS ts,void *usrP)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  *(void**)usrP = ts->user;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetTimeStepNumber"
/*@
   TSGetTimeStepNumber - Gets the number of time steps completed.

   Not Collective

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Output Parameter:
.  iter - number of steps completed so far

   Level: intermediate

.keywords: TS, timestep, get, iteration, number
.seealso: TSGetTime(), TSGetTimeStep(), TSSetPreStep(), TSSetPreStage(), TSSetPostStep()
@*/
PetscErrorCode  TSGetTimeStepNumber(TS ts,PetscInt *iter)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidIntPointer(iter,2);
  *iter = ts->steps;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetInitialTimeStep"
/*@
   TSSetInitialTimeStep - Sets the initial timestep to be used,
   as well as the initial time.

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
.  initial_time - the initial time
-  time_step - the size of the timestep

   Level: intermediate

.seealso: TSSetTimeStep(), TSGetTimeStep()

.keywords: TS, set, initial, timestep
@*/
PetscErrorCode  TSSetInitialTimeStep(TS ts,PetscReal initial_time,PetscReal time_step)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = TSSetTimeStep(ts,time_step);CHKERRQ(ierr);
  ierr = TSSetTime(ts,initial_time);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetTimeStep"
/*@
   TSSetTimeStep - Allows one to reset the timestep at any time,
   useful for simple pseudo-timestepping codes.

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
-  time_step - the size of the timestep

   Level: intermediate

.seealso: TSSetInitialTimeStep(), TSGetTimeStep()

.keywords: TS, set, timestep
@*/
PetscErrorCode  TSSetTimeStep(TS ts,PetscReal time_step)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidLogicalCollectiveReal(ts,time_step,2);
  ts->time_step      = time_step;
  ts->time_step_orig = time_step;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetExactFinalTime"
/*@
   TSSetExactFinalTime - Determines whether to adapt the final time step to
     match the exact final time, interpolate solution to the exact final time,
     or just return at the final time TS computed.

  Logically Collective on TS

   Input Parameter:
+   ts - the time-step context
-   eftopt - exact final time option

   Level: beginner

.seealso: TSExactFinalTimeOption
@*/
PetscErrorCode  TSSetExactFinalTime(TS ts,TSExactFinalTimeOption eftopt)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidLogicalCollectiveEnum(ts,eftopt,2);
  ts->exact_final_time = eftopt;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetTimeStep"
/*@
   TSGetTimeStep - Gets the current timestep size.

   Not Collective

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Output Parameter:
.  dt - the current timestep size

   Level: intermediate

.seealso: TSSetInitialTimeStep(), TSGetTimeStep()

.keywords: TS, get, timestep
@*/
PetscErrorCode  TSGetTimeStep(TS ts,PetscReal *dt)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidRealPointer(dt,2);
  *dt = ts->time_step;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetSolution"
/*@
   TSGetSolution - Returns the solution at the present timestep. It
   is valid to call this routine inside the function that you are evaluating
   in order to move to the new timestep. This vector not changed until
   the solution at the next timestep has been calculated.

   Not Collective, but Vec returned is parallel if TS is parallel

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Output Parameter:
.  v - the vector containing the solution

   Level: intermediate

.seealso: TSGetTimeStep()

.keywords: TS, timestep, get, solution
@*/
PetscErrorCode  TSGetSolution(TS ts,Vec *v)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(v,2);
  *v = ts->vec_sol;
  PetscFunctionReturn(0);
}

/* ----- Routines to initialize and destroy a timestepper ---- */
#undef __FUNCT__
#define __FUNCT__ "TSSetProblemType"
/*@
  TSSetProblemType - Sets the type of problem to be solved.

  Not collective

  Input Parameters:
+ ts   - The TS
- type - One of TS_LINEAR, TS_NONLINEAR where these types refer to problems of the forms
.vb
         U_t - A U = 0      (linear)
         U_t - A(t) U = 0   (linear)
         F(t,U,U_t) = 0     (nonlinear)
.ve

   Level: beginner

.keywords: TS, problem type
.seealso: TSSetUp(), TSProblemType, TS
@*/
PetscErrorCode  TSSetProblemType(TS ts, TSProblemType type)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  ts->problem_type = type;
  if (type == TS_LINEAR) {
    SNES snes;
    ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
    ierr = SNESSetType(snes,SNESKSPONLY);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetProblemType"
/*@C
  TSGetProblemType - Gets the type of problem to be solved.

  Not collective

  Input Parameter:
. ts   - The TS

  Output Parameter:
. type - One of TS_LINEAR, TS_NONLINEAR where these types refer to problems of the forms
.vb
         M U_t = A U
         M(t) U_t = A(t) U
         F(t,U,U_t)
.ve

   Level: beginner

.keywords: TS, problem type
.seealso: TSSetUp(), TSProblemType, TS
@*/
PetscErrorCode  TSGetProblemType(TS ts, TSProblemType *type)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  PetscValidIntPointer(type,2);
  *type = ts->problem_type;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetUp"
/*@
   TSSetUp - Sets up the internal data structures for the later use
   of a timestepper.

   Collective on TS

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Notes:
   For basic use of the TS solvers the user need not explicitly call
   TSSetUp(), since these actions will automatically occur during
   the call to TSStep().  However, if one wishes to control this
   phase separately, TSSetUp() should be called after TSCreate()
   and optional routines of the form TSSetXXX(), but before TSStep().

   Level: advanced

.keywords: TS, timestep, setup

.seealso: TSCreate(), TSStep(), TSDestroy()
@*/
PetscErrorCode  TSSetUp(TS ts)
{
  PetscErrorCode ierr;
  DM             dm;
  PetscErrorCode (*func)(SNES,Vec,Vec,void*);
  PetscErrorCode (*jac)(SNES,Vec,Mat*,Mat*,MatStructure*,void*);
  TSIJacobian    ijac;
  TSRHSJacobian  rhsjac;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (ts->setupcalled) PetscFunctionReturn(0);

  if (!((PetscObject)ts)->type_name) {
    ierr = TSSetType(ts,TSEULER);CHKERRQ(ierr);
  }

  if (!ts->vec_sol) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONGSTATE,"Must call TSSetSolution() first");

  ierr = TSGetAdapt(ts,&ts->adapt);CHKERRQ(ierr);

  if (ts->ops->setup) {
    ierr = (*ts->ops->setup)(ts);CHKERRQ(ierr);
  }

  /* in the case where we've set a DMTSFunction or what have you, we need the default SNESFunction
   to be set right but can't do it elsewhere due to the overreliance on ctx=ts.
   */
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMSNESGetFunction(dm,&func,NULL);CHKERRQ(ierr);
  if (!func) {
    ierr =DMSNESSetFunction(dm,SNESTSFormFunction,ts);CHKERRQ(ierr);
  }
  /* if the SNES doesn't have a jacobian set and the TS has an ijacobian or rhsjacobian set, set the SNES to use it.
     Otherwise, the SNES will use coloring internally to form the Jacobian.
   */
  ierr = DMSNESGetJacobian(dm,&jac,NULL);CHKERRQ(ierr);
  ierr = DMTSGetIJacobian(dm,&ijac,NULL);CHKERRQ(ierr);
  ierr = DMTSGetRHSJacobian(dm,&rhsjac,NULL);CHKERRQ(ierr);
  if (!jac && (ijac || rhsjac)) {
    ierr = DMSNESSetJacobian(dm,SNESTSFormJacobian,ts);CHKERRQ(ierr);
  }
  ts->setupcalled = PETSC_TRUE;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSReset"
/*@
   TSReset - Resets a TS context and removes any allocated Vecs and Mats.

   Collective on TS

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Level: beginner

.keywords: TS, timestep, reset

.seealso: TSCreate(), TSSetup(), TSDestroy()
@*/
PetscErrorCode  TSReset(TS ts)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (ts->ops->reset) {
    ierr = (*ts->ops->reset)(ts);CHKERRQ(ierr);
  }
  if (ts->snes) {ierr = SNESReset(ts->snes);CHKERRQ(ierr);}

  ierr = MatDestroy(&ts->Arhs);CHKERRQ(ierr);
  ierr = MatDestroy(&ts->Brhs);CHKERRQ(ierr);
  ierr = VecDestroy(&ts->Frhs);CHKERRQ(ierr);
  ierr = VecDestroy(&ts->vec_sol);CHKERRQ(ierr);
  ierr = VecDestroy(&ts->vatol);CHKERRQ(ierr);
  ierr = VecDestroy(&ts->vrtol);CHKERRQ(ierr);
  ierr = VecDestroyVecs(ts->nwork,&ts->work);CHKERRQ(ierr);

  ts->setupcalled = PETSC_FALSE;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSDestroy"
/*@
   TSDestroy - Destroys the timestepper context that was created
   with TSCreate().

   Collective on TS

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Level: beginner

.keywords: TS, timestepper, destroy

.seealso: TSCreate(), TSSetUp(), TSSolve()
@*/
PetscErrorCode  TSDestroy(TS *ts)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (!*ts) PetscFunctionReturn(0);
  PetscValidHeaderSpecific((*ts),TS_CLASSID,1);
  if (--((PetscObject)(*ts))->refct > 0) {*ts = 0; PetscFunctionReturn(0);}

  ierr = TSReset((*ts));CHKERRQ(ierr);

  /* if memory was published with AMS then destroy it */
  ierr = PetscObjectAMSViewOff((PetscObject)*ts);CHKERRQ(ierr);
  if ((*ts)->ops->destroy) {ierr = (*(*ts)->ops->destroy)((*ts));CHKERRQ(ierr);}

  ierr = TSAdaptDestroy(&(*ts)->adapt);CHKERRQ(ierr);
  ierr = SNESDestroy(&(*ts)->snes);CHKERRQ(ierr);
  ierr = DMDestroy(&(*ts)->dm);CHKERRQ(ierr);
  ierr = TSMonitorCancel((*ts));CHKERRQ(ierr);

  ierr = PetscHeaderDestroy(ts);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetSNES"
/*@
   TSGetSNES - Returns the SNES (nonlinear solver) associated with
   a TS (timestepper) context. Valid only for nonlinear problems.

   Not Collective, but SNES is parallel if TS is parallel

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Output Parameter:
.  snes - the nonlinear solver context

   Notes:
   The user can then directly manipulate the SNES context to set various
   options, etc.  Likewise, the user can then extract and manipulate the
   KSP, KSP, and PC contexts as well.

   TSGetSNES() does not work for integrators that do not use SNES; in
   this case TSGetSNES() returns NULL in snes.

   Level: beginner

.keywords: timestep, get, SNES
@*/
PetscErrorCode  TSGetSNES(TS ts,SNES *snes)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(snes,2);
  if (!ts->snes) {
    ierr = SNESCreate(PetscObjectComm((PetscObject)ts),&ts->snes);CHKERRQ(ierr);
    ierr = SNESSetFunction(ts->snes,NULL,SNESTSFormFunction,ts);CHKERRQ(ierr);
    ierr = PetscLogObjectParent(ts,ts->snes);CHKERRQ(ierr);
    ierr = PetscObjectIncrementTabLevel((PetscObject)ts->snes,(PetscObject)ts,1);CHKERRQ(ierr);
    if (ts->dm) {ierr = SNESSetDM(ts->snes,ts->dm);CHKERRQ(ierr);}
    if (ts->problem_type == TS_LINEAR) {
      ierr = SNESSetType(ts->snes,SNESKSPONLY);CHKERRQ(ierr);
    }
  }
  *snes = ts->snes;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetSNES"
/*@
   TSSetSNES - Set the SNES (nonlinear solver) to be used by the timestepping context

   Collective

   Input Parameter:
+  ts - the TS context obtained from TSCreate()
-  snes - the nonlinear solver context

   Notes:
   Most users should have the TS created by calling TSGetSNES()

   Level: developer

.keywords: timestep, set, SNES
@*/
PetscErrorCode TSSetSNES(TS ts,SNES snes)
{
  PetscErrorCode ierr;
  PetscErrorCode (*func)(SNES,Vec,Mat*,Mat*,MatStructure*,void*);

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(snes,SNES_CLASSID,2);
  ierr = PetscObjectReference((PetscObject)snes);CHKERRQ(ierr);
  ierr = SNESDestroy(&ts->snes);CHKERRQ(ierr);

  ts->snes = snes;

  ierr = SNESSetFunction(ts->snes,NULL,SNESTSFormFunction,ts);CHKERRQ(ierr);
  ierr = SNESGetJacobian(ts->snes,NULL,NULL,&func,NULL);CHKERRQ(ierr);
  if (func == SNESTSFormJacobian) {
    ierr = SNESSetJacobian(ts->snes,NULL,NULL,SNESTSFormJacobian,ts);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetKSP"
/*@
   TSGetKSP - Returns the KSP (linear solver) associated with
   a TS (timestepper) context.

   Not Collective, but KSP is parallel if TS is parallel

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Output Parameter:
.  ksp - the nonlinear solver context

   Notes:
   The user can then directly manipulate the KSP context to set various
   options, etc.  Likewise, the user can then extract and manipulate the
   KSP and PC contexts as well.

   TSGetKSP() does not work for integrators that do not use KSP;
   in this case TSGetKSP() returns NULL in ksp.

   Level: beginner

.keywords: timestep, get, KSP
@*/
PetscErrorCode  TSGetKSP(TS ts,KSP *ksp)
{
  PetscErrorCode ierr;
  SNES           snes;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(ksp,2);
  if (!((PetscObject)ts)->type_name) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_NULL,"KSP is not created yet. Call TSSetType() first");
  if (ts->problem_type != TS_LINEAR) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONG,"Linear only; use TSGetSNES()");
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESGetKSP(snes,ksp);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

/* ----------- Routines to set solver parameters ---------- */

#undef __FUNCT__
#define __FUNCT__ "TSGetDuration"
/*@
   TSGetDuration - Gets the maximum number of timesteps to use and
   maximum time for iteration.

   Not Collective

   Input Parameters:
+  ts       - the TS context obtained from TSCreate()
.  maxsteps - maximum number of iterations to use, or NULL
-  maxtime  - final time to iterate to, or NULL

   Level: intermediate

.keywords: TS, timestep, get, maximum, iterations, time
@*/
PetscErrorCode  TSGetDuration(TS ts, PetscInt *maxsteps, PetscReal *maxtime)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  if (maxsteps) {
    PetscValidIntPointer(maxsteps,2);
    *maxsteps = ts->max_steps;
  }
  if (maxtime) {
    PetscValidScalarPointer(maxtime,3);
    *maxtime = ts->max_time;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetDuration"
/*@
   TSSetDuration - Sets the maximum number of timesteps to use and
   maximum time for iteration.

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
.  maxsteps - maximum number of iterations to use
-  maxtime - final time to iterate to

   Options Database Keys:
.  -ts_max_steps <maxsteps> - Sets maxsteps
.  -ts_final_time <maxtime> - Sets maxtime

   Notes:
   The default maximum number of iterations is 5000. Default time is 5.0

   Level: intermediate

.keywords: TS, timestep, set, maximum, iterations

.seealso: TSSetExactFinalTime()
@*/
PetscErrorCode  TSSetDuration(TS ts,PetscInt maxsteps,PetscReal maxtime)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidLogicalCollectiveInt(ts,maxsteps,2);
  PetscValidLogicalCollectiveReal(ts,maxtime,2);
  if (maxsteps >= 0) ts->max_steps = maxsteps;
  if (maxtime != PETSC_DEFAULT) ts->max_time = maxtime;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetSolution"
/*@
   TSSetSolution - Sets the initial solution vector
   for use by the TS routines.

   Logically Collective on TS and Vec

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
-  u - the solution vector

   Level: beginner

.keywords: TS, timestep, set, solution, initial conditions
@*/
PetscErrorCode  TSSetSolution(TS ts,Vec u)
{
  PetscErrorCode ierr;
  DM             dm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(u,VEC_CLASSID,2);
  ierr = PetscObjectReference((PetscObject)u);CHKERRQ(ierr);
  ierr = VecDestroy(&ts->vec_sol);CHKERRQ(ierr);

  ts->vec_sol = u;

  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMShellSetGlobalVector(dm,u);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetPreStep"
/*@C
  TSSetPreStep - Sets the general-purpose function
  called once at the beginning of each time step.

  Logically Collective on TS

  Input Parameters:
+ ts   - The TS context obtained from TSCreate()
- func - The function

  Calling sequence of func:
. func (TS ts);

  Level: intermediate

  Note:
  If a step is rejected, TSStep() will call this routine again before each attempt.
  The last completed time step number can be queried using TSGetTimeStepNumber(), the
  size of the step being attempted can be obtained using TSGetTimeStep().

.keywords: TS, timestep
.seealso: TSSetPreStage(), TSSetPostStep(), TSStep()
@*/
PetscErrorCode  TSSetPreStep(TS ts, PetscErrorCode (*func)(TS))
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  ts->prestep = func;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSPreStep"
/*@
  TSPreStep - Runs the user-defined pre-step function.

  Collective on TS

  Input Parameters:
. ts   - The TS context obtained from TSCreate()

  Notes:
  TSPreStep() is typically used within time stepping implementations,
  so most users would not generally call this routine themselves.

  Level: developer

.keywords: TS, timestep
.seealso: TSSetPreStep(), TSPreStage(), TSPostStep()
@*/
PetscErrorCode  TSPreStep(TS ts)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (ts->prestep) {
    PetscStackCallStandard((*ts->prestep),(ts));
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetPreStage"
/*@C
  TSSetPreStage - Sets the general-purpose function
  called once at the beginning of each stage.

  Logically Collective on TS

  Input Parameters:
+ ts   - The TS context obtained from TSCreate()
- func - The function

  Calling sequence of func:
. PetscErrorCode func(TS ts, PetscReal stagetime);

  Level: intermediate

  Note:
  There may be several stages per time step. If the solve for a given stage fails, the step may be rejected and retried.
  The time step number being computed can be queried using TSGetTimeStepNumber() and the total size of the step being
  attempted can be obtained using TSGetTimeStep(). The time at the start of the step is available via TSGetTime().

.keywords: TS, timestep
.seealso: TSSetPreStep(), TSSetPostStep(), TSGetApplicationContext()
@*/
PetscErrorCode  TSSetPreStage(TS ts, PetscErrorCode (*func)(TS,PetscReal))
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  ts->prestage = func;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSPreStage"
/*@
  TSPreStage - Runs the user-defined pre-stage function set using TSSetPreStage()

  Collective on TS

  Input Parameters:
. ts   - The TS context obtained from TSCreate()

  Notes:
  TSPreStage() is typically used within time stepping implementations,
  most users would not generally call this routine themselves.

  Level: developer

.keywords: TS, timestep
.seealso: TSSetPreStep(), TSPreStep(), TSPostStep()
@*/
PetscErrorCode  TSPreStage(TS ts, PetscReal stagetime)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (ts->prestage) {
    PetscStackCallStandard((*ts->prestage),(ts,stagetime));
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetPostStep"
/*@C
  TSSetPostStep - Sets the general-purpose function
  called once at the end of each time step.

  Logically Collective on TS

  Input Parameters:
+ ts   - The TS context obtained from TSCreate()
- func - The function

  Calling sequence of func:
$ func (TS ts);

  Level: intermediate

.keywords: TS, timestep
.seealso: TSSetPreStep(), TSSetPreStage(), TSGetTimeStep(), TSGetTimeStepNumber(), TSGetTime()
@*/
PetscErrorCode  TSSetPostStep(TS ts, PetscErrorCode (*func)(TS))
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  ts->poststep = func;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSPostStep"
/*@
  TSPostStep - Runs the user-defined post-step function.

  Collective on TS

  Input Parameters:
. ts   - The TS context obtained from TSCreate()

  Notes:
  TSPostStep() is typically used within time stepping implementations,
  so most users would not generally call this routine themselves.

  Level: developer

.keywords: TS, timestep
@*/
PetscErrorCode  TSPostStep(TS ts)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (ts->poststep) {
    PetscStackCallStandard((*ts->poststep),(ts));
  }
  PetscFunctionReturn(0);
}

/* ------------ Routines to set performance monitoring options ----------- */

#undef __FUNCT__
#define __FUNCT__ "TSMonitorSet"
/*@C
   TSMonitorSet - Sets an ADDITIONAL function that is to be used at every
   timestep to display the iteration's  progress.

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
.  monitor - monitoring routine
.  mctx - [optional] user-defined context for private data for the
             monitor routine (use NULL if no context is desired)
-  monitordestroy - [optional] routine that frees monitor context
          (may be NULL)

   Calling sequence of monitor:
$    int monitor(TS ts,PetscInt steps,PetscReal time,Vec u,void *mctx)

+    ts - the TS context
.    steps - iteration number (after the final time step the monitor routine is called with a step of -1, this is at the final time which may have
                               been interpolated to)
.    time - current time
.    u - current iterate
-    mctx - [optional] monitoring context

   Notes:
   This routine adds an additional monitor to the list of monitors that
   already has been loaded.

   Fortran notes: Only a single monitor function can be set for each TS object

   Level: intermediate

.keywords: TS, timestep, set, monitor

.seealso: TSMonitorDefault(), TSMonitorCancel()
@*/
PetscErrorCode  TSMonitorSet(TS ts,PetscErrorCode (*monitor)(TS,PetscInt,PetscReal,Vec,void*),void *mctx,PetscErrorCode (*mdestroy)(void**))
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (ts->numbermonitors >= MAXTSMONITORS) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_OUTOFRANGE,"Too many monitors set");
  ts->monitor[ts->numbermonitors]          = monitor;
  ts->monitordestroy[ts->numbermonitors]   = mdestroy;
  ts->monitorcontext[ts->numbermonitors++] = (void*)mctx;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorCancel"
/*@C
   TSMonitorCancel - Clears all the monitors that have been set on a time-step object.

   Logically Collective on TS

   Input Parameters:
.  ts - the TS context obtained from TSCreate()

   Notes:
   There is no way to remove a single, specific monitor.

   Level: intermediate

.keywords: TS, timestep, set, monitor

.seealso: TSMonitorDefault(), TSMonitorSet()
@*/
PetscErrorCode  TSMonitorCancel(TS ts)
{
  PetscErrorCode ierr;
  PetscInt       i;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  for (i=0; i<ts->numbermonitors; i++) {
    if (ts->monitordestroy[i]) {
      ierr = (*ts->monitordestroy[i])(&ts->monitorcontext[i]);CHKERRQ(ierr);
    }
  }
  ts->numbermonitors = 0;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorDefault"
/*@
   TSMonitorDefault - Sets the Default monitor

   Level: intermediate

.keywords: TS, set, monitor

.seealso: TSMonitorDefault(), TSMonitorSet()
@*/
PetscErrorCode TSMonitorDefault(TS ts,PetscInt step,PetscReal ptime,Vec v,void *dummy)
{
  PetscErrorCode ierr;
  PetscViewer    viewer = dummy ? (PetscViewer) dummy : PETSC_VIEWER_STDOUT_(PetscObjectComm((PetscObject)ts));

  PetscFunctionBegin;
  ierr = PetscViewerASCIIAddTab(viewer,((PetscObject)ts)->tablevel);CHKERRQ(ierr);
  ierr = PetscViewerASCIIPrintf(viewer,"%D TS dt %g time %g\n",step,(double)ts->time_step,(double)ptime);CHKERRQ(ierr);
  ierr = PetscViewerASCIISubtractTab(viewer,((PetscObject)ts)->tablevel);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetRetainStages"
/*@
   TSSetRetainStages - Request that all stages in the upcoming step be stored so that interpolation will be available.

   Logically Collective on TS

   Input Argument:
.  ts - time stepping context

   Output Argument:
.  flg - PETSC_TRUE or PETSC_FALSE

   Level: intermediate

.keywords: TS, set

.seealso: TSInterpolate(), TSSetPostStep()
@*/
PetscErrorCode TSSetRetainStages(TS ts,PetscBool flg)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->retain_stages = flg;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSInterpolate"
/*@
   TSInterpolate - Interpolate the solution computed during the previous step to an arbitrary location in the interval

   Collective on TS

   Input Argument:
+  ts - time stepping context
-  t - time to interpolate to

   Output Argument:
.  U - state at given time

   Notes:
   The user should call TSSetRetainStages() before taking a step in which interpolation will be requested.

   Level: intermediate

   Developer Notes:
   TSInterpolate() and the storing of previous steps/stages should be generalized to support delay differential equations and continuous adjoints.

.keywords: TS, set

.seealso: TSSetRetainStages(), TSSetPostStep()
@*/
PetscErrorCode TSInterpolate(TS ts,PetscReal t,Vec U)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  if (t < ts->ptime - ts->time_step_prev || t > ts->ptime) SETERRQ3(PetscObjectComm((PetscObject)ts),PETSC_ERR_ARG_OUTOFRANGE,"Requested time %G not in last time steps [%G,%G]",t,ts->ptime-ts->time_step_prev,ts->ptime);
  if (!ts->ops->interpolate) SETERRQ1(PetscObjectComm((PetscObject)ts),PETSC_ERR_SUP,"%s does not provide interpolation",((PetscObject)ts)->type_name);
  ierr = (*ts->ops->interpolate)(ts,t,U);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSStep"
/*@
   TSStep - Steps one time step

   Collective on TS

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Level: intermediate

   Notes:
   The hook set using TSSetPreStep() is called before each attempt to take the step. In general, the time step size may
   be changed due to adaptive error controller or solve failures. Note that steps may contain multiple stages.

   This may over-step the final time provided in TSSetDuration() depending on the time-step used. TSSolve() interpolates to exactly the
   time provided in TSSetDuration(). One can use TSInterpolate() to determine an interpolated solution within the final timestep.

.keywords: TS, timestep, solve

.seealso: TSCreate(), TSSetUp(), TSDestroy(), TSSolve(), TSSetPreStep(), TSSetPreStage(), TSInterpolate()
@*/
PetscErrorCode  TSStep(TS ts)
{
  PetscReal      ptime_prev;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts, TS_CLASSID,1);
  ierr = TSSetUp(ts);CHKERRQ(ierr);

  ts->reason = TS_CONVERGED_ITERATING;
  ptime_prev = ts->ptime;

  ierr = PetscLogEventBegin(TS_Step,ts,0,0,0);CHKERRQ(ierr);
  ierr = (*ts->ops->step)(ts);CHKERRQ(ierr);
  ierr = PetscLogEventEnd(TS_Step,ts,0,0,0);CHKERRQ(ierr);

  ts->time_step_prev = ts->ptime - ptime_prev;

  if (ts->reason < 0) {
    if (ts->errorifstepfailed) {
      if (ts->reason == TS_DIVERGED_NONLINEAR_SOLVE) {
        SETERRQ1(PetscObjectComm((PetscObject)ts),PETSC_ERR_NOT_CONVERGED,"TSStep has failed due to %s, increase -ts_max_snes_failures or make negative to attempt recovery",TSConvergedReasons[ts->reason]);
      } else SETERRQ1(PetscObjectComm((PetscObject)ts),PETSC_ERR_NOT_CONVERGED,"TSStep has failed due to %s",TSConvergedReasons[ts->reason]);
    }
  } else if (!ts->reason) {
    if (ts->steps >= ts->max_steps)     ts->reason = TS_CONVERGED_ITS;
    else if (ts->ptime >= ts->max_time) ts->reason = TS_CONVERGED_TIME;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSEvaluateStep"
/*@
   TSEvaluateStep - Evaluate the solution at the end of a time step with a given order of accuracy.

   Collective on TS

   Input Arguments:
+  ts - time stepping context
.  order - desired order of accuracy
-  done - whether the step was evaluated at this order (pass NULL to generate an error if not available)

   Output Arguments:
.  U - state at the end of the current step

   Level: advanced

   Notes:
   This function cannot be called until all stages have been evaluated.
   It is normally called by adaptive controllers before a step has been accepted and may also be called by the user after TSStep() has returned.

.seealso: TSStep(), TSAdapt
@*/
PetscErrorCode TSEvaluateStep(TS ts,PetscInt order,Vec U,PetscBool *done)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidType(ts,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,3);
  if (!ts->ops->evaluatestep) SETERRQ1(PetscObjectComm((PetscObject)ts),PETSC_ERR_SUP,"TSEvaluateStep not implemented for type '%s'",((PetscObject)ts)->type_name);
  ierr = (*ts->ops->evaluatestep)(ts,order,U,done);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSolve"
/*@
   TSSolve - Steps the requested number of timesteps.

   Collective on TS

   Input Parameter:
+  ts - the TS context obtained from TSCreate()
-  u - the solution vector  (can be null if TSSetSolution() was used, otherwise must contain the initial conditions)

   Level: beginner

   Notes:
   The final time returned by this function may be different from the time of the internally
   held state accessible by TSGetSolution() and TSGetTime() because the method may have
   stepped over the final time.

.keywords: TS, timestep, solve

.seealso: TSCreate(), TSSetSolution(), TSStep()
@*/
PetscErrorCode TSSolve(TS ts,Vec u)
{
  PetscBool         flg;
  PetscViewer       viewer;
  Vec               solution;
  PetscErrorCode    ierr;
  PetscViewerFormat format;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (u) PetscValidHeaderSpecific(u,VEC_CLASSID,2);
  if (ts->exact_final_time == TS_EXACTFINALTIME_INTERPOLATE) {   /* Need ts->vec_sol to be distinct so it is not overwritten when we interpolate at the end */
    PetscValidHeaderSpecific(u,VEC_CLASSID,2);
    if (!ts->vec_sol || u == ts->vec_sol) {
      ierr = VecDuplicate(u,&solution);CHKERRQ(ierr);
      ierr = TSSetSolution(ts,solution);CHKERRQ(ierr);
      ierr = VecDestroy(&solution);CHKERRQ(ierr); /* grant ownership */
    }
    ierr = VecCopy(u,ts->vec_sol);CHKERRQ(ierr);
  } else if (u) {
    ierr = TSSetSolution(ts,u);CHKERRQ(ierr);
  }
  ierr = TSSetUp(ts);CHKERRQ(ierr);
  /* reset time step and iteration counters */
  ts->steps             = 0;
  ts->ksp_its           = 0;
  ts->snes_its          = 0;
  ts->num_snes_failures = 0;
  ts->reject            = 0;
  ts->reason            = TS_CONVERGED_ITERATING;

  ierr = PetscOptionsGetViewer(PetscObjectComm((PetscObject)ts),((PetscObject)ts)->prefix,"-ts_view_pre",&viewer,&format,&flg);CHKERRQ(ierr);
  if (flg && !PetscPreLoadingOn) {
    ierr = PetscViewerPushFormat(viewer,format);CHKERRQ(ierr);
    ierr = TSView(ts,viewer);CHKERRQ(ierr);
    ierr = PetscViewerPopFormat(viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);CHKERRQ(ierr);
  }

  if (ts->ops->solve) {         /* This private interface is transitional and should be removed when all implementations are updated. */
    ierr = (*ts->ops->solve)(ts);CHKERRQ(ierr);
    ierr = VecCopy(ts->vec_sol,u);CHKERRQ(ierr);
    ts->solvetime = ts->ptime;
  } else {
    /* steps the requested number of timesteps. */
    if (ts->steps >= ts->max_steps)     ts->reason = TS_CONVERGED_ITS;
    else if (ts->ptime >= ts->max_time) ts->reason = TS_CONVERGED_TIME;
    while (!ts->reason) {
      ierr = TSMonitor(ts,ts->steps,ts->ptime,ts->vec_sol);CHKERRQ(ierr);
      ierr = TSStep(ts);CHKERRQ(ierr);
      ierr = TSPostStep(ts);CHKERRQ(ierr);
    }
    if (ts->exact_final_time == TS_EXACTFINALTIME_INTERPOLATE && ts->ptime > ts->max_time) {
      ierr = TSInterpolate(ts,ts->max_time,u);CHKERRQ(ierr);
      ts->solvetime = ts->max_time;
      solution = u;
    } else {
      if (u) {ierr = VecCopy(ts->vec_sol,u);CHKERRQ(ierr);}
      ts->solvetime = ts->ptime;
      solution = ts->vec_sol;
    }
    ierr = TSMonitor(ts,ts->steps,ts->solvetime,solution);CHKERRQ(ierr);
  }
  ierr = PetscOptionsGetViewer(PetscObjectComm((PetscObject)ts),((PetscObject)ts)->prefix,"-ts_view",&viewer,&format,&flg);CHKERRQ(ierr);
  if (flg && !PetscPreLoadingOn) {
    ierr = PetscViewerPushFormat(viewer,format);CHKERRQ(ierr);
    ierr = TSView(ts,viewer);CHKERRQ(ierr);
    ierr = PetscViewerPopFormat(viewer);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&viewer);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitor"
/*@
   TSMonitor - Runs all user-provided monitor routines set using TSMonitorSet()

   Collective on TS

   Input Parameters:
+  ts - time stepping context obtained from TSCreate()
.  step - step number that has just completed
.  ptime - model time of the state
-  u - state at the current model time

   Notes:
   TSMonitor() is typically used within the time stepping implementations.
   Users might call this function when using the TSStep() interface instead of TSSolve().

   Level: advanced

.keywords: TS, timestep
@*/
PetscErrorCode TSMonitor(TS ts,PetscInt step,PetscReal ptime,Vec u)
{
  PetscErrorCode ierr;
  PetscInt       i,n = ts->numbermonitors;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(u,VEC_CLASSID,4);
  for (i=0; i<n; i++) {
    ierr = (*ts->monitor[i])(ts,step,ptime,u,ts->monitorcontext[i]);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

/* ------------------------------------------------------------------------*/
struct _n_TSMonitorLGCtx {
  PetscDrawLG lg;
  PetscInt    howoften;  /* when > 0 uses step % howoften, when negative only final solution plotted */
  PetscInt    ksp_its,snes_its;
};


#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGCtxCreate"
/*@C
   TSMonitorLGCtxCreate - Creates a line graph context for use with
   TS to monitor the solution process graphically in various ways

   Collective on TS

   Input Parameters:
+  host - the X display to open, or null for the local machine
.  label - the title to put in the title bar
.  x, y - the screen coordinates of the upper left coordinate of the window
.  m, n - the screen width and height in pixels
-  howoften - if positive then determines the frequency of the plotting, if -1 then only at the final time

   Output Parameter:
.  ctx - the context

   Options Database Key:
+  -ts_monitor_lg_timestep - automatically sets line graph monitor
.  -ts_monitor_lg_solution -
.  -ts_monitor_lg_error -
.  -ts_monitor_lg_ksp_iterations -
.  -ts_monitor_lg_snes_iterations -
-  -lg_indicate_data_points <true,false> - indicate the data points (at each time step) on the plot; default is true

   Notes:
   Use TSMonitorLGCtxDestroy() to destroy.

   Level: intermediate

.keywords: TS, monitor, line graph, residual, seealso

.seealso: TSMonitorLGTimeStep(), TSMonitorSet(), TSMonitorLGSolution(), TSMonitorLGError()

@*/
PetscErrorCode  TSMonitorLGCtxCreate(MPI_Comm comm,const char host[],const char label[],int x,int y,int m,int n,PetscInt howoften,TSMonitorLGCtx *ctx)
{
  PetscDraw      win;
  PetscErrorCode ierr;
  PetscBool      flg = PETSC_TRUE;

  PetscFunctionBegin;
  ierr = PetscNew(struct _n_TSMonitorLGCtx,ctx);CHKERRQ(ierr);
  ierr = PetscDrawCreate(comm,host,label,x,y,m,n,&win);CHKERRQ(ierr);
  ierr = PetscDrawSetFromOptions(win);CHKERRQ(ierr);
  ierr = PetscDrawLGCreate(win,1,&(*ctx)->lg);CHKERRQ(ierr);
  ierr = PetscOptionsGetBool(NULL,"-lg_indicate_data_points",&flg,NULL);CHKERRQ(ierr);
  if (flg) {
    ierr = PetscDrawLGIndicateDataPoints((*ctx)->lg);CHKERRQ(ierr);
  }
  ierr = PetscLogObjectParent((*ctx)->lg,win);CHKERRQ(ierr);
  (*ctx)->howoften = howoften;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGTimeStep"
PetscErrorCode TSMonitorLGTimeStep(TS ts,PetscInt step,PetscReal ptime,Vec v,void *monctx)
{
  TSMonitorLGCtx ctx = (TSMonitorLGCtx) monctx;
  PetscReal      x   = ptime,y;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (!step) {
    PetscDrawAxis axis;
    ierr = PetscDrawLGGetAxis(ctx->lg,&axis);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLabels(axis,"Timestep as function of time","Time","Time step");CHKERRQ(ierr);
    ierr = PetscDrawLGReset(ctx->lg);CHKERRQ(ierr);
  }
  ierr = TSGetTimeStep(ts,&y);CHKERRQ(ierr);
  ierr = PetscDrawLGAddPoint(ctx->lg,&x,&y);CHKERRQ(ierr);
  if (((ctx->howoften > 0) && (!(step % ctx->howoften))) || ((ctx->howoften == -1) && ts->reason)) {
    ierr = PetscDrawLGDraw(ctx->lg);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGCtxDestroy"
/*@C
   TSMonitorLGCtxDestroy - Destroys a line graph context that was created
   with TSMonitorLGCtxCreate().

   Collective on TSMonitorLGCtx

   Input Parameter:
.  ctx - the monitor context

   Level: intermediate

.keywords: TS, monitor, line graph, destroy

.seealso: TSMonitorLGCtxCreate(),  TSMonitorSet(), TSMonitorLGTimeStep();
@*/
PetscErrorCode  TSMonitorLGCtxDestroy(TSMonitorLGCtx *ctx)
{
  PetscDraw      draw;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscDrawLGGetDraw((*ctx)->lg,&draw);CHKERRQ(ierr);
  ierr = PetscDrawDestroy(&draw);CHKERRQ(ierr);
  ierr = PetscDrawLGDestroy(&(*ctx)->lg);CHKERRQ(ierr);
  ierr = PetscFree(*ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetTime"
/*@
   TSGetTime - Gets the time of the most recently completed step.

   Not Collective

   Input Parameter:
.  ts - the TS context obtained from TSCreate()

   Output Parameter:
.  t  - the current time

   Level: beginner

   Note:
   When called during time step evaluation (e.g. during residual evaluation or via hooks set using TSSetPreStep(),
   TSSetPreStage(), or TSSetPostStep()), the time is the time at the start of the step being evaluated.

.seealso: TSSetInitialTimeStep(), TSGetTimeStep()

.keywords: TS, get, time
@*/
PetscErrorCode  TSGetTime(TS ts,PetscReal *t)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidRealPointer(t,2);
  *t = ts->ptime;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetTime"
/*@
   TSSetTime - Allows one to reset the time.

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context obtained from TSCreate()
-  time - the time

   Level: intermediate

.seealso: TSGetTime(), TSSetDuration()

.keywords: TS, set, time
@*/
PetscErrorCode  TSSetTime(TS ts, PetscReal t)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidLogicalCollectiveReal(ts,t,2);
  ts->ptime = t;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetOptionsPrefix"
/*@C
   TSSetOptionsPrefix - Sets the prefix used for searching for all
   TS options in the database.

   Logically Collective on TS

   Input Parameter:
+  ts     - The TS context
-  prefix - The prefix to prepend to all option names

   Notes:
   A hyphen (-) must NOT be given at the beginning of the prefix name.
   The first character of all runtime options is AUTOMATICALLY the
   hyphen.

   Level: advanced

.keywords: TS, set, options, prefix, database

.seealso: TSSetFromOptions()

@*/
PetscErrorCode  TSSetOptionsPrefix(TS ts,const char prefix[])
{
  PetscErrorCode ierr;
  SNES           snes;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = PetscObjectSetOptionsPrefix((PetscObject)ts,prefix);CHKERRQ(ierr);
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESSetOptionsPrefix(snes,prefix);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSAppendOptionsPrefix"
/*@C
   TSAppendOptionsPrefix - Appends to the prefix used for searching for all
   TS options in the database.

   Logically Collective on TS

   Input Parameter:
+  ts     - The TS context
-  prefix - The prefix to prepend to all option names

   Notes:
   A hyphen (-) must NOT be given at the beginning of the prefix name.
   The first character of all runtime options is AUTOMATICALLY the
   hyphen.

   Level: advanced

.keywords: TS, append, options, prefix, database

.seealso: TSGetOptionsPrefix()

@*/
PetscErrorCode  TSAppendOptionsPrefix(TS ts,const char prefix[])
{
  PetscErrorCode ierr;
  SNES           snes;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = PetscObjectAppendOptionsPrefix((PetscObject)ts,prefix);CHKERRQ(ierr);
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESAppendOptionsPrefix(snes,prefix);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetOptionsPrefix"
/*@C
   TSGetOptionsPrefix - Sets the prefix used for searching for all
   TS options in the database.

   Not Collective

   Input Parameter:
.  ts - The TS context

   Output Parameter:
.  prefix - A pointer to the prefix string used

   Notes: On the fortran side, the user should pass in a string 'prifix' of
   sufficient length to hold the prefix.

   Level: intermediate

.keywords: TS, get, options, prefix, database

.seealso: TSAppendOptionsPrefix()
@*/
PetscErrorCode  TSGetOptionsPrefix(TS ts,const char *prefix[])
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(prefix,2);
  ierr = PetscObjectGetOptionsPrefix((PetscObject)ts,prefix);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetRHSJacobian"
/*@C
   TSGetRHSJacobian - Returns the Jacobian J at the present timestep.

   Not Collective, but parallel objects are returned if TS is parallel

   Input Parameter:
.  ts  - The TS context obtained from TSCreate()

   Output Parameters:
+  Amat - The (approximate) Jacobian J of G, where U_t = G(U,t)  (or NULL)
.  Pmat - The matrix from which the preconditioner is constructed, usually the same as Amat  (or NULL)
.  func - Function to compute the Jacobian of the RHS  (or NULL)
-  ctx - User-defined context for Jacobian evaluation routine  (or NULL)

   Notes: You can pass in NULL for any return argument you do not need.

   Level: intermediate

.seealso: TSGetTimeStep(), TSGetMatrices(), TSGetTime(), TSGetTimeStepNumber()

.keywords: TS, timestep, get, matrix, Jacobian
@*/
PetscErrorCode  TSGetRHSJacobian(TS ts,Mat *Amat,Mat *Pmat,TSRHSJacobian *func,void **ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  DM             dm;

  PetscFunctionBegin;
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESGetJacobian(snes,Amat,Pmat,NULL,NULL);CHKERRQ(ierr);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetRHSJacobian(dm,func,ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetIJacobian"
/*@C
   TSGetIJacobian - Returns the implicit Jacobian at the present timestep.

   Not Collective, but parallel objects are returned if TS is parallel

   Input Parameter:
.  ts  - The TS context obtained from TSCreate()

   Output Parameters:
+  Amat  - The (approximate) Jacobian of F(t,U,U_t)
.  Pmat - The matrix from which the preconditioner is constructed, often the same as Amat
.  f   - The function to compute the matrices
- ctx - User-defined context for Jacobian evaluation routine

   Notes: You can pass in NULL for any return argument you do not need.

   Level: advanced

.seealso: TSGetTimeStep(), TSGetRHSJacobian(), TSGetMatrices(), TSGetTime(), TSGetTimeStepNumber()

.keywords: TS, timestep, get, matrix, Jacobian
@*/
PetscErrorCode  TSGetIJacobian(TS ts,Mat *Amat,Mat *Pmat,TSIJacobian *f,void **ctx)
{
  PetscErrorCode ierr;
  SNES           snes;
  DM             dm;

  PetscFunctionBegin;
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESSetUpMatrices(snes);CHKERRQ(ierr);
  ierr = SNESGetJacobian(snes,Amat,Pmat,NULL,NULL);CHKERRQ(ierr);
  ierr = TSGetDM(ts,&dm);CHKERRQ(ierr);
  ierr = DMTSGetIJacobian(dm,f,ctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSMonitorDrawSolution"
/*@C
   TSMonitorDrawSolution - Monitors progress of the TS solvers by calling
   VecView() for the solution at each timestep

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
-  dummy - either a viewer or NULL

   Options Database:
.   -ts_monitor_draw_solution_initial - show initial solution as well as current solution

   Notes: the initial solution and current solution are not displayed with a common axis scaling so generally the option -ts_monitor_draw_solution_initial
       will look bad

   Level: intermediate

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView()
@*/
PetscErrorCode  TSMonitorDrawSolution(TS ts,PetscInt step,PetscReal ptime,Vec u,void *dummy)
{
  PetscErrorCode   ierr;
  TSMonitorDrawCtx ictx = (TSMonitorDrawCtx)dummy;
  PetscDraw        draw;

  PetscFunctionBegin;
  if (!step && ictx->showinitial) {
    if (!ictx->initialsolution) {
      ierr = VecDuplicate(u,&ictx->initialsolution);CHKERRQ(ierr);
    }
    ierr = VecCopy(u,ictx->initialsolution);CHKERRQ(ierr);
  }
  if (!(((ictx->howoften > 0) && (!(step % ictx->howoften))) || ((ictx->howoften == -1) && ts->reason))) PetscFunctionReturn(0);

  if (ictx->showinitial) {
    PetscReal pause;
    ierr = PetscViewerDrawGetPause(ictx->viewer,&pause);CHKERRQ(ierr);
    ierr = PetscViewerDrawSetPause(ictx->viewer,0.0);CHKERRQ(ierr);
    ierr = VecView(ictx->initialsolution,ictx->viewer);CHKERRQ(ierr);
    ierr = PetscViewerDrawSetPause(ictx->viewer,pause);CHKERRQ(ierr);
    ierr = PetscViewerDrawSetHold(ictx->viewer,PETSC_TRUE);CHKERRQ(ierr);
  }
  ierr = VecView(u,ictx->viewer);CHKERRQ(ierr);
  if (ictx->showtimestepandtime) {
    PetscReal xl,yl,xr,yr,tw,w,h;
    char      time[32];
    size_t    len;

    ierr = PetscViewerDrawGetDraw(ictx->viewer,0,&draw);CHKERRQ(ierr);
    ierr = PetscSNPrintf(time,32,"Timestep %d Time %f",(int)step,(double)ptime);CHKERRQ(ierr);
    ierr = PetscDrawGetCoordinates(draw,&xl,&yl,&xr,&yr);CHKERRQ(ierr);
    ierr =  PetscStrlen(time,&len);CHKERRQ(ierr);
    ierr = PetscDrawStringGetSize(draw,&tw,NULL);CHKERRQ(ierr);
    w    = xl + .5*(xr - xl) - .5*len*tw;
    h    = yl + .95*(yr - yl);
    ierr = PetscDrawString(draw,w,h,PETSC_DRAW_BLACK,time);CHKERRQ(ierr);
    ierr = PetscDrawFlush(draw);CHKERRQ(ierr);
  }

  if (ictx->showinitial) {
    ierr = PetscViewerDrawSetHold(ictx->viewer,PETSC_FALSE);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorDrawSolutionPhase"
/*@C
   TSMonitorDrawSolutionPhase - Monitors progress of the TS solvers by plotting the solution as a phase diagram

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
-  dummy - either a viewer or NULL

   Level: intermediate

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView()
@*/
PetscErrorCode  TSMonitorDrawSolutionPhase(TS ts,PetscInt step,PetscReal ptime,Vec u,void *dummy)
{
  PetscErrorCode    ierr;
  TSMonitorDrawCtx  ictx = (TSMonitorDrawCtx)dummy;
  PetscDraw         draw;
  MPI_Comm          comm;
  PetscInt          n;
  PetscMPIInt       size;
  PetscReal         xl,yl,xr,yr,tw,w,h;
  char              time[32];
  size_t            len;
  const PetscScalar *U;

  PetscFunctionBegin;
  ierr = PetscObjectGetComm((PetscObject)ts,&comm);CHKERRQ(ierr);
  ierr = MPI_Comm_size(comm,&size);CHKERRQ(ierr);
  if (size != 1) SETERRQ(comm,PETSC_ERR_SUP,"Only allowed for sequential runs");
  ierr = VecGetSize(u,&n);CHKERRQ(ierr);
  if (n != 2) SETERRQ(comm,PETSC_ERR_SUP,"Only for ODEs with two unknowns");

  ierr = PetscViewerDrawGetDraw(ictx->viewer,0,&draw);CHKERRQ(ierr);

  ierr = VecGetArrayRead(u,&U);CHKERRQ(ierr);
  ierr = PetscDrawAxisGetLimits(ictx->axis,&xl,&xr,&yl,&yr);CHKERRQ(ierr);
  if ((PetscRealPart(U[0]) < xl) || (PetscRealPart(U[1]) < yl) || (PetscRealPart(U[0]) > xr) || (PetscRealPart(U[1]) > yr)) {
      ierr = VecRestoreArrayRead(u,&U);CHKERRQ(ierr);
      PetscFunctionReturn(0);
  }
  if (!step) ictx->color++;
  ierr = PetscDrawPoint(draw,PetscRealPart(U[0]),PetscRealPart(U[1]),ictx->color);CHKERRQ(ierr);
  ierr = VecRestoreArrayRead(u,&U);CHKERRQ(ierr);

  if (ictx->showtimestepandtime) {
    ierr = PetscDrawGetCoordinates(draw,&xl,&yl,&xr,&yr);CHKERRQ(ierr);
    ierr = PetscSNPrintf(time,32,"Timestep %d Time %f",(int)step,(double)ptime);CHKERRQ(ierr);
    ierr = PetscStrlen(time,&len);CHKERRQ(ierr);
    ierr = PetscDrawStringGetSize(draw,&tw,NULL);CHKERRQ(ierr);
    w    = xl + .5*(xr - xl) - .5*len*tw;
    h    = yl + .95*(yr - yl);
    ierr = PetscDrawString(draw,w,h,PETSC_DRAW_BLACK,time);CHKERRQ(ierr);
  }
  ierr = PetscDrawFlush(draw);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSMonitorDrawCtxDestroy"
/*@C
   TSMonitorDrawCtxDestroy - Destroys the monitor context for TSMonitorDrawSolution()

   Collective on TS

   Input Parameters:
.    ctx - the monitor context

   Level: intermediate

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView(), TSMonitorDrawSolution(), TSMonitorDrawError()
@*/
PetscErrorCode  TSMonitorDrawCtxDestroy(TSMonitorDrawCtx *ictx)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscDrawAxisDestroy(&(*ictx)->axis);CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&(*ictx)->viewer);CHKERRQ(ierr);
  ierr = VecDestroy(&(*ictx)->initialsolution);CHKERRQ(ierr);
  ierr = PetscFree(*ictx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorDrawCtxCreate"
/*@C
   TSMonitorDrawCtxCreate - Creates the monitor context for TSMonitorDrawCtx

   Collective on TS

   Input Parameter:
.    ts - time-step context

   Output Patameter:
.    ctx - the monitor context

   Options Database:
.   -ts_monitor_draw_solution_initial - show initial solution as well as current solution

   Level: intermediate

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView(), TSMonitorDrawCtx()
@*/
PetscErrorCode  TSMonitorDrawCtxCreate(MPI_Comm comm,const char host[],const char label[],int x,int y,int m,int n,PetscInt howoften,TSMonitorDrawCtx *ctx)
{
  PetscErrorCode   ierr;

  PetscFunctionBegin;
  ierr = PetscNew(struct _n_TSMonitorDrawCtx,ctx);CHKERRQ(ierr);
  ierr = PetscViewerDrawOpen(comm,host,label,x,y,m,n,&(*ctx)->viewer);CHKERRQ(ierr);
  ierr = PetscViewerSetFromOptions((*ctx)->viewer);CHKERRQ(ierr);

  (*ctx)->howoften    = howoften;
  (*ctx)->showinitial = PETSC_FALSE;
  ierr = PetscOptionsGetBool(NULL,"-ts_monitor_draw_solution_initial",&(*ctx)->showinitial,NULL);CHKERRQ(ierr);

  (*ctx)->showtimestepandtime = PETSC_FALSE;
  ierr = PetscOptionsGetBool(NULL,"-ts_monitor_draw_solution_show_time",&(*ctx)->showtimestepandtime,NULL);CHKERRQ(ierr);
  (*ctx)->color = PETSC_DRAW_WHITE;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorDrawError"
/*@C
   TSMonitorDrawError - Monitors progress of the TS solvers by calling
   VecView() for the error at each timestep

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
-  dummy - either a viewer or NULL

   Level: intermediate

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView()
@*/
PetscErrorCode  TSMonitorDrawError(TS ts,PetscInt step,PetscReal ptime,Vec u,void *dummy)
{
  PetscErrorCode   ierr;
  TSMonitorDrawCtx ctx    = (TSMonitorDrawCtx)dummy;
  PetscViewer      viewer = ctx->viewer;
  Vec              work;

  PetscFunctionBegin;
  if (!(((ctx->howoften > 0) && (!(step % ctx->howoften))) || ((ctx->howoften == -1) && ts->reason))) PetscFunctionReturn(0);
  ierr = VecDuplicate(u,&work);CHKERRQ(ierr);
  ierr = TSComputeSolutionFunction(ts,ptime,work);CHKERRQ(ierr);
  ierr = VecAXPY(work,-1.0,u);CHKERRQ(ierr);
  ierr = VecView(work,viewer);CHKERRQ(ierr);
  ierr = VecDestroy(&work);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#include <petsc-private/dmimpl.h>
#undef __FUNCT__
#define __FUNCT__ "TSSetDM"
/*@
   TSSetDM - Sets the DM that may be used by some preconditioners

   Logically Collective on TS and DM

   Input Parameters:
+  ts - the preconditioner context
-  dm - the dm

   Level: intermediate


.seealso: TSGetDM(), SNESSetDM(), SNESGetDM()
@*/
PetscErrorCode  TSSetDM(TS ts,DM dm)
{
  PetscErrorCode ierr;
  SNES           snes;
  DMTS           tsdm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ierr = PetscObjectReference((PetscObject)dm);CHKERRQ(ierr);
  if (ts->dm) {               /* Move the DMTS context over to the new DM unless the new DM already has one */
    if (ts->dm->dmts && !dm->dmts) {
      ierr = DMCopyDMTS(ts->dm,dm);CHKERRQ(ierr);
      ierr = DMGetDMTS(ts->dm,&tsdm);CHKERRQ(ierr);
      if (tsdm->originaldm == ts->dm) { /* Grant write privileges to the replacement DM */
        tsdm->originaldm = dm;
      }
    }
    ierr = DMDestroy(&ts->dm);CHKERRQ(ierr);
  }
  ts->dm = dm;

  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESSetDM(snes,dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetDM"
/*@
   TSGetDM - Gets the DM that may be used by some preconditioners

   Not Collective

   Input Parameter:
. ts - the preconditioner context

   Output Parameter:
.  dm - the dm

   Level: intermediate


.seealso: TSSetDM(), SNESSetDM(), SNESGetDM()
@*/
PetscErrorCode  TSGetDM(TS ts,DM *dm)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (!ts->dm) {
    ierr = DMShellCreate(PetscObjectComm((PetscObject)ts),&ts->dm);CHKERRQ(ierr);
    if (ts->snes) {ierr = SNESSetDM(ts->snes,ts->dm);CHKERRQ(ierr);}
  }
  *dm = ts->dm;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "SNESTSFormFunction"
/*@
   SNESTSFormFunction - Function to evaluate nonlinear residual

   Logically Collective on SNES

   Input Parameter:
+ snes - nonlinear solver
. U - the current state at which to evaluate the residual
- ctx - user context, must be a TS

   Output Parameter:
. F - the nonlinear residual

   Notes:
   This function is not normally called by users and is automatically registered with the SNES used by TS.
   It is most frequently passed to MatFDColoringSetFunction().

   Level: advanced

.seealso: SNESSetFunction(), MatFDColoringSetFunction()
@*/
PetscErrorCode  SNESTSFormFunction(SNES snes,Vec U,Vec F,void *ctx)
{
  TS             ts = (TS)ctx;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(snes,SNES_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,2);
  PetscValidHeaderSpecific(F,VEC_CLASSID,3);
  PetscValidHeaderSpecific(ts,TS_CLASSID,4);
  ierr = (ts->ops->snesfunction)(snes,U,F,ts);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "SNESTSFormJacobian"
/*@
   SNESTSFormJacobian - Function to evaluate the Jacobian

   Collective on SNES

   Input Parameter:
+ snes - nonlinear solver
. U - the current state at which to evaluate the residual
- ctx - user context, must be a TS

   Output Parameter:
+ A - the Jacobian
. B - the preconditioning matrix (may be the same as A)
- flag - indicates any structure change in the matrix

   Notes:
   This function is not normally called by users and is automatically registered with the SNES used by TS.

   Level: developer

.seealso: SNESSetJacobian()
@*/
PetscErrorCode  SNESTSFormJacobian(SNES snes,Vec U,Mat *A,Mat *B,MatStructure *flag,void *ctx)
{
  TS             ts = (TS)ctx;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(snes,SNES_CLASSID,1);
  PetscValidHeaderSpecific(U,VEC_CLASSID,2);
  PetscValidPointer(A,3);
  PetscValidHeaderSpecific(*A,MAT_CLASSID,3);
  PetscValidPointer(B,4);
  PetscValidHeaderSpecific(*B,MAT_CLASSID,4);
  PetscValidPointer(flag,5);
  PetscValidHeaderSpecific(ts,TS_CLASSID,6);
  ierr = (ts->ops->snesjacobian)(snes,U,A,B,flag,ts);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeRHSFunctionLinear"
/*@C
   TSComputeRHSFunctionLinear - Evaluate the right hand side via the user-provided Jacobian, for linear problems only

   Collective on TS

   Input Arguments:
+  ts - time stepping context
.  t - time at which to evaluate
.  U - state at which to evaluate
-  ctx - context

   Output Arguments:
.  F - right hand side

   Level: intermediate

   Notes:
   This function is intended to be passed to TSSetRHSFunction() to evaluate the right hand side for linear problems.
   The matrix (and optionally the evaluation context) should be passed to TSSetRHSJacobian().

.seealso: TSSetRHSFunction(), TSSetRHSJacobian(), TSComputeRHSJacobianConstant()
@*/
PetscErrorCode TSComputeRHSFunctionLinear(TS ts,PetscReal t,Vec U,Vec F,void *ctx)
{
  PetscErrorCode ierr;
  Mat            Arhs,Brhs;
  MatStructure   flg2;

  PetscFunctionBegin;
  ierr = TSGetRHSMats_Private(ts,&Arhs,&Brhs);CHKERRQ(ierr);
  ierr = TSComputeRHSJacobian(ts,t,U,&Arhs,&Brhs,&flg2);CHKERRQ(ierr);
  ierr = MatMult(Arhs,U,F);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeRHSJacobianConstant"
/*@C
   TSComputeRHSJacobianConstant - Reuses a Jacobian that is time-independent.

   Collective on TS

   Input Arguments:
+  ts - time stepping context
.  t - time at which to evaluate
.  U - state at which to evaluate
-  ctx - context

   Output Arguments:
+  A - pointer to operator
.  B - pointer to preconditioning matrix
-  flg - matrix structure flag

   Level: intermediate

   Notes:
   This function is intended to be passed to TSSetRHSJacobian() to evaluate the Jacobian for linear time-independent problems.

.seealso: TSSetRHSFunction(), TSSetRHSJacobian(), TSComputeRHSFunctionLinear()
@*/
PetscErrorCode TSComputeRHSJacobianConstant(TS ts,PetscReal t,Vec U,Mat *A,Mat *B,MatStructure *flg,void *ctx)
{
  PetscFunctionBegin;
  *flg = SAME_PRECONDITIONER;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeIFunctionLinear"
/*@C
   TSComputeIFunctionLinear - Evaluate the left hand side via the user-provided Jacobian, for linear problems only

   Collective on TS

   Input Arguments:
+  ts - time stepping context
.  t - time at which to evaluate
.  U - state at which to evaluate
.  Udot - time derivative of state vector
-  ctx - context

   Output Arguments:
.  F - left hand side

   Level: intermediate

   Notes:
   The assumption here is that the left hand side is of the form A*Udot (and not A*Udot + B*U). For other cases, the
   user is required to write their own TSComputeIFunction.
   This function is intended to be passed to TSSetIFunction() to evaluate the left hand side for linear problems.
   The matrix (and optionally the evaluation context) should be passed to TSSetIJacobian().

.seealso: TSSetIFunction(), TSSetIJacobian(), TSComputeIJacobianConstant()
@*/
PetscErrorCode TSComputeIFunctionLinear(TS ts,PetscReal t,Vec U,Vec Udot,Vec F,void *ctx)
{
  PetscErrorCode ierr;
  Mat            A,B;
  MatStructure   flg2;

  PetscFunctionBegin;
  ierr = TSGetIJacobian(ts,&A,&B,NULL,NULL);CHKERRQ(ierr);
  ierr = TSComputeIJacobian(ts,t,U,Udot,1.0,&A,&B,&flg2,PETSC_TRUE);CHKERRQ(ierr);
  ierr = MatMult(A,Udot,F);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeIJacobianConstant"
/*@C
   TSComputeIJacobianConstant - Reuses a Jacobian that is time-independent.

   Collective on TS

   Input Arguments:
+  ts - time stepping context
.  t - time at which to evaluate
.  U - state at which to evaluate
.  Udot - time derivative of state vector
.  shift - shift to apply
-  ctx - context

   Output Arguments:
+  A - pointer to operator
.  B - pointer to preconditioning matrix
-  flg - matrix structure flag

   Level: intermediate

   Notes:
   This function is intended to be passed to TSSetIJacobian() to evaluate the Jacobian for linear time-independent problems.

.seealso: TSSetIFunction(), TSSetIJacobian(), TSComputeIFunctionLinear()
@*/
PetscErrorCode TSComputeIJacobianConstant(TS ts,PetscReal t,Vec U,Vec Udot,PetscReal shift,Mat *A,Mat *B,MatStructure *flg,void *ctx)
{
  PetscFunctionBegin;
  *flg = SAME_PRECONDITIONER;
  PetscFunctionReturn(0);
}
#undef __FUNCT__
#define __FUNCT__ "TSGetEquationType"
/*@
   TSGetEquationType - Gets the type of the equation that TS is solving.

   Not Collective

   Input Parameter:
.  ts - the TS context

   Output Parameter:
.  equation_type - see TSEquationType

   Level: beginner

.keywords: TS, equation type

.seealso: TSSetEquationType(), TSEquationType
@*/
PetscErrorCode  TSGetEquationType(TS ts,TSEquationType *equation_type)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(equation_type,2);
  *equation_type = ts->equation_type;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetEquationType"
/*@
   TSSetEquationType - Sets the type of the equation that TS is solving.

   Not Collective

   Input Parameter:
+  ts - the TS context
.  equation_type - see TSEquationType

   Level: advanced

.keywords: TS, equation type

.seealso: TSGetEquationType(), TSEquationType
@*/
PetscErrorCode  TSSetEquationType(TS ts,TSEquationType equation_type)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->equation_type = equation_type;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetConvergedReason"
/*@
   TSGetConvergedReason - Gets the reason the TS iteration was stopped.

   Not Collective

   Input Parameter:
.  ts - the TS context

   Output Parameter:
.  reason - negative value indicates diverged, positive value converged, see TSConvergedReason or the
            manual pages for the individual convergence tests for complete lists

   Level: beginner

   Notes:
   Can only be called after the call to TSSolve() is complete.

.keywords: TS, nonlinear, set, convergence, test

.seealso: TSSetConvergenceTest(), TSConvergedReason
@*/
PetscErrorCode  TSGetConvergedReason(TS ts,TSConvergedReason *reason)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(reason,2);
  *reason = ts->reason;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetConvergedReason"
/*@
   TSSetConvergedReason - Sets the reason for handling the convergence of TSSolve.

   Not Collective

   Input Parameter:
+  ts - the TS context
.  reason - negative value indicates diverged, positive value converged, see TSConvergedReason or the
            manual pages for the individual convergence tests for complete lists

   Level: advanced

   Notes:
   Can only be called during TSSolve() is active.

.keywords: TS, nonlinear, set, convergence, test

.seealso: TSConvergedReason
@*/
PetscErrorCode  TSSetConvergedReason(TS ts,TSConvergedReason reason)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->reason = reason;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetSolveTime"
/*@
   TSGetSolveTime - Gets the time after a call to TSSolve()

   Not Collective

   Input Parameter:
.  ts - the TS context

   Output Parameter:
.  ftime - the final time. This time should correspond to the final time set with TSSetDuration()

   Level: beginner

   Notes:
   Can only be called after the call to TSSolve() is complete.

.keywords: TS, nonlinear, set, convergence, test

.seealso: TSSetConvergenceTest(), TSConvergedReason
@*/
PetscErrorCode  TSGetSolveTime(TS ts,PetscReal *ftime)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(ftime,2);
  *ftime = ts->solvetime;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetSNESIterations"
/*@
   TSGetSNESIterations - Gets the total number of nonlinear iterations
   used by the time integrator.

   Not Collective

   Input Parameter:
.  ts - TS context

   Output Parameter:
.  nits - number of nonlinear iterations

   Notes:
   This counter is reset to zero for each successive call to TSSolve().

   Level: intermediate

.keywords: TS, get, number, nonlinear, iterations

.seealso:  TSGetKSPIterations()
@*/
PetscErrorCode TSGetSNESIterations(TS ts,PetscInt *nits)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidIntPointer(nits,2);
  *nits = ts->snes_its;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetKSPIterations"
/*@
   TSGetKSPIterations - Gets the total number of linear iterations
   used by the time integrator.

   Not Collective

   Input Parameter:
.  ts - TS context

   Output Parameter:
.  lits - number of linear iterations

   Notes:
   This counter is reset to zero for each successive call to TSSolve().

   Level: intermediate

.keywords: TS, get, number, linear, iterations

.seealso:  TSGetSNESIterations(), SNESGetKSPIterations()
@*/
PetscErrorCode TSGetKSPIterations(TS ts,PetscInt *lits)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidIntPointer(lits,2);
  *lits = ts->ksp_its;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetStepRejections"
/*@
   TSGetStepRejections - Gets the total number of rejected steps.

   Not Collective

   Input Parameter:
.  ts - TS context

   Output Parameter:
.  rejects - number of steps rejected

   Notes:
   This counter is reset to zero for each successive call to TSSolve().

   Level: intermediate

.keywords: TS, get, number

.seealso:  TSGetSNESIterations(), TSGetKSPIterations(), TSSetMaxStepRejections(), TSGetSNESFailures(), TSSetMaxSNESFailures(), TSSetErrorIfStepFails()
@*/
PetscErrorCode TSGetStepRejections(TS ts,PetscInt *rejects)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidIntPointer(rejects,2);
  *rejects = ts->reject;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetSNESFailures"
/*@
   TSGetSNESFailures - Gets the total number of failed SNES solves

   Not Collective

   Input Parameter:
.  ts - TS context

   Output Parameter:
.  fails - number of failed nonlinear solves

   Notes:
   This counter is reset to zero for each successive call to TSSolve().

   Level: intermediate

.keywords: TS, get, number

.seealso:  TSGetSNESIterations(), TSGetKSPIterations(), TSSetMaxStepRejections(), TSGetStepRejections(), TSSetMaxSNESFailures()
@*/
PetscErrorCode TSGetSNESFailures(TS ts,PetscInt *fails)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidIntPointer(fails,2);
  *fails = ts->num_snes_failures;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetMaxStepRejections"
/*@
   TSSetMaxStepRejections - Sets the maximum number of step rejections before a step fails

   Not Collective

   Input Parameter:
+  ts - TS context
-  rejects - maximum number of rejected steps, pass -1 for unlimited

   Notes:
   The counter is reset to zero for each step

   Options Database Key:
 .  -ts_max_reject - Maximum number of step rejections before a step fails

   Level: intermediate

.keywords: TS, set, maximum, number

.seealso:  TSGetSNESIterations(), TSGetKSPIterations(), TSSetMaxSNESFailures(), TSGetStepRejections(), TSGetSNESFailures(), TSSetErrorIfStepFails(), TSGetConvergedReason()
@*/
PetscErrorCode TSSetMaxStepRejections(TS ts,PetscInt rejects)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->max_reject = rejects;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetMaxSNESFailures"
/*@
   TSSetMaxSNESFailures - Sets the maximum number of failed SNES solves

   Not Collective

   Input Parameter:
+  ts - TS context
-  fails - maximum number of failed nonlinear solves, pass -1 for unlimited

   Notes:
   The counter is reset to zero for each successive call to TSSolve().

   Options Database Key:
 .  -ts_max_snes_failures - Maximum number of nonlinear solve failures

   Level: intermediate

.keywords: TS, set, maximum, number

.seealso:  TSGetSNESIterations(), TSGetKSPIterations(), TSSetMaxStepRejections(), TSGetStepRejections(), TSGetSNESFailures(), SNESGetConvergedReason(), TSGetConvergedReason()
@*/
PetscErrorCode TSSetMaxSNESFailures(TS ts,PetscInt fails)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->max_snes_failures = fails;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetErrorIfStepFails()"
/*@
   TSSetErrorIfStepFails - Error if no step succeeds

   Not Collective

   Input Parameter:
+  ts - TS context
-  err - PETSC_TRUE to error if no step succeeds, PETSC_FALSE to return without failure

   Options Database Key:
 .  -ts_error_if_step_fails - Error if no step succeeds

   Level: intermediate

.keywords: TS, set, error

.seealso:  TSGetSNESIterations(), TSGetKSPIterations(), TSSetMaxStepRejections(), TSGetStepRejections(), TSGetSNESFailures(), TSSetErrorIfStepFails(), TSGetConvergedReason()
@*/
PetscErrorCode TSSetErrorIfStepFails(TS ts,PetscBool err)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->errorifstepfailed = err;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorSolutionBinary"
/*@C
   TSMonitorSolutionBinary - Monitors progress of the TS solvers by VecView() for the solution at each timestep. Normally the viewer is a binary file

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
.  u - current state
-  viewer - binary viewer

   Level: intermediate

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView()
@*/
PetscErrorCode  TSMonitorSolutionBinary(TS ts,PetscInt step,PetscReal ptime,Vec u,void *viewer)
{
  PetscErrorCode ierr;
  PetscViewer    v = (PetscViewer)viewer;

  PetscFunctionBegin;
  ierr = VecView(u,v);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorSolutionVTK"
/*@C
   TSMonitorSolutionVTK - Monitors progress of the TS solvers by VecView() for the solution at each timestep.

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
.  u - current state
-  filenametemplate - string containing a format specifier for the integer time step (e.g. %03D)

   Level: intermediate

   Notes:
   The VTK format does not allow writing multiple time steps in the same file, therefore a different file will be written for each time step.
   These are named according to the file name template.

   This function is normally passed as an argument to TSMonitorSet() along with TSMonitorSolutionVTKDestroy().

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView()
@*/
PetscErrorCode TSMonitorSolutionVTK(TS ts,PetscInt step,PetscReal ptime,Vec u,void *filenametemplate)
{
  PetscErrorCode ierr;
  char           filename[PETSC_MAX_PATH_LEN];
  PetscViewer    viewer;

  PetscFunctionBegin;
  ierr = PetscSNPrintf(filename,sizeof(filename),(const char*)filenametemplate,step);CHKERRQ(ierr);
  ierr = PetscViewerVTKOpen(PetscObjectComm((PetscObject)ts),filename,FILE_MODE_WRITE,&viewer);CHKERRQ(ierr);
  ierr = VecView(u,viewer);CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&viewer);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorSolutionVTKDestroy"
/*@C
   TSMonitorSolutionVTKDestroy - Destroy context for monitoring

   Collective on TS

   Input Parameters:
.  filenametemplate - string containing a format specifier for the integer time step (e.g. %03D)

   Level: intermediate

   Note:
   This function is normally passed to TSMonitorSet() along with TSMonitorSolutionVTK().

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorSolutionVTK()
@*/
PetscErrorCode TSMonitorSolutionVTKDestroy(void *filenametemplate)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFree(*(char**)filenametemplate);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetAdapt"
/*@
   TSGetAdapt - Get the adaptive controller context for the current method

   Collective on TS if controller has not been created yet

   Input Arguments:
.  ts - time stepping context

   Output Arguments:
.  adapt - adaptive controller

   Level: intermediate

.seealso: TSAdapt, TSAdaptSetType(), TSAdaptChoose()
@*/
PetscErrorCode TSGetAdapt(TS ts,TSAdapt *adapt)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidPointer(adapt,2);
  if (!ts->adapt) {
    ierr = TSAdaptCreate(PetscObjectComm((PetscObject)ts),&ts->adapt);CHKERRQ(ierr);
    ierr = PetscLogObjectParent(ts,ts->adapt);CHKERRQ(ierr);
    ierr = PetscObjectIncrementTabLevel((PetscObject)ts->adapt,(PetscObject)ts,1);CHKERRQ(ierr);
  }
  *adapt = ts->adapt;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetTolerances"
/*@
   TSSetTolerances - Set tolerances for local truncation error when using adaptive controller

   Logically Collective

   Input Arguments:
+  ts - time integration context
.  atol - scalar absolute tolerances, PETSC_DECIDE to leave current value
.  vatol - vector of absolute tolerances or NULL, used in preference to atol if present
.  rtol - scalar relative tolerances, PETSC_DECIDE to leave current value
-  vrtol - vector of relative tolerances or NULL, used in preference to atol if present

   Level: beginner

.seealso: TS, TSAdapt, TSVecNormWRMS(), TSGetTolerances()
@*/
PetscErrorCode TSSetTolerances(TS ts,PetscReal atol,Vec vatol,PetscReal rtol,Vec vrtol)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (atol != PETSC_DECIDE && atol != PETSC_DEFAULT) ts->atol = atol;
  if (vatol) {
    ierr = PetscObjectReference((PetscObject)vatol);CHKERRQ(ierr);
    ierr = VecDestroy(&ts->vatol);CHKERRQ(ierr);

    ts->vatol = vatol;
  }
  if (rtol != PETSC_DECIDE && rtol != PETSC_DEFAULT) ts->rtol = rtol;
  if (vrtol) {
    ierr = PetscObjectReference((PetscObject)vrtol);CHKERRQ(ierr);
    ierr = VecDestroy(&ts->vrtol);CHKERRQ(ierr);

    ts->vrtol = vrtol;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetTolerances"
/*@
   TSGetTolerances - Get tolerances for local truncation error when using adaptive controller

   Logically Collective

   Input Arguments:
.  ts - time integration context

   Output Arguments:
+  atol - scalar absolute tolerances, NULL to ignore
.  vatol - vector of absolute tolerances, NULL to ignore
.  rtol - scalar relative tolerances, NULL to ignore
-  vrtol - vector of relative tolerances, NULL to ignore

   Level: beginner

.seealso: TS, TSAdapt, TSVecNormWRMS(), TSSetTolerances()
@*/
PetscErrorCode TSGetTolerances(TS ts,PetscReal *atol,Vec *vatol,PetscReal *rtol,Vec *vrtol)
{
  PetscFunctionBegin;
  if (atol)  *atol  = ts->atol;
  if (vatol) *vatol = ts->vatol;
  if (rtol)  *rtol  = ts->rtol;
  if (vrtol) *vrtol = ts->vrtol;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSErrorNormWRMS"
/*@
   TSErrorNormWRMS - compute a weighted norm of the difference between a vector and the current state

   Collective on TS

   Input Arguments:
+  ts - time stepping context
-  Y - state vector to be compared to ts->vec_sol

   Output Arguments:
.  norm - weighted norm, a value of 1.0 is considered small

   Level: developer

.seealso: TSSetTolerances()
@*/
PetscErrorCode TSErrorNormWRMS(TS ts,Vec Y,PetscReal *norm)
{
  PetscErrorCode    ierr;
  PetscInt          i,n,N;
  const PetscScalar *u,*y;
  Vec               U;
  PetscReal         sum,gsum;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(Y,VEC_CLASSID,2);
  PetscValidPointer(norm,3);
  U = ts->vec_sol;
  PetscCheckSameTypeAndComm(U,1,Y,2);
  if (U == Y) SETERRQ(PetscObjectComm((PetscObject)U),PETSC_ERR_ARG_IDN,"Y cannot be the TS solution vector");

  ierr = VecGetSize(U,&N);CHKERRQ(ierr);
  ierr = VecGetLocalSize(U,&n);CHKERRQ(ierr);
  ierr = VecGetArrayRead(U,&u);CHKERRQ(ierr);
  ierr = VecGetArrayRead(Y,&y);CHKERRQ(ierr);
  sum  = 0.;
  if (ts->vatol && ts->vrtol) {
    const PetscScalar *atol,*rtol;
    ierr = VecGetArrayRead(ts->vatol,&atol);CHKERRQ(ierr);
    ierr = VecGetArrayRead(ts->vrtol,&rtol);CHKERRQ(ierr);
    for (i=0; i<n; i++) {
      PetscReal tol = PetscRealPart(atol[i]) + PetscRealPart(rtol[i]) * PetscMax(PetscAbsScalar(u[i]),PetscAbsScalar(y[i]));
      sum += PetscSqr(PetscAbsScalar(y[i] - u[i]) / tol);
    }
    ierr = VecRestoreArrayRead(ts->vatol,&atol);CHKERRQ(ierr);
    ierr = VecRestoreArrayRead(ts->vrtol,&rtol);CHKERRQ(ierr);
  } else if (ts->vatol) {       /* vector atol, scalar rtol */
    const PetscScalar *atol;
    ierr = VecGetArrayRead(ts->vatol,&atol);CHKERRQ(ierr);
    for (i=0; i<n; i++) {
      PetscReal tol = PetscRealPart(atol[i]) + ts->rtol * PetscMax(PetscAbsScalar(u[i]),PetscAbsScalar(y[i]));
      sum += PetscSqr(PetscAbsScalar(y[i] - u[i]) / tol);
    }
    ierr = VecRestoreArrayRead(ts->vatol,&atol);CHKERRQ(ierr);
  } else if (ts->vrtol) {       /* scalar atol, vector rtol */
    const PetscScalar *rtol;
    ierr = VecGetArrayRead(ts->vrtol,&rtol);CHKERRQ(ierr);
    for (i=0; i<n; i++) {
      PetscReal tol = ts->atol + PetscRealPart(rtol[i]) * PetscMax(PetscAbsScalar(u[i]),PetscAbsScalar(y[i]));
      sum += PetscSqr(PetscAbsScalar(y[i] - u[i]) / tol);
    }
    ierr = VecRestoreArrayRead(ts->vrtol,&rtol);CHKERRQ(ierr);
  } else {                      /* scalar atol, scalar rtol */
    for (i=0; i<n; i++) {
      PetscReal tol = ts->atol + ts->rtol * PetscMax(PetscAbsScalar(u[i]),PetscAbsScalar(y[i]));
      sum += PetscSqr(PetscAbsScalar(y[i] - u[i]) / tol);
    }
  }
  ierr = VecRestoreArrayRead(U,&u);CHKERRQ(ierr);
  ierr = VecRestoreArrayRead(Y,&y);CHKERRQ(ierr);

  ierr  = MPI_Allreduce(&sum,&gsum,1,MPIU_REAL,MPIU_SUM,PetscObjectComm((PetscObject)ts));CHKERRQ(ierr);
  *norm = PetscSqrtReal(gsum / N);
  if (PetscIsInfOrNanScalar(*norm)) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_FP,"Infinite or not-a-number generated in norm");
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSSetCFLTimeLocal"
/*@
   TSSetCFLTimeLocal - Set the local CFL constraint relative to forward Euler

   Logically Collective on TS

   Input Arguments:
+  ts - time stepping context
-  cfltime - maximum stable time step if using forward Euler (value can be different on each process)

   Note:
   After calling this function, the global CFL time can be obtained by calling TSGetCFLTime()

   Level: intermediate

.seealso: TSGetCFLTime(), TSADAPTCFL
@*/
PetscErrorCode TSSetCFLTimeLocal(TS ts,PetscReal cfltime)
{
  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  ts->cfltime_local = cfltime;
  ts->cfltime       = -1.;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSGetCFLTime"
/*@
   TSGetCFLTime - Get the maximum stable time step according to CFL criteria applied to forward Euler

   Collective on TS

   Input Arguments:
.  ts - time stepping context

   Output Arguments:
.  cfltime - maximum stable time step for forward Euler

   Level: advanced

.seealso: TSSetCFLTimeLocal()
@*/
PetscErrorCode TSGetCFLTime(TS ts,PetscReal *cfltime)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (ts->cfltime < 0) {
    ierr = MPI_Allreduce(&ts->cfltime_local,&ts->cfltime,1,MPIU_REAL,MPIU_MIN,PetscObjectComm((PetscObject)ts));CHKERRQ(ierr);
  }
  *cfltime = ts->cfltime;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSVISetVariableBounds"
/*@
   TSVISetVariableBounds - Sets the lower and upper bounds for the solution vector. xl <= x <= xu

   Input Parameters:
.  ts   - the TS context.
.  xl   - lower bound.
.  xu   - upper bound.

   Notes:
   If this routine is not called then the lower and upper bounds are set to
   SNES_VI_NINF and SNES_VI_INF respectively during SNESSetUp().

   Level: advanced

@*/
PetscErrorCode TSVISetVariableBounds(TS ts, Vec xl, Vec xu)
{
  PetscErrorCode ierr;
  SNES           snes;

  PetscFunctionBegin;
  ierr = TSGetSNES(ts,&snes);CHKERRQ(ierr);
  ierr = SNESVISetVariableBounds(snes,xl,xu);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#if defined(PETSC_HAVE_MATLAB_ENGINE)
#include <mex.h>

typedef struct {char *funcname; mxArray *ctx;} TSMatlabContext;

#undef __FUNCT__
#define __FUNCT__ "TSComputeFunction_Matlab"
/*
   TSComputeFunction_Matlab - Calls the function that has been set with
                         TSSetFunctionMatlab().

   Collective on TS

   Input Parameters:
+  snes - the TS context
-  u - input vector

   Output Parameter:
.  y - function vector, as set by TSSetFunction()

   Notes:
   TSComputeFunction() is typically used within nonlinear solvers
   implementations, so most users would not generally call this routine
   themselves.

   Level: developer

.keywords: TS, nonlinear, compute, function

.seealso: TSSetFunction(), TSGetFunction()
*/
PetscErrorCode  TSComputeFunction_Matlab(TS snes,PetscReal time,Vec u,Vec udot,Vec y, void *ctx)
{
  PetscErrorCode  ierr;
  TSMatlabContext *sctx = (TSMatlabContext*)ctx;
  int             nlhs  = 1,nrhs = 7;
  mxArray         *plhs[1],*prhs[7];
  long long int   lx = 0,lxdot = 0,ly = 0,ls = 0;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(snes,TS_CLASSID,1);
  PetscValidHeaderSpecific(u,VEC_CLASSID,3);
  PetscValidHeaderSpecific(udot,VEC_CLASSID,4);
  PetscValidHeaderSpecific(y,VEC_CLASSID,5);
  PetscCheckSameComm(snes,1,u,3);
  PetscCheckSameComm(snes,1,y,5);

  ierr = PetscMemcpy(&ls,&snes,sizeof(snes));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lx,&u,sizeof(u));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lxdot,&udot,sizeof(udot));CHKERRQ(ierr);
  ierr = PetscMemcpy(&ly,&y,sizeof(u));CHKERRQ(ierr);

  prhs[0] =  mxCreateDoubleScalar((double)ls);
  prhs[1] =  mxCreateDoubleScalar(time);
  prhs[2] =  mxCreateDoubleScalar((double)lx);
  prhs[3] =  mxCreateDoubleScalar((double)lxdot);
  prhs[4] =  mxCreateDoubleScalar((double)ly);
  prhs[5] =  mxCreateString(sctx->funcname);
  prhs[6] =  sctx->ctx;
  ierr    =  mexCallMATLAB(nlhs,plhs,nrhs,prhs,"PetscTSComputeFunctionInternal");CHKERRQ(ierr);
  ierr    =  mxGetScalar(plhs[0]);CHKERRQ(ierr);
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  mxDestroyArray(prhs[4]);
  mxDestroyArray(prhs[5]);
  mxDestroyArray(plhs[0]);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSSetFunctionMatlab"
/*
   TSSetFunctionMatlab - Sets the function evaluation routine and function
   vector for use by the TS routines in solving ODEs
   equations from MATLAB. Here the function is a string containing the name of a MATLAB function

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context
-  func - function evaluation routine

   Calling sequence of func:
$    func (TS ts,PetscReal time,Vec u,Vec udot,Vec f,void *ctx);

   Level: beginner

.keywords: TS, nonlinear, set, function

.seealso: TSGetFunction(), TSComputeFunction(), TSSetJacobian(), TSSetFunction()
*/
PetscErrorCode  TSSetFunctionMatlab(TS ts,const char *func,mxArray *ctx)
{
  PetscErrorCode  ierr;
  TSMatlabContext *sctx;

  PetscFunctionBegin;
  /* currently sctx is memory bleed */
  ierr = PetscMalloc(sizeof(TSMatlabContext),&sctx);CHKERRQ(ierr);
  ierr = PetscStrallocpy(func,&sctx->funcname);CHKERRQ(ierr);
  /*
     This should work, but it doesn't
  sctx->ctx = ctx;
  mexMakeArrayPersistent(sctx->ctx);
  */
  sctx->ctx = mxDuplicateArray(ctx);

  ierr = TSSetIFunction(ts,NULL,TSComputeFunction_Matlab,sctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeJacobian_Matlab"
/*
   TSComputeJacobian_Matlab - Calls the function that has been set with
                         TSSetJacobianMatlab().

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  u - input vector
.  A, B - the matrices
-  ctx - user context

   Output Parameter:
.  flag - structure of the matrix

   Level: developer

.keywords: TS, nonlinear, compute, function

.seealso: TSSetFunction(), TSGetFunction()
@*/
PetscErrorCode  TSComputeJacobian_Matlab(TS ts,PetscReal time,Vec u,Vec udot,PetscReal shift,Mat *A,Mat *B,MatStructure *flag, void *ctx)
{
  PetscErrorCode  ierr;
  TSMatlabContext *sctx = (TSMatlabContext*)ctx;
  int             nlhs  = 2,nrhs = 9;
  mxArray         *plhs[2],*prhs[9];
  long long int   lx = 0,lxdot = 0,lA = 0,ls = 0, lB = 0;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(u,VEC_CLASSID,3);

  /* call Matlab function in ctx with arguments u and y */

  ierr = PetscMemcpy(&ls,&ts,sizeof(ts));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lx,&u,sizeof(u));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lxdot,&udot,sizeof(u));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lA,A,sizeof(u));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lB,B,sizeof(u));CHKERRQ(ierr);

  prhs[0] =  mxCreateDoubleScalar((double)ls);
  prhs[1] =  mxCreateDoubleScalar((double)time);
  prhs[2] =  mxCreateDoubleScalar((double)lx);
  prhs[3] =  mxCreateDoubleScalar((double)lxdot);
  prhs[4] =  mxCreateDoubleScalar((double)shift);
  prhs[5] =  mxCreateDoubleScalar((double)lA);
  prhs[6] =  mxCreateDoubleScalar((double)lB);
  prhs[7] =  mxCreateString(sctx->funcname);
  prhs[8] =  sctx->ctx;
  ierr    =  mexCallMATLAB(nlhs,plhs,nrhs,prhs,"PetscTSComputeJacobianInternal");CHKERRQ(ierr);
  ierr    =  mxGetScalar(plhs[0]);CHKERRQ(ierr);
  *flag   =  (MatStructure) mxGetScalar(plhs[1]);CHKERRQ(ierr);
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  mxDestroyArray(prhs[4]);
  mxDestroyArray(prhs[5]);
  mxDestroyArray(prhs[6]);
  mxDestroyArray(prhs[7]);
  mxDestroyArray(plhs[0]);
  mxDestroyArray(plhs[1]);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSSetJacobianMatlab"
/*
   TSSetJacobianMatlab - Sets the Jacobian function evaluation routine and two empty Jacobian matrices
   vector for use by the TS routines in solving ODEs from MATLAB. Here the function is a string containing the name of a MATLAB function

   Logically Collective on TS

   Input Parameters:
+  ts - the TS context
.  A,B - Jacobian matrices
.  func - function evaluation routine
-  ctx - user context

   Calling sequence of func:
$    flag = func (TS ts,PetscReal time,Vec u,Vec udot,Mat A,Mat B,void *ctx);


   Level: developer

.keywords: TS, nonlinear, set, function

.seealso: TSGetFunction(), TSComputeFunction(), TSSetJacobian(), TSSetFunction()
*/
PetscErrorCode  TSSetJacobianMatlab(TS ts,Mat A,Mat B,const char *func,mxArray *ctx)
{
  PetscErrorCode  ierr;
  TSMatlabContext *sctx;

  PetscFunctionBegin;
  /* currently sctx is memory bleed */
  ierr = PetscMalloc(sizeof(TSMatlabContext),&sctx);CHKERRQ(ierr);
  ierr = PetscStrallocpy(func,&sctx->funcname);CHKERRQ(ierr);
  /*
     This should work, but it doesn't
  sctx->ctx = ctx;
  mexMakeArrayPersistent(sctx->ctx);
  */
  sctx->ctx = mxDuplicateArray(ctx);

  ierr = TSSetIJacobian(ts,A,B,TSComputeJacobian_Matlab,sctx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitor_Matlab"
/*
   TSMonitor_Matlab - Calls the function that has been set with TSMonitorSetMatlab().

   Collective on TS

.seealso: TSSetFunction(), TSGetFunction()
@*/
PetscErrorCode  TSMonitor_Matlab(TS ts,PetscInt it, PetscReal time,Vec u, void *ctx)
{
  PetscErrorCode  ierr;
  TSMatlabContext *sctx = (TSMatlabContext*)ctx;
  int             nlhs  = 1,nrhs = 6;
  mxArray         *plhs[1],*prhs[6];
  long long int   lx = 0,ls = 0;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  PetscValidHeaderSpecific(u,VEC_CLASSID,4);

  ierr = PetscMemcpy(&ls,&ts,sizeof(ts));CHKERRQ(ierr);
  ierr = PetscMemcpy(&lx,&u,sizeof(u));CHKERRQ(ierr);

  prhs[0] =  mxCreateDoubleScalar((double)ls);
  prhs[1] =  mxCreateDoubleScalar((double)it);
  prhs[2] =  mxCreateDoubleScalar((double)time);
  prhs[3] =  mxCreateDoubleScalar((double)lx);
  prhs[4] =  mxCreateString(sctx->funcname);
  prhs[5] =  sctx->ctx;
  ierr    =  mexCallMATLAB(nlhs,plhs,nrhs,prhs,"PetscTSMonitorInternal");CHKERRQ(ierr);
  ierr    =  mxGetScalar(plhs[0]);CHKERRQ(ierr);
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  mxDestroyArray(prhs[4]);
  mxDestroyArray(plhs[0]);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "TSMonitorSetMatlab"
/*
   TSMonitorSetMatlab - Sets the monitor function from Matlab

   Level: developer

.keywords: TS, nonlinear, set, function

.seealso: TSGetFunction(), TSComputeFunction(), TSSetJacobian(), TSSetFunction()
*/
PetscErrorCode  TSMonitorSetMatlab(TS ts,const char *func,mxArray *ctx)
{
  PetscErrorCode  ierr;
  TSMatlabContext *sctx;

  PetscFunctionBegin;
  /* currently sctx is memory bleed */
  ierr = PetscMalloc(sizeof(TSMatlabContext),&sctx);CHKERRQ(ierr);
  ierr = PetscStrallocpy(func,&sctx->funcname);CHKERRQ(ierr);
  /*
     This should work, but it doesn't
  sctx->ctx = ctx;
  mexMakeArrayPersistent(sctx->ctx);
  */
  sctx->ctx = mxDuplicateArray(ctx);

  ierr = TSMonitorSet(ts,TSMonitor_Matlab,sctx,NULL);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}
#endif



#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGSolution"
/*@C
   TSMonitorLGSolution - Monitors progress of the TS solvers by plotting each component of the solution vector
       in a time based line graph

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
-  lg - a line graph object

   Level: intermediate

    Notes: each process in a parallel run displays its component solutions in a separate window

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView()
@*/
PetscErrorCode  TSMonitorLGSolution(TS ts,PetscInt step,PetscReal ptime,Vec u,void *dummy)
{
  PetscErrorCode    ierr;
  TSMonitorLGCtx    ctx = (TSMonitorLGCtx)dummy;
  const PetscScalar *yy;
  PetscInt          dim;

  PetscFunctionBegin;
  if (!step) {
    PetscDrawAxis axis;
    ierr = PetscDrawLGGetAxis(ctx->lg,&axis);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLabels(axis,"Solution as function of time","Time","Solution");CHKERRQ(ierr);
    ierr = VecGetLocalSize(u,&dim);CHKERRQ(ierr);
    ierr = PetscDrawLGSetDimension(ctx->lg,dim);CHKERRQ(ierr);
    ierr = PetscDrawLGReset(ctx->lg);CHKERRQ(ierr);
  }
  ierr = VecGetArrayRead(u,&yy);CHKERRQ(ierr);
#if defined(PETSC_USE_COMPLEX)
  {
    PetscReal *yreal;
    PetscInt  i,n;
    ierr = VecGetLocalSize(u,&n);CHKERRQ(ierr);
    ierr = PetscMalloc(n*sizeof(PetscReal),&yreal);CHKERRQ(ierr);
    for (i=0; i<n; i++) yreal[i] = PetscRealPart(yy[i]);
    ierr = PetscDrawLGAddCommonPoint(ctx->lg,ptime,yreal);CHKERRQ(ierr);
    ierr = PetscFree(yreal);CHKERRQ(ierr);
  }
#else
  ierr = PetscDrawLGAddCommonPoint(ctx->lg,ptime,yy);CHKERRQ(ierr);
#endif
  ierr = VecRestoreArrayRead(u,&yy);CHKERRQ(ierr);
  if (((ctx->howoften > 0) && (!(step % ctx->howoften))) || ((ctx->howoften == -1) && ts->reason)) {
    ierr = PetscDrawLGDraw(ctx->lg);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGError"
/*@C
   TSMonitorLGError - Monitors progress of the TS solvers by plotting each component of the solution vector
       in a time based line graph

   Collective on TS

   Input Parameters:
+  ts - the TS context
.  step - current time-step
.  ptime - current time
-  lg - a line graph object

   Level: intermediate

   Notes:
   Only for sequential solves.

   The user must provide the solution using TSSetSolutionFunction() to use this monitor.

   Options Database Keys:
.  -ts_monitor_lg_error - create a graphical monitor of error history

.keywords: TS,  vector, monitor, view

.seealso: TSMonitorSet(), TSMonitorDefault(), VecView(), TSSetSolutionFunction()
@*/
PetscErrorCode  TSMonitorLGError(TS ts,PetscInt step,PetscReal ptime,Vec u,void *dummy)
{
  PetscErrorCode    ierr;
  TSMonitorLGCtx    ctx = (TSMonitorLGCtx)dummy;
  const PetscScalar *yy;
  Vec               y;
  PetscInt          dim;

  PetscFunctionBegin;
  if (!step) {
    PetscDrawAxis axis;
    ierr = PetscDrawLGGetAxis(ctx->lg,&axis);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLabels(axis,"Error in solution as function of time","Time","Solution");CHKERRQ(ierr);
    ierr = VecGetLocalSize(u,&dim);CHKERRQ(ierr);
    ierr = PetscDrawLGSetDimension(ctx->lg,dim);CHKERRQ(ierr);
    ierr = PetscDrawLGReset(ctx->lg);CHKERRQ(ierr);
  }
  ierr = VecDuplicate(u,&y);CHKERRQ(ierr);
  ierr = TSComputeSolutionFunction(ts,ptime,y);CHKERRQ(ierr);
  ierr = VecAXPY(y,-1.0,u);CHKERRQ(ierr);
  ierr = VecGetArrayRead(y,&yy);CHKERRQ(ierr);
#if defined(PETSC_USE_COMPLEX)
  {
    PetscReal *yreal;
    PetscInt  i,n;
    ierr = VecGetLocalSize(y,&n);CHKERRQ(ierr);
    ierr = PetscMalloc(n*sizeof(PetscReal),&yreal);CHKERRQ(ierr);
    for (i=0; i<n; i++) yreal[i] = PetscRealPart(yy[i]);
    ierr = PetscDrawLGAddCommonPoint(ctx->lg,ptime,yreal);CHKERRQ(ierr);
    ierr = PetscFree(yreal);CHKERRQ(ierr);
  }
#else
  ierr = PetscDrawLGAddCommonPoint(ctx->lg,ptime,yy);CHKERRQ(ierr);
#endif
  ierr = VecRestoreArrayRead(y,&yy);CHKERRQ(ierr);
  ierr = VecDestroy(&y);CHKERRQ(ierr);
  if (((ctx->howoften > 0) && (!(step % ctx->howoften))) || ((ctx->howoften == -1) && ts->reason)) {
    ierr = PetscDrawLGDraw(ctx->lg);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGSNESIterations"
PetscErrorCode TSMonitorLGSNESIterations(TS ts,PetscInt n,PetscReal ptime,Vec v,void *monctx)
{
  TSMonitorLGCtx ctx = (TSMonitorLGCtx) monctx;
  PetscReal      x   = ptime,y;
  PetscErrorCode ierr;
  PetscInt       its;

  PetscFunctionBegin;
  if (!n) {
    PetscDrawAxis axis;

    ierr = PetscDrawLGGetAxis(ctx->lg,&axis);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLabels(axis,"Nonlinear iterations as function of time","Time","SNES Iterations");CHKERRQ(ierr);
    ierr = PetscDrawLGReset(ctx->lg);CHKERRQ(ierr);

    ctx->snes_its = 0;
  }
  ierr = TSGetSNESIterations(ts,&its);CHKERRQ(ierr);
  y    = its - ctx->snes_its;
  ierr = PetscDrawLGAddPoint(ctx->lg,&x,&y);CHKERRQ(ierr);
  if (((ctx->howoften > 0) && (!(n % ctx->howoften)) && (n > -1)) || ((ctx->howoften == -1) && (n == -1))) {
    ierr = PetscDrawLGDraw(ctx->lg);CHKERRQ(ierr);
  }
  ctx->snes_its = its;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSMonitorLGKSPIterations"
PetscErrorCode TSMonitorLGKSPIterations(TS ts,PetscInt n,PetscReal ptime,Vec v,void *monctx)
{
  TSMonitorLGCtx ctx = (TSMonitorLGCtx) monctx;
  PetscReal      x   = ptime,y;
  PetscErrorCode ierr;
  PetscInt       its;

  PetscFunctionBegin;
  if (!n) {
    PetscDrawAxis axis;

    ierr = PetscDrawLGGetAxis(ctx->lg,&axis);CHKERRQ(ierr);
    ierr = PetscDrawAxisSetLabels(axis,"Linear iterations as function of time","Time","KSP Iterations");CHKERRQ(ierr);
    ierr = PetscDrawLGReset(ctx->lg);CHKERRQ(ierr);

    ctx->ksp_its = 0;
  }
  ierr = TSGetKSPIterations(ts,&its);CHKERRQ(ierr);
  y    = its - ctx->ksp_its;
  ierr = PetscDrawLGAddPoint(ctx->lg,&x,&y);CHKERRQ(ierr);
  if (((ctx->howoften > 0) && (!(n % ctx->howoften)) && (n > -1)) || ((ctx->howoften == -1) && (n == -1))) {
    ierr = PetscDrawLGDraw(ctx->lg);CHKERRQ(ierr);
  }
  ctx->ksp_its = its;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TSComputeLinearStability"
/*@
   TSComputeLinearStability - computes the linear stability function at a point

   Collective on TS and Vec

   Input Parameters:
+  ts - the TS context
-  xr,xi - real and imaginary part of input arguments

   Output Parameters:
.  yr,yi - real and imaginary part of function value

   Level: developer

.keywords: TS, compute

.seealso: TSSetRHSFunction(), TSComputeIFunction()
@*/
PetscErrorCode TSComputeLinearStability(TS ts,PetscReal xr,PetscReal xi,PetscReal *yr,PetscReal *yi)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ts,TS_CLASSID,1);
  if (!ts->ops->linearstability) SETERRQ(PetscObjectComm((PetscObject)ts),PETSC_ERR_SUP,"Linearized stability function not provided for this method");
  ierr = (*ts->ops->linearstability)(ts,xr,xi,yr,yi);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}
