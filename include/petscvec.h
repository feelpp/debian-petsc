/*
    Defines the vector component of PETSc. Vectors generally represent
  degrees of freedom for finite element/finite difference functions
  on a grid. They have more mathematical structure then simple arrays.
*/

#ifndef __PETSCVEC_H
#define __PETSCVEC_H
#include <petscis.h>
#include <petscviewer.h>

/*S
     Vec - Abstract PETSc vector object

   Level: beginner

  Concepts: field variables, unknowns, arrays

.seealso:  VecCreate(), VecType, VecSetType()
S*/
typedef struct _p_Vec*         Vec;

/*S
     VecScatter - Object used to manage communication of data
       between vectors in parallel. Manages both scatters and gathers

   Level: beginner

  Concepts: scatter

.seealso:  VecScatterCreate(), VecScatterBegin(), VecScatterEnd()
S*/
typedef struct _p_VecScatter*  VecScatter;

/*E
  ScatterMode - Determines the direction of a scatter

  Level: beginner

.seealso: VecScatter, VecScatterBegin(), VecScatterEnd()
E*/
typedef enum {SCATTER_FORWARD=0, SCATTER_REVERSE=1, SCATTER_FORWARD_LOCAL=2, SCATTER_REVERSE_LOCAL=3, SCATTER_LOCAL=2} ScatterMode;

/*MC
    SCATTER_FORWARD - Scatters the values as dictated by the VecScatterCreate() call

    Level: beginner

.seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_REVERSE, SCATTER_FORWARD_LOCAL,
          SCATTER_REVERSE_LOCAL

M*/

/*MC
    SCATTER_REVERSE - Moves the values in the opposite direction then the directions indicated in
         in the VecScatterCreate()

    Level: beginner

.seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_FORWARD, SCATTER_FORWARD_LOCAL,
          SCATTER_REVERSE_LOCAL

M*/

/*MC
    SCATTER_FORWARD_LOCAL - Scatters the values as dictated by the VecScatterCreate() call except NO parallel communication
       is done. Any variables that have be moved between processes are ignored

    Level: developer

.seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_REVERSE, SCATTER_FORWARD,
          SCATTER_REVERSE_LOCAL

M*/

/*MC
    SCATTER_REVERSE_LOCAL - Moves the values in the opposite direction then the directions indicated in
         in the VecScatterCreate()  except NO parallel communication
       is done. Any variables that have be moved between processes are ignored

    Level: developer

.seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_FORWARD, SCATTER_FORWARD_LOCAL,
          SCATTER_REVERSE

M*/

/*J
    VecType - String with the name of a PETSc vector

   Level: beginner

.seealso: VecSetType(), Vec, VecCreate(), VecDestroy()
J*/
typedef const char* VecType;
#define VECSEQ         "seq"
#define VECMPI         "mpi"
#define VECSTANDARD    "standard"   /* seq on one process and mpi on several */
#define VECSHARED      "shared"
#define VECSIEVE       "sieve"
#define VECSEQCUSP     "seqcusp"
#define VECMPICUSP     "mpicusp"
#define VECCUSP        "cusp"       /* seqcusp on one process and mpicusp on several */
#define VECNEST        "nest"
#define VECSEQPTHREAD  "seqpthread"
#define VECMPIPTHREAD  "mpipthread"
#define VECPTHREAD     "pthread"    /* seqpthread on one process and mpipthread on several */


/* Logging support */
#define    VEC_FILE_CLASSID 1211214
PETSC_EXTERN PetscClassId VEC_CLASSID;
PETSC_EXTERN PetscClassId VEC_SCATTER_CLASSID;


PETSC_EXTERN PetscErrorCode VecInitializePackage(void);
PETSC_EXTERN PetscErrorCode VecFinalizePackage(void);

PETSC_EXTERN PetscErrorCode VecCreate(MPI_Comm,Vec*);
PETSC_EXTERN PetscErrorCode VecCreateSeq(MPI_Comm,PetscInt,Vec*);
PETSC_EXTERN PetscErrorCode VecCreateMPI(MPI_Comm,PetscInt,PetscInt,Vec*);
PETSC_EXTERN PetscErrorCode VecCreateSeqWithArray(MPI_Comm,PetscInt,PetscInt,const PetscScalar[],Vec*);
PETSC_EXTERN PetscErrorCode VecCreateMPIWithArray(MPI_Comm,PetscInt,PetscInt,PetscInt,const PetscScalar[],Vec*);
PETSC_EXTERN PetscErrorCode VecCreateShared(MPI_Comm,PetscInt,PetscInt,Vec*);
PETSC_EXTERN PetscErrorCode VecSetFromOptions(Vec);
PETSC_EXTERN PetscErrorCode VecViewFromOptions(Vec,const char[],const char[]);

PETSC_EXTERN PetscErrorCode VecSetUp(Vec);
PETSC_EXTERN PetscErrorCode VecDestroy(Vec*);
PETSC_EXTERN PetscErrorCode VecZeroEntries(Vec);
PETSC_EXTERN PetscErrorCode VecSetOptionsPrefix(Vec,const char[]);
PETSC_EXTERN PetscErrorCode VecAppendOptionsPrefix(Vec,const char[]);
PETSC_EXTERN PetscErrorCode VecGetOptionsPrefix(Vec,const char*[]);

PETSC_EXTERN PetscErrorCode VecSetSizes(Vec,PetscInt,PetscInt);

PETSC_EXTERN PetscErrorCode VecDotNorm2(Vec,Vec,PetscScalar*,PetscReal*);
PETSC_EXTERN PetscErrorCode VecDot(Vec,Vec,PetscScalar*);
PETSC_EXTERN PetscErrorCode VecDotRealPart(Vec,Vec,PetscReal*);
PETSC_EXTERN PetscErrorCode VecTDot(Vec,Vec,PetscScalar*);
PETSC_EXTERN PetscErrorCode VecMDot(Vec,PetscInt,const Vec[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecMTDot(Vec,PetscInt,const Vec[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecGetSubVector(Vec,IS,Vec*);
PETSC_EXTERN PetscErrorCode VecRestoreSubVector(Vec,IS,Vec*);

/*E
    NormType - determines what type of norm to compute

    Level: beginner

.seealso: VecNorm(), VecNormBegin(), VecNormEnd(), MatNorm()
E*/
typedef enum {NORM_1=0,NORM_2=1,NORM_FROBENIUS=2,NORM_INFINITY=3,NORM_1_AND_2=4} NormType;
PETSC_EXTERN const char *const NormTypes[];
#define NORM_MAX NORM_INFINITY

/*MC
     NORM_1 - the one norm, ||v|| = sum_i | v_i |. ||A|| = max_j || v_*j ||, maximum column sum

   Level: beginner

.seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_2, NORM_FROBENIUS,
           NORM_INFINITY, NORM_1_AND_2

M*/

/*MC
     NORM_2 - the two norm, ||v|| = sqrt(sum_i (v_i)^2) (vectors only)

   Level: beginner

.seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_FROBENIUS,
           NORM_INFINITY, NORM_1_AND_2

M*/

/*MC
     NORM_FROBENIUS - ||A|| = sqrt(sum_ij (A_ij)^2), same as NORM_2 for vectors

   Level: beginner

.seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_2,
           NORM_INFINITY, NORM_1_AND_2

M*/

/*MC
     NORM_INFINITY - ||v|| = max_i |v_i|. ||A|| = max_i || v_i* ||, maximum row sum

   Level: beginner

.seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_2,
           NORM_FROBINIUS, NORM_1_AND_2

M*/

/*MC
     NORM_1_AND_2 - computes both the 1 and 2 norm of a vector

   Level: beginner

.seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_2,
           NORM_FROBINIUS, NORM_INFINITY

M*/

/*MC
     NORM_MAX - see NORM_INFINITY

   Level: beginner

M*/

PETSC_EXTERN PetscErrorCode VecNorm(Vec,NormType,PetscReal *);
PETSC_EXTERN PetscErrorCode VecNormAvailable(Vec,NormType,PetscBool *,PetscReal *);
PETSC_EXTERN PetscErrorCode VecNormalize(Vec,PetscReal *);
PETSC_EXTERN PetscErrorCode VecSum(Vec,PetscScalar*);
PETSC_EXTERN PetscErrorCode VecMax(Vec,PetscInt*,PetscReal *);
PETSC_EXTERN PetscErrorCode VecMin(Vec,PetscInt*,PetscReal *);
PETSC_EXTERN PetscErrorCode VecScale(Vec,PetscScalar);
PETSC_EXTERN PetscErrorCode VecCopy(Vec,Vec);
PETSC_EXTERN PetscErrorCode VecSetRandom(Vec,PetscRandom);
PETSC_EXTERN PetscErrorCode VecSet(Vec,PetscScalar);
PETSC_EXTERN PetscErrorCode VecSwap(Vec,Vec);
PETSC_EXTERN PetscErrorCode VecAXPY(Vec,PetscScalar,Vec);
PETSC_EXTERN PetscErrorCode VecAXPBY(Vec,PetscScalar,PetscScalar,Vec);
PETSC_EXTERN PetscErrorCode VecMAXPY(Vec,PetscInt,const PetscScalar[],Vec[]);
PETSC_EXTERN PetscErrorCode VecAYPX(Vec,PetscScalar,Vec);
PETSC_EXTERN PetscErrorCode VecWAXPY(Vec,PetscScalar,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecAXPBYPCZ(Vec,PetscScalar,PetscScalar,PetscScalar,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecPointwiseMax(Vec,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecPointwiseMaxAbs(Vec,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecPointwiseMin(Vec,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecPointwiseMult(Vec,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecPointwiseDivide(Vec,Vec,Vec);
PETSC_EXTERN PetscErrorCode VecMaxPointwiseDivide(Vec,Vec,PetscReal*);
PETSC_EXTERN PetscErrorCode VecShift(Vec,PetscScalar);
PETSC_EXTERN PetscErrorCode VecReciprocal(Vec);
PETSC_EXTERN PetscErrorCode VecPermute(Vec, IS, PetscBool );
PETSC_EXTERN PetscErrorCode VecSqrtAbs(Vec);
PETSC_EXTERN PetscErrorCode VecLog(Vec);
PETSC_EXTERN PetscErrorCode VecExp(Vec);
PETSC_EXTERN PetscErrorCode VecAbs(Vec);
PETSC_EXTERN PetscErrorCode VecDuplicate(Vec,Vec*);
PETSC_EXTERN PetscErrorCode VecDuplicateVecs(Vec,PetscInt,Vec*[]);
PETSC_EXTERN PetscErrorCode VecDestroyVecs(PetscInt, Vec*[]);
PETSC_EXTERN PetscErrorCode VecStrideNormAll(Vec,NormType,PetscReal[]);
PETSC_EXTERN PetscErrorCode VecStrideMaxAll(Vec,PetscInt [],PetscReal []);
PETSC_EXTERN PetscErrorCode VecStrideMinAll(Vec,PetscInt [],PetscReal []);
PETSC_EXTERN PetscErrorCode VecStrideScaleAll(Vec,const PetscScalar[]);

PETSC_EXTERN PetscErrorCode VecStrideNorm(Vec,PetscInt,NormType,PetscReal*);
PETSC_EXTERN PetscErrorCode VecStrideMax(Vec,PetscInt,PetscInt *,PetscReal *);
PETSC_EXTERN PetscErrorCode VecStrideMin(Vec,PetscInt,PetscInt *,PetscReal *);
PETSC_EXTERN PetscErrorCode VecStrideScale(Vec,PetscInt,PetscScalar);
PETSC_EXTERN PetscErrorCode VecStrideSet(Vec,PetscInt,PetscScalar);


PETSC_EXTERN PetscErrorCode VecStrideGather(Vec,PetscInt,Vec,InsertMode);
PETSC_EXTERN PetscErrorCode VecStrideScatter(Vec,PetscInt,Vec,InsertMode);
PETSC_EXTERN PetscErrorCode VecStrideGatherAll(Vec,Vec[],InsertMode);
PETSC_EXTERN PetscErrorCode VecStrideScatterAll(Vec[],Vec,InsertMode);

PETSC_EXTERN PetscErrorCode VecSetValues(Vec,PetscInt,const PetscInt[],const PetscScalar[],InsertMode);
PETSC_EXTERN PetscErrorCode VecGetValues(Vec,PetscInt,const PetscInt[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecAssemblyBegin(Vec);
PETSC_EXTERN PetscErrorCode VecAssemblyEnd(Vec);
PETSC_EXTERN PetscErrorCode VecStashSetInitialSize(Vec,PetscInt,PetscInt);
PETSC_EXTERN PetscErrorCode VecStashView(Vec,PetscViewer);
PETSC_EXTERN PetscErrorCode VecStashGetInfo(Vec,PetscInt*,PetscInt*,PetscInt*,PetscInt*);

/*MC
   VecSetValue - Set a single entry into a vector.

   Synopsis:
   #include "petscvec.h"
   PetscErrorCode VecSetValue(Vec v,PetscInt row,PetscScalar value, InsertMode mode);

   Not Collective

   Input Parameters:
+  v - the vector
.  row - the row location of the entry
.  value - the value to insert
-  mode - either INSERT_VALUES or ADD_VALUES

   Notes:
   For efficiency one should use VecSetValues() and set several or
   many values simultaneously if possible.

   These values may be cached, so VecAssemblyBegin() and VecAssemblyEnd()
   MUST be called after all calls to VecSetValues() have been completed.

   VecSetValues() uses 0-based indices in Fortran as well as in C.

   Level: beginner

.seealso: VecSetValues(), VecAssemblyBegin(), VecAssemblyEnd(), VecSetValuesBlockedLocal(), VecSetValueLocal()
M*/
PETSC_STATIC_INLINE PetscErrorCode VecSetValue(Vec v,PetscInt i,PetscScalar va,InsertMode mode) {return VecSetValues(v,1,&i,&va,mode);}


PETSC_EXTERN PetscErrorCode VecSetBlockSize(Vec,PetscInt);
PETSC_EXTERN PetscErrorCode VecGetBlockSize(Vec,PetscInt*);
PETSC_EXTERN PetscErrorCode VecSetValuesBlocked(Vec,PetscInt,const PetscInt[],const PetscScalar[],InsertMode);

/* Dynamic creation and loading functions */
PETSC_EXTERN PetscFunctionList VecList;
PETSC_EXTERN PetscBool         VecRegisterAllCalled;
PETSC_EXTERN PetscErrorCode VecSetType(Vec, VecType);
PETSC_EXTERN PetscErrorCode VecGetType(Vec, VecType *);
PETSC_EXTERN PetscErrorCode VecRegister(const char[],PetscErrorCode (*)(Vec));
PETSC_EXTERN PetscErrorCode VecRegisterAll(void);

PETSC_EXTERN PetscErrorCode VecScatterCreate(Vec,IS,Vec,IS,VecScatter *);
PETSC_EXTERN PetscErrorCode VecScatterCreateEmpty(MPI_Comm,VecScatter *);
PETSC_EXTERN PetscErrorCode VecScatterCreateLocal(VecScatter,PetscInt,const PetscInt[],const PetscInt[],const PetscInt[],PetscInt,const PetscInt[],const PetscInt[],const PetscInt[],PetscInt);
PETSC_EXTERN PetscErrorCode VecScatterBegin(VecScatter,Vec,Vec,InsertMode,ScatterMode);
PETSC_EXTERN PetscErrorCode VecScatterEnd(VecScatter,Vec,Vec,InsertMode,ScatterMode);
PETSC_EXTERN PetscErrorCode VecScatterDestroy(VecScatter*);
PETSC_EXTERN PetscErrorCode VecScatterCopy(VecScatter,VecScatter *);
PETSC_EXTERN PetscErrorCode VecScatterView(VecScatter,PetscViewer);
PETSC_EXTERN PetscErrorCode VecScatterRemap(VecScatter,PetscInt *,PetscInt*);
PETSC_EXTERN PetscErrorCode VecScatterGetMerged(VecScatter,PetscBool *);

PETSC_EXTERN PetscErrorCode VecGetArray4d(Vec,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscScalar****[]);
PETSC_EXTERN PetscErrorCode VecRestoreArray4d(Vec,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscScalar****[]);
PETSC_EXTERN PetscErrorCode VecGetArray3d(Vec,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscScalar***[]);
PETSC_EXTERN PetscErrorCode VecRestoreArray3d(Vec,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscInt,PetscScalar***[]);
PETSC_EXTERN PetscErrorCode VecGetArray2d(Vec,PetscInt,PetscInt,PetscInt,PetscInt,PetscScalar**[]);
PETSC_EXTERN PetscErrorCode VecRestoreArray2d(Vec,PetscInt,PetscInt,PetscInt,PetscInt,PetscScalar**[]);
PETSC_EXTERN PetscErrorCode VecGetArray1d(Vec,PetscInt,PetscInt,PetscScalar *[]);
PETSC_EXTERN PetscErrorCode VecRestoreArray1d(Vec,PetscInt,PetscInt,PetscScalar *[]);

PETSC_EXTERN PetscErrorCode VecPlaceArray(Vec,const PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecResetArray(Vec);
PETSC_EXTERN PetscErrorCode VecReplaceArray(Vec,const PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecGetArrays(const Vec[],PetscInt,PetscScalar**[]);
PETSC_EXTERN PetscErrorCode VecRestoreArrays(const Vec[],PetscInt,PetscScalar**[]);

PETSC_EXTERN PetscErrorCode VecView(Vec,PetscViewer);
PETSC_EXTERN PetscErrorCode VecEqual(Vec,Vec,PetscBool *);
PETSC_EXTERN PetscErrorCode VecLoad(Vec, PetscViewer);

PETSC_EXTERN PetscErrorCode VecGetSize(Vec,PetscInt*);
PETSC_EXTERN PetscErrorCode VecGetLocalSize(Vec,PetscInt*);
PETSC_EXTERN PetscErrorCode VecGetOwnershipRange(Vec,PetscInt*,PetscInt*);
PETSC_EXTERN PetscErrorCode VecGetOwnershipRanges(Vec,const PetscInt *[]);

PETSC_EXTERN PetscErrorCode VecSetLocalToGlobalMapping(Vec,ISLocalToGlobalMapping);
PETSC_EXTERN PetscErrorCode VecSetValuesLocal(Vec,PetscInt,const PetscInt[],const PetscScalar[],InsertMode);

/*MC
   VecSetValueLocal - Set a single entry into a vector using the local numbering

   Synopsis:
   #include "petscvec.h"
   PetscErrorCode VecSetValueLocal(Vec v,PetscInt row,PetscScalar value, InsertMode mode);

   Not Collective

   Input Parameters:
+  v - the vector
.  row - the row location of the entry
.  value - the value to insert
-  mode - either INSERT_VALUES or ADD_VALUES

   Notes:
   For efficiency one should use VecSetValues() and set several or
   many values simultaneously if possible.

   These values may be cached, so VecAssemblyBegin() and VecAssemblyEnd()
   MUST be called after all calls to VecSetValues() have been completed.

   VecSetValues() uses 0-based indices in Fortran as well as in C.

   Level: beginner

.seealso: VecSetValues(), VecAssemblyBegin(), VecAssemblyEnd(), VecSetValuesBlockedLocal(), VecSetValue()
M*/
PETSC_STATIC_INLINE PetscErrorCode VecSetValueLocal(Vec v,PetscInt i,PetscScalar va,InsertMode mode) {return VecSetValuesLocal(v,1,&i,&va,mode);}

PETSC_EXTERN PetscErrorCode VecSetLocalToGlobalMappingBlock(Vec,ISLocalToGlobalMapping);
PETSC_EXTERN PetscErrorCode VecSetValuesBlockedLocal(Vec,PetscInt,const PetscInt[],const PetscScalar[],InsertMode);
PETSC_EXTERN PetscErrorCode VecGetLocalToGlobalMappingBlock(Vec,ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode VecGetLocalToGlobalMapping(Vec,ISLocalToGlobalMapping*);

PETSC_EXTERN PetscErrorCode VecDotBegin(Vec,Vec,PetscScalar *);
PETSC_EXTERN PetscErrorCode VecDotEnd(Vec,Vec,PetscScalar *);
PETSC_EXTERN PetscErrorCode VecTDotBegin(Vec,Vec,PetscScalar *);
PETSC_EXTERN PetscErrorCode VecTDotEnd(Vec,Vec,PetscScalar *);
PETSC_EXTERN PetscErrorCode VecNormBegin(Vec,NormType,PetscReal *);
PETSC_EXTERN PetscErrorCode VecNormEnd(Vec,NormType,PetscReal *);

PETSC_EXTERN PetscErrorCode VecMDotBegin(Vec,PetscInt,const Vec[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecMDotEnd(Vec,PetscInt,const Vec[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecMTDotBegin(Vec,PetscInt,const Vec[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode VecMTDotEnd(Vec,PetscInt,const Vec[],PetscScalar[]);
PETSC_EXTERN PetscErrorCode PetscCommSplitReductionBegin(MPI_Comm);


typedef enum {VEC_IGNORE_OFF_PROC_ENTRIES,VEC_IGNORE_NEGATIVE_INDICES} VecOption;
PETSC_EXTERN PetscErrorCode VecSetOption(Vec,VecOption,PetscBool );

PETSC_EXTERN PetscErrorCode VecGetArray(Vec,PetscScalar**);
PETSC_EXTERN PetscErrorCode VecGetArrayRead(Vec,const PetscScalar**);
PETSC_EXTERN PetscErrorCode VecRestoreArray(Vec,PetscScalar**);
PETSC_EXTERN PetscErrorCode VecRestoreArrayRead(Vec,const PetscScalar**);

PETSC_EXTERN PetscErrorCode VecValidValues(Vec,PetscInt,PetscBool);

PETSC_EXTERN PetscErrorCode VecContourScale(Vec,PetscReal,PetscReal);

/*
    These numbers need to match the entries in
  the function table in vecimpl.h
*/
typedef enum { VECOP_VIEW = 33, VECOP_LOAD = 41, VECOP_DUPLICATE = 0} VecOperation;
PETSC_EXTERN PetscErrorCode VecSetOperation(Vec,VecOperation,void(*)(void));

/*
     Routines for dealing with ghosted vectors:
  vectors with ghost elements at the end of the array.
*/
PETSC_EXTERN PetscErrorCode VecMPISetGhost(Vec,PetscInt,const PetscInt[]);
PETSC_EXTERN PetscErrorCode VecCreateGhost(MPI_Comm,PetscInt,PetscInt,PetscInt,const PetscInt[],Vec*);
PETSC_EXTERN PetscErrorCode VecCreateGhostWithArray(MPI_Comm,PetscInt,PetscInt,PetscInt,const PetscInt[],const PetscScalar[],Vec*);
PETSC_EXTERN PetscErrorCode VecCreateGhostBlock(MPI_Comm,PetscInt,PetscInt,PetscInt,PetscInt,const PetscInt[],Vec*);
PETSC_EXTERN PetscErrorCode VecCreateGhostBlockWithArray(MPI_Comm,PetscInt,PetscInt,PetscInt,PetscInt,const PetscInt[],const PetscScalar[],Vec*);
PETSC_EXTERN PetscErrorCode VecGhostGetLocalForm(Vec,Vec*);
PETSC_EXTERN PetscErrorCode VecGhostRestoreLocalForm(Vec,Vec*);
PETSC_EXTERN PetscErrorCode VecGhostIsLocalForm(Vec,Vec,PetscBool*);
PETSC_EXTERN PetscErrorCode VecGhostUpdateBegin(Vec,InsertMode,ScatterMode);
PETSC_EXTERN PetscErrorCode VecGhostUpdateEnd(Vec,InsertMode,ScatterMode);

PETSC_EXTERN PetscErrorCode VecConjugate(Vec);

PETSC_EXTERN PetscErrorCode VecScatterCreateToAll(Vec,VecScatter*,Vec*);
PETSC_EXTERN PetscErrorCode VecScatterCreateToZero(Vec,VecScatter*,Vec*);

PETSC_EXTERN PetscErrorCode PetscViewerMathematicaGetVector(PetscViewer, Vec);
PETSC_EXTERN PetscErrorCode PetscViewerMathematicaPutVector(PetscViewer, Vec);

/*S
     Vecs - Collection of vectors where the data for the vectors is stored in
            one contiguous memory

   Level: advanced

   Notes:
    Temporary construct for handling multiply right hand side solves

    This is faked by storing a single vector that has enough array space for
    n vectors

  Concepts: parallel decomposition

S*/
        struct _n_Vecs  {PetscInt n; Vec v;};
typedef struct _n_Vecs* Vecs;
PETSC_EXTERN PetscErrorCode VecsDestroy(Vecs);
PETSC_EXTERN PetscErrorCode VecsCreateSeq(MPI_Comm,PetscInt,PetscInt,Vecs*);
PETSC_EXTERN PetscErrorCode VecsCreateSeqWithArray(MPI_Comm,PetscInt,PetscInt,PetscScalar*,Vecs*);
PETSC_EXTERN PetscErrorCode VecsDuplicate(Vecs,Vecs*);

#if defined(PETSC_HAVE_CUSP)
typedef struct _p_PetscCUSPIndices* PetscCUSPIndices;
PETSC_EXTERN PetscErrorCode PetscCUSPIndicesCreate(PetscInt, PetscInt*,PetscInt, PetscInt*,PetscCUSPIndices*);
PETSC_EXTERN PetscErrorCode PetscCUSPIndicesDestroy(PetscCUSPIndices*);
PETSC_EXTERN PetscErrorCode VecCUSPCopyToGPUSome_Public(Vec,PetscCUSPIndices);
PETSC_EXTERN PetscErrorCode VecCUSPCopyFromGPUSome_Public(Vec,PetscCUSPIndices);

#if defined(PETSC_HAVE_TXPETSCGPU)
PETSC_EXTERN PetscErrorCode VecCUSPResetIndexBuffersFlagsGPU_Public(PetscCUSPIndices);
PETSC_EXTERN PetscErrorCode VecCUSPCopySomeToContiguousBufferGPU_Public(Vec,PetscCUSPIndices);
PETSC_EXTERN PetscErrorCode VecCUSPCopySomeFromContiguousBufferGPU_Public(Vec,PetscCUSPIndices);
PETSC_EXTERN PetscErrorCode VecScatterInitializeForGPU(VecScatter,Vec,ScatterMode);
PETSC_EXTERN PetscErrorCode VecScatterFinalizeForGPU(VecScatter);
#endif

PETSC_EXTERN PetscErrorCode VecCreateSeqCUSP(MPI_Comm,PetscInt,Vec*);
PETSC_EXTERN PetscErrorCode VecCreateMPICUSP(MPI_Comm,PetscInt,PetscInt,Vec*);
#endif

PETSC_EXTERN PetscErrorCode VecNestGetSubVecs(Vec,PetscInt*,Vec**);
PETSC_EXTERN PetscErrorCode VecNestGetSubVec(Vec,PetscInt,Vec*);
PETSC_EXTERN PetscErrorCode VecNestSetSubVecs(Vec,PetscInt,PetscInt*,Vec*);
PETSC_EXTERN PetscErrorCode VecNestSetSubVec(Vec,PetscInt,Vec);
PETSC_EXTERN PetscErrorCode VecCreateNest(MPI_Comm,PetscInt,IS*,Vec*,Vec*);
PETSC_EXTERN PetscErrorCode VecNestGetSize(Vec,PetscInt*);

PETSC_EXTERN PetscErrorCode PetscOptionsVec(const char[],const char[],const char[],Vec,PetscBool*);
PETSC_EXTERN PetscErrorCode VecChop(Vec,PetscReal);

PETSC_EXTERN PetscErrorCode VecGetLayout(Vec,PetscLayout*);
PETSC_EXTERN PetscErrorCode VecSetLayout(Vec,PetscLayout);

PETSC_EXTERN PetscErrorCode PetscSectionVecView(PetscSection, Vec, PetscViewer);
PETSC_EXTERN PetscErrorCode VecGetValuesSection(Vec, PetscSection, PetscInt, PetscScalar **);
PETSC_EXTERN PetscErrorCode VecSetValuesSection(Vec, PetscSection, PetscInt, PetscScalar [], InsertMode);

#endif

