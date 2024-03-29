#include "petscsys.h"
#include "petscfix.h"
#include "petsc-private/fortranimpl.h"
/* ts.c */
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

#include "petscts.h"
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetfromoptions_ TSSETFROMOPTIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetfromoptions_ tssetfromoptions
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputerhsjacobian_ TSCOMPUTERHSJACOBIAN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputerhsjacobian_ tscomputerhsjacobian
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputerhsfunction_ TSCOMPUTERHSFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputerhsfunction_ tscomputerhsfunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputesolutionfunction_ TSCOMPUTESOLUTIONFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputesolutionfunction_ tscomputesolutionfunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputeforcingfunction_ TSCOMPUTEFORCINGFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputeforcingfunction_ tscomputeforcingfunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputeifunction_ TSCOMPUTEIFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputeifunction_ tscomputeifunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputeijacobian_ TSCOMPUTEIJACOBIAN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputeijacobian_ tscomputeijacobian
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetapplicationcontext_ TSSETAPPLICATIONCONTEXT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetapplicationcontext_ tssetapplicationcontext
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetapplicationcontext_ TSGETAPPLICATIONCONTEXT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetapplicationcontext_ tsgetapplicationcontext
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgettimestepnumber_ TSGETTIMESTEPNUMBER
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgettimestepnumber_ tsgettimestepnumber
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetinitialtimestep_ TSSETINITIALTIMESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetinitialtimestep_ tssetinitialtimestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssettimestep_ TSSETTIMESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssettimestep_ tssettimestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetexactfinaltime_ TSSETEXACTFINALTIME
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetexactfinaltime_ tssetexactfinaltime
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgettimestep_ TSGETTIMESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgettimestep_ tsgettimestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetsolution_ TSGETSOLUTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetsolution_ tsgetsolution
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetproblemtype_ TSSETPROBLEMTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetproblemtype_ tssetproblemtype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetup_ TSSETUP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetup_ tssetup
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsreset_ TSRESET
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsreset_ tsreset
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsdestroy_ TSDESTROY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsdestroy_ tsdestroy
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetsnes_ TSGETSNES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetsnes_ tsgetsnes
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetsnes_ TSSETSNES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetsnes_ tssetsnes
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetksp_ TSGETKSP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetksp_ tsgetksp
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetduration_ TSGETDURATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetduration_ tsgetduration
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetduration_ TSSETDURATION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetduration_ tssetduration
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetsolution_ TSSETSOLUTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetsolution_ tssetsolution
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsprestep_ TSPRESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsprestep_ tsprestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsprestage_ TSPRESTAGE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsprestage_ tsprestage
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tspoststep_ TSPOSTSTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tspoststep_ tspoststep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsmonitordefault_ TSMONITORDEFAULT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsmonitordefault_ tsmonitordefault
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetretainstages_ TSSETRETAINSTAGES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetretainstages_ tssetretainstages
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsinterpolate_ TSINTERPOLATE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsinterpolate_ tsinterpolate
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsstep_ TSSTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsstep_ tsstep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsevaluatestep_ TSEVALUATESTEP
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsevaluatestep_ tsevaluatestep
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssolve_ TSSOLVE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssolve_ tssolve
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsmonitor_ TSMONITOR
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsmonitor_ tsmonitor
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgettime_ TSGETTIME
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgettime_ tsgettime
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssettime_ TSSETTIME
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssettime_ tssettime
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetdm_ TSSETDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetdm_ tssetdm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetdm_ TSGETDM
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetdm_ tsgetdm
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snestsformfunction_ SNESTSFORMFUNCTION
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snestsformfunction_ snestsformfunction
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define snestsformjacobian_ SNESTSFORMJACOBIAN
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define snestsformjacobian_ snestsformjacobian
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetequationtype_ TSGETEQUATIONTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetequationtype_ tsgetequationtype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetequationtype_ TSSETEQUATIONTYPE
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetequationtype_ tssetequationtype
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetconvergedreason_ TSGETCONVERGEDREASON
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetconvergedreason_ tsgetconvergedreason
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetconvergedreason_ TSSETCONVERGEDREASON
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetconvergedreason_ tssetconvergedreason
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetsolvetime_ TSGETSOLVETIME
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetsolvetime_ tsgetsolvetime
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetsnesiterations_ TSGETSNESITERATIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetsnesiterations_ tsgetsnesiterations
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetkspiterations_ TSGETKSPITERATIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetkspiterations_ tsgetkspiterations
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetsteprejections_ TSGETSTEPREJECTIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetsteprejections_ tsgetsteprejections
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetsnesfailures_ TSGETSNESFAILURES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetsnesfailures_ tsgetsnesfailures
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetmaxsteprejections_ TSSETMAXSTEPREJECTIONS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetmaxsteprejections_ tssetmaxsteprejections
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetmaxsnesfailures_ TSSETMAXSNESFAILURES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetmaxsnesfailures_ tssetmaxsnesfailures
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsseterrorifstepfails_ TSSETERRORIFSTEPFAILS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsseterrorifstepfails_ tsseterrorifstepfails
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetadapt_ TSGETADAPT
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetadapt_ tsgetadapt
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssettolerances_ TSSETTOLERANCES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssettolerances_ tssettolerances
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgettolerances_ TSGETTOLERANCES
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgettolerances_ tsgettolerances
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tserrornormwrms_ TSERRORNORMWRMS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tserrornormwrms_ tserrornormwrms
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tssetcfltimelocal_ TSSETCFLTIMELOCAL
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tssetcfltimelocal_ tssetcfltimelocal
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsgetcfltime_ TSGETCFLTIME
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsgetcfltime_ tsgetcfltime
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tsvisetvariablebounds_ TSVISETVARIABLEBOUNDS
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tsvisetvariablebounds_ tsvisetvariablebounds
#endif
#ifdef PETSC_HAVE_FORTRAN_CAPS
#define tscomputelinearstability_ TSCOMPUTELINEARSTABILITY
#elif !defined(PETSC_HAVE_FORTRAN_UNDERSCORE) && !defined(FORTRANDOUBLEUNDERSCORE)
#define tscomputelinearstability_ tscomputelinearstability
#endif


/* Definitions of Fortran Wrapper routines */
#if defined(__cplusplus)
extern "C" {
#endif
void PETSC_STDCALL  tssetfromoptions_(TS ts, int *__ierr ){
*__ierr = TSSetFromOptions(
	(TS)PetscToPointer((ts) ));
}
void PETSC_STDCALL  tscomputerhsjacobian_(TS ts,PetscReal *t,Vec U,Mat *A,Mat *B,MatStructure *flg, int *__ierr ){
*__ierr = TSComputeRHSJacobian(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ),A,B,flg);
}
void PETSC_STDCALL  tscomputerhsfunction_(TS ts,PetscReal *t,Vec U,Vec y, int *__ierr ){
*__ierr = TSComputeRHSFunction(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ),
	(Vec)PetscToPointer((y) ));
}
void PETSC_STDCALL  tscomputesolutionfunction_(TS ts,PetscReal *t,Vec U, int *__ierr ){
*__ierr = TSComputeSolutionFunction(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ));
}
void PETSC_STDCALL  tscomputeforcingfunction_(TS ts,PetscReal *t,Vec U, int *__ierr ){
*__ierr = TSComputeForcingFunction(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ));
}
void PETSC_STDCALL  tscomputeifunction_(TS ts,PetscReal *t,Vec U,Vec Udot,Vec Y,PetscBool *imex, int *__ierr ){
*__ierr = TSComputeIFunction(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ),
	(Vec)PetscToPointer((Udot) ),
	(Vec)PetscToPointer((Y) ),*imex);
}
void PETSC_STDCALL  tscomputeijacobian_(TS ts,PetscReal *t,Vec U,Vec Udot,PetscReal *shift,Mat *A,Mat *B,MatStructure *flg,PetscBool *imex, int *__ierr ){
*__ierr = TSComputeIJacobian(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ),
	(Vec)PetscToPointer((Udot) ),*shift,A,B,flg,*imex);
}
void PETSC_STDCALL  tssetapplicationcontext_(TS ts,void*usrP, int *__ierr ){
*__ierr = TSSetApplicationContext(
	(TS)PetscToPointer((ts) ),usrP);
}
void PETSC_STDCALL  tsgetapplicationcontext_(TS ts,void*usrP, int *__ierr ){
*__ierr = TSGetApplicationContext(
	(TS)PetscToPointer((ts) ),usrP);
}
void PETSC_STDCALL  tsgettimestepnumber_(TS ts,PetscInt *iter, int *__ierr ){
*__ierr = TSGetTimeStepNumber(
	(TS)PetscToPointer((ts) ),iter);
}
void PETSC_STDCALL  tssetinitialtimestep_(TS ts,PetscReal *initial_time,PetscReal *time_step, int *__ierr ){
*__ierr = TSSetInitialTimeStep(
	(TS)PetscToPointer((ts) ),*initial_time,*time_step);
}
void PETSC_STDCALL  tssettimestep_(TS ts,PetscReal *time_step, int *__ierr ){
*__ierr = TSSetTimeStep(
	(TS)PetscToPointer((ts) ),*time_step);
}
void PETSC_STDCALL  tssetexactfinaltime_(TS ts,TSExactFinalTimeOption *eftopt, int *__ierr ){
*__ierr = TSSetExactFinalTime(
	(TS)PetscToPointer((ts) ),*eftopt);
}
void PETSC_STDCALL  tsgettimestep_(TS ts,PetscReal *dt, int *__ierr ){
*__ierr = TSGetTimeStep(
	(TS)PetscToPointer((ts) ),dt);
}
void PETSC_STDCALL  tsgetsolution_(TS ts,Vec *v, int *__ierr ){
*__ierr = TSGetSolution(
	(TS)PetscToPointer((ts) ),v);
}
void PETSC_STDCALL  tssetproblemtype_(TS ts,TSProblemType *type, int *__ierr ){
*__ierr = TSSetProblemType(
	(TS)PetscToPointer((ts) ),*type);
}
void PETSC_STDCALL  tssetup_(TS ts, int *__ierr ){
*__ierr = TSSetUp(
	(TS)PetscToPointer((ts) ));
}
void PETSC_STDCALL  tsreset_(TS ts, int *__ierr ){
*__ierr = TSReset(
	(TS)PetscToPointer((ts) ));
}
void PETSC_STDCALL  tsdestroy_(TS *ts, int *__ierr ){
*__ierr = TSDestroy(ts);
}
void PETSC_STDCALL  tsgetsnes_(TS ts,SNES *snes, int *__ierr ){
*__ierr = TSGetSNES(
	(TS)PetscToPointer((ts) ),snes);
}
void PETSC_STDCALL  tssetsnes_(TS ts,SNES snes, int *__ierr ){
*__ierr = TSSetSNES(
	(TS)PetscToPointer((ts) ),
	(SNES)PetscToPointer((snes) ));
}
void PETSC_STDCALL  tsgetksp_(TS ts,KSP *ksp, int *__ierr ){
*__ierr = TSGetKSP(
	(TS)PetscToPointer((ts) ),ksp);
}
void PETSC_STDCALL  tsgetduration_(TS ts,PetscInt *maxsteps,PetscReal *maxtime, int *__ierr ){
*__ierr = TSGetDuration(
	(TS)PetscToPointer((ts) ),maxsteps,maxtime);
}
void PETSC_STDCALL  tssetduration_(TS ts,PetscInt *maxsteps,PetscReal *maxtime, int *__ierr ){
*__ierr = TSSetDuration(
	(TS)PetscToPointer((ts) ),*maxsteps,*maxtime);
}
void PETSC_STDCALL  tssetsolution_(TS ts,Vec u, int *__ierr ){
*__ierr = TSSetSolution(
	(TS)PetscToPointer((ts) ),
	(Vec)PetscToPointer((u) ));
}
void PETSC_STDCALL  tsprestep_(TS ts, int *__ierr ){
*__ierr = TSPreStep(
	(TS)PetscToPointer((ts) ));
}
void PETSC_STDCALL  tsprestage_(TS ts,PetscReal *stagetime, int *__ierr ){
*__ierr = TSPreStage(
	(TS)PetscToPointer((ts) ),*stagetime);
}
void PETSC_STDCALL  tspoststep_(TS ts, int *__ierr ){
*__ierr = TSPostStep(
	(TS)PetscToPointer((ts) ));
}
void PETSC_STDCALL  tsmonitordefault_(TS ts,PetscInt *step,PetscReal *ptime,Vec v,void*dummy, int *__ierr ){
*__ierr = TSMonitorDefault(
	(TS)PetscToPointer((ts) ),*step,*ptime,
	(Vec)PetscToPointer((v) ),dummy);
}
void PETSC_STDCALL  tssetretainstages_(TS ts,PetscBool *flg, int *__ierr ){
*__ierr = TSSetRetainStages(
	(TS)PetscToPointer((ts) ),*flg);
}
void PETSC_STDCALL  tsinterpolate_(TS ts,PetscReal *t,Vec U, int *__ierr ){
*__ierr = TSInterpolate(
	(TS)PetscToPointer((ts) ),*t,
	(Vec)PetscToPointer((U) ));
}
void PETSC_STDCALL  tsstep_(TS ts, int *__ierr ){
*__ierr = TSStep(
	(TS)PetscToPointer((ts) ));
}
void PETSC_STDCALL  tsevaluatestep_(TS ts,PetscInt *order,Vec U,PetscBool *done, int *__ierr ){
*__ierr = TSEvaluateStep(
	(TS)PetscToPointer((ts) ),*order,
	(Vec)PetscToPointer((U) ),done);
}
void PETSC_STDCALL  tssolve_(TS ts,Vec u, int *__ierr ){
*__ierr = TSSolve(
	(TS)PetscToPointer((ts) ),
	(Vec)PetscToPointer((u) ));
}
void PETSC_STDCALL  tsmonitor_(TS ts,PetscInt *step,PetscReal *ptime,Vec u, int *__ierr ){
*__ierr = TSMonitor(
	(TS)PetscToPointer((ts) ),*step,*ptime,
	(Vec)PetscToPointer((u) ));
}
void PETSC_STDCALL  tsgettime_(TS ts,PetscReal *t, int *__ierr ){
*__ierr = TSGetTime(
	(TS)PetscToPointer((ts) ),t);
}
void PETSC_STDCALL  tssettime_(TS ts,PetscReal *t, int *__ierr ){
*__ierr = TSSetTime(
	(TS)PetscToPointer((ts) ),*t);
}
void PETSC_STDCALL  tssetdm_(TS ts,DM dm, int *__ierr ){
*__ierr = TSSetDM(
	(TS)PetscToPointer((ts) ),
	(DM)PetscToPointer((dm) ));
}
void PETSC_STDCALL  tsgetdm_(TS ts,DM *dm, int *__ierr ){
*__ierr = TSGetDM(
	(TS)PetscToPointer((ts) ),dm);
}
void PETSC_STDCALL  snestsformfunction_(SNES snes,Vec U,Vec F,void*ctx, int *__ierr ){
*__ierr = SNESTSFormFunction(
	(SNES)PetscToPointer((snes) ),
	(Vec)PetscToPointer((U) ),
	(Vec)PetscToPointer((F) ),ctx);
}
void PETSC_STDCALL  snestsformjacobian_(SNES snes,Vec U,Mat *A,Mat *B,MatStructure *flag,void*ctx, int *__ierr ){
*__ierr = SNESTSFormJacobian(
	(SNES)PetscToPointer((snes) ),
	(Vec)PetscToPointer((U) ),A,B,flag,ctx);
}
void PETSC_STDCALL  tsgetequationtype_(TS ts,TSEquationType *equation_type, int *__ierr ){
*__ierr = TSGetEquationType(
	(TS)PetscToPointer((ts) ),
	(TSEquationType* )PetscToPointer((equation_type) ));
}
void PETSC_STDCALL  tssetequationtype_(TS ts,TSEquationType *equation_type, int *__ierr ){
*__ierr = TSSetEquationType(
	(TS)PetscToPointer((ts) ),*equation_type);
}
void PETSC_STDCALL  tsgetconvergedreason_(TS ts,TSConvergedReason *reason, int *__ierr ){
*__ierr = TSGetConvergedReason(
	(TS)PetscToPointer((ts) ),reason);
}
void PETSC_STDCALL  tssetconvergedreason_(TS ts,TSConvergedReason *reason, int *__ierr ){
*__ierr = TSSetConvergedReason(
	(TS)PetscToPointer((ts) ),*reason);
}
void PETSC_STDCALL  tsgetsolvetime_(TS ts,PetscReal *ftime, int *__ierr ){
*__ierr = TSGetSolveTime(
	(TS)PetscToPointer((ts) ),ftime);
}
void PETSC_STDCALL  tsgetsnesiterations_(TS ts,PetscInt *nits, int *__ierr ){
*__ierr = TSGetSNESIterations(
	(TS)PetscToPointer((ts) ),nits);
}
void PETSC_STDCALL  tsgetkspiterations_(TS ts,PetscInt *lits, int *__ierr ){
*__ierr = TSGetKSPIterations(
	(TS)PetscToPointer((ts) ),lits);
}
void PETSC_STDCALL  tsgetsteprejections_(TS ts,PetscInt *rejects, int *__ierr ){
*__ierr = TSGetStepRejections(
	(TS)PetscToPointer((ts) ),rejects);
}
void PETSC_STDCALL  tsgetsnesfailures_(TS ts,PetscInt *fails, int *__ierr ){
*__ierr = TSGetSNESFailures(
	(TS)PetscToPointer((ts) ),fails);
}
void PETSC_STDCALL  tssetmaxsteprejections_(TS ts,PetscInt *rejects, int *__ierr ){
*__ierr = TSSetMaxStepRejections(
	(TS)PetscToPointer((ts) ),*rejects);
}
void PETSC_STDCALL  tssetmaxsnesfailures_(TS ts,PetscInt *fails, int *__ierr ){
*__ierr = TSSetMaxSNESFailures(
	(TS)PetscToPointer((ts) ),*fails);
}
void PETSC_STDCALL  tsseterrorifstepfails_(TS ts,PetscBool *err, int *__ierr ){
*__ierr = TSSetErrorIfStepFails(
	(TS)PetscToPointer((ts) ),*err);
}
void PETSC_STDCALL  tsgetadapt_(TS ts,TSAdapt *adapt, int *__ierr ){
*__ierr = TSGetAdapt(
	(TS)PetscToPointer((ts) ),adapt);
}
void PETSC_STDCALL  tssettolerances_(TS ts,PetscReal *atol,Vec vatol,PetscReal *rtol,Vec vrtol, int *__ierr ){
*__ierr = TSSetTolerances(
	(TS)PetscToPointer((ts) ),*atol,
	(Vec)PetscToPointer((vatol) ),*rtol,
	(Vec)PetscToPointer((vrtol) ));
}
void PETSC_STDCALL  tsgettolerances_(TS ts,PetscReal *atol,Vec *vatol,PetscReal *rtol,Vec *vrtol, int *__ierr ){
*__ierr = TSGetTolerances(
	(TS)PetscToPointer((ts) ),atol,vatol,rtol,vrtol);
}
void PETSC_STDCALL  tserrornormwrms_(TS ts,Vec Y,PetscReal *norm, int *__ierr ){
*__ierr = TSErrorNormWRMS(
	(TS)PetscToPointer((ts) ),
	(Vec)PetscToPointer((Y) ),norm);
}
void PETSC_STDCALL  tssetcfltimelocal_(TS ts,PetscReal *cfltime, int *__ierr ){
*__ierr = TSSetCFLTimeLocal(
	(TS)PetscToPointer((ts) ),*cfltime);
}
void PETSC_STDCALL  tsgetcfltime_(TS ts,PetscReal *cfltime, int *__ierr ){
*__ierr = TSGetCFLTime(
	(TS)PetscToPointer((ts) ),cfltime);
}
void PETSC_STDCALL  tsvisetvariablebounds_(TS ts,Vec xl,Vec xu, int *__ierr ){
*__ierr = TSVISetVariableBounds(
	(TS)PetscToPointer((ts) ),
	(Vec)PetscToPointer((xl) ),
	(Vec)PetscToPointer((xu) ));
}
void PETSC_STDCALL  tscomputelinearstability_(TS ts,PetscReal *xr,PetscReal *xi,PetscReal *yr,PetscReal *yi, int *__ierr ){
*__ierr = TSComputeLinearStability(
	(TS)PetscToPointer((ts) ),*xr,*xi,yr,yi);
}
#if defined(__cplusplus)
}
#endif
