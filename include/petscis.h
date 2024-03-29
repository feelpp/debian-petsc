/*
   An index set is a generalization of a subset of integers.  Index sets
   are used for defining scatters and gathers.
*/
#if !defined(__PETSCIS_H)
#define __PETSCIS_H
#include <petscsys.h>
#include <petscviewertypes.h>
#include <petscsftypes.h>

#define IS_FILE_CLASSID 1211218
PETSC_EXTERN PetscClassId IS_CLASSID;

PETSC_EXTERN PetscErrorCode ISInitializePackage(void);

/*S
     IS - Abstract PETSc object that allows indexing.

   Level: beginner

  Concepts: indexing, stride

.seealso:  ISCreateGeneral(), ISCreateBlock(), ISCreateStride(), ISGetIndices(), ISDestroy()
S*/
typedef struct _p_IS* IS;

/*J
    ISType - String with the name of a PETSc index set type

   Level: beginner

.seealso: ISSetType(), IS, ISCreate(), ISRegister()
J*/
typedef const char* ISType;
#define ISGENERAL      "general"
#define ISSTRIDE       "stride"
#define ISBLOCK        "block"

/* Dynamic creation and loading functions */
PETSC_EXTERN PetscFunctionList ISList;
PETSC_EXTERN PetscBool         ISRegisterAllCalled;
PETSC_EXTERN PetscErrorCode ISSetType(IS, ISType);
PETSC_EXTERN PetscErrorCode ISGetType(IS, ISType *);
PETSC_EXTERN PetscErrorCode ISRegister(const char[],PetscErrorCode (*)(IS));
PETSC_EXTERN PetscErrorCode ISRegisterAll(void);
PETSC_EXTERN PetscErrorCode ISCreate(MPI_Comm,IS*);

/*
    Default index set data structures that PETSc provides.
*/
PETSC_EXTERN PetscErrorCode ISCreateGeneral(MPI_Comm,PetscInt,const PetscInt[],PetscCopyMode,IS *);
PETSC_EXTERN PetscErrorCode ISGeneralSetIndices(IS,PetscInt,const PetscInt[],PetscCopyMode);
PETSC_EXTERN PetscErrorCode ISCreateBlock(MPI_Comm,PetscInt,PetscInt,const PetscInt[],PetscCopyMode,IS *);
PETSC_EXTERN PetscErrorCode ISBlockSetIndices(IS,PetscInt,PetscInt,const PetscInt[],PetscCopyMode);
PETSC_EXTERN PetscErrorCode ISCreateStride(MPI_Comm,PetscInt,PetscInt,PetscInt,IS *);
PETSC_EXTERN PetscErrorCode ISStrideSetStride(IS,PetscInt,PetscInt,PetscInt);

PETSC_EXTERN PetscErrorCode ISDestroy(IS*);
PETSC_EXTERN PetscErrorCode ISSetPermutation(IS);
PETSC_EXTERN PetscErrorCode ISPermutation(IS,PetscBool *);
PETSC_EXTERN PetscErrorCode ISSetIdentity(IS);
PETSC_EXTERN PetscErrorCode ISIdentity(IS,PetscBool *);
PETSC_EXTERN PetscErrorCode ISContiguousLocal(IS,PetscInt,PetscInt,PetscInt*,PetscBool*);

PETSC_EXTERN PetscErrorCode ISGetIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISRestoreIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISGetTotalIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISRestoreTotalIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISGetNonlocalIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISRestoreNonlocalIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISGetNonlocalIS(IS, IS *is);
PETSC_EXTERN PetscErrorCode ISRestoreNonlocalIS(IS, IS *is);
PETSC_EXTERN PetscErrorCode ISGetSize(IS,PetscInt *);
PETSC_EXTERN PetscErrorCode ISGetLocalSize(IS,PetscInt *);
PETSC_EXTERN PetscErrorCode ISInvertPermutation(IS,PetscInt,IS*);
PETSC_EXTERN PetscErrorCode ISView(IS,PetscViewer);
PETSC_EXTERN PetscErrorCode ISEqual(IS,IS,PetscBool  *);
PETSC_EXTERN PetscErrorCode ISSort(IS);
PETSC_EXTERN PetscErrorCode ISSorted(IS,PetscBool  *);
PETSC_EXTERN PetscErrorCode ISDifference(IS,IS,IS*);
PETSC_EXTERN PetscErrorCode ISSum(IS,IS,IS*);
PETSC_EXTERN PetscErrorCode ISExpand(IS,IS,IS*);
PETSC_EXTERN PetscErrorCode ISGetMinMax(IS,PetscInt*,PetscInt*);

PETSC_EXTERN PetscErrorCode ISBlockGetIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISBlockRestoreIndices(IS,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode ISBlockGetLocalSize(IS,PetscInt *);
PETSC_EXTERN PetscErrorCode ISBlockGetSize(IS,PetscInt *);
PETSC_EXTERN PetscErrorCode ISGetBlockSize(IS,PetscInt*);
PETSC_EXTERN PetscErrorCode ISSetBlockSize(IS,PetscInt);

PETSC_EXTERN PetscErrorCode ISStrideGetInfo(IS,PetscInt *,PetscInt*);

PETSC_EXTERN PetscErrorCode ISToGeneral(IS);

PETSC_EXTERN PetscErrorCode ISDuplicate(IS,IS*);
PETSC_EXTERN PetscErrorCode ISCopy(IS,IS);
PETSC_EXTERN PetscErrorCode ISAllGather(IS,IS*);
PETSC_EXTERN PetscErrorCode ISComplement(IS,PetscInt,PetscInt,IS*);
PETSC_EXTERN PetscErrorCode ISConcatenate(MPI_Comm,PetscInt,const IS[],IS*);
PETSC_EXTERN PetscErrorCode ISListToPair(MPI_Comm,PetscInt, IS[],IS*,IS*);
PETSC_EXTERN PetscErrorCode ISPairToList(IS,IS,PetscInt*, IS *[]);
PETSC_EXTERN PetscErrorCode ISEmbed(IS,IS,PetscBool,IS*);
PETSC_EXTERN PetscErrorCode ISOnComm(IS,MPI_Comm,PetscCopyMode,IS*);

/* --------------------------------------------------------------------------*/
PETSC_EXTERN PetscClassId IS_LTOGM_CLASSID;

/*S
   ISLocalToGlobalMapping - mappings from an arbitrary
      local ordering from 0 to n-1 to a global PETSc ordering
      used by a vector or matrix.

   Level: intermediate

   Note: mapping from Local to Global is scalable; but Global
  to Local may not be if the range of global values represented locally
  is very large.

   Note: the ISLocalToGlobalMapping is actually a private object; it is included
  here for the inline function ISLocalToGlobalMappingApply() to allow it to be inlined since
  it is used so often.

.seealso:  ISLocalToGlobalMappingCreate()
S*/
typedef struct _p_ISLocalToGlobalMapping* ISLocalToGlobalMapping;

/*E
    ISGlobalToLocalMappingType - Indicates if missing global indices are

   IS_GTOLM_MASK - missing global indices are replaced with -1
   IS_GTOLM_DROP - missing global indices are dropped

   Level: beginner

.seealso: ISGlobalToLocalMappingApply()

E*/
typedef enum {IS_GTOLM_MASK,IS_GTOLM_DROP} ISGlobalToLocalMappingType;

PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingCreate(MPI_Comm,PetscInt,const PetscInt[],PetscCopyMode,ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingCreateIS(IS,ISLocalToGlobalMapping *);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingCreateSF(PetscSF,PetscInt,ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingView(ISLocalToGlobalMapping,PetscViewer);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingDestroy(ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingApplyIS(ISLocalToGlobalMapping,IS,IS*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingApply(ISLocalToGlobalMapping,PetscInt,const PetscInt[],PetscInt[]);
PETSC_EXTERN PetscErrorCode ISGlobalToLocalMappingApply(ISLocalToGlobalMapping,ISGlobalToLocalMappingType,PetscInt,const PetscInt[],PetscInt*,PetscInt[]);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingGetSize(ISLocalToGlobalMapping,PetscInt*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingGetInfo(ISLocalToGlobalMapping,PetscInt*,PetscInt*[],PetscInt*[],PetscInt**[]);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingRestoreInfo(ISLocalToGlobalMapping,PetscInt*,PetscInt*[],PetscInt*[],PetscInt**[]);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingGetIndices(ISLocalToGlobalMapping,const PetscInt**);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingRestoreIndices(ISLocalToGlobalMapping,const PetscInt**);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingBlock(ISLocalToGlobalMapping,PetscInt,ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingUnBlock(ISLocalToGlobalMapping,PetscInt,ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode ISLocalToGlobalMappingConcatenate(MPI_Comm,PetscInt,const ISLocalToGlobalMapping[],ISLocalToGlobalMapping*);
PETSC_EXTERN PetscErrorCode ISG2LMapApply(ISLocalToGlobalMapping,PetscInt,const PetscInt[],PetscInt[]);

/* --------------------------------------------------------------------------*/
/*E
    ISColoringType - determines if the coloring is for the entire parallel grid/graph/matrix
                     or for just the local ghosted portion

    Level: beginner

$   IS_COLORING_GLOBAL - does not include the colors for ghost points, this is used when the function
$                        is called synchronously in parallel. This requires generating a "parallel coloring".
$   IS_COLORING_GHOSTED - includes colors for ghost points, this is used when the function can be called
$                         seperately on individual processes with the ghost points already filled in. Does not
$                         require a "parallel coloring", rather each process colors its local + ghost part.
$                         Using this can result in much less parallel communication. In the paradigm of
$                         DMGetLocalVector() and DMGetGlobalVector() this could be called IS_COLORING_LOCAL

.seealso: DMCreateColoring()
E*/
typedef enum {IS_COLORING_GLOBAL,IS_COLORING_GHOSTED} ISColoringType;
PETSC_EXTERN const char *const ISColoringTypes[];
typedef unsigned PETSC_IS_COLOR_VALUE_TYPE ISColoringValue;
PETSC_EXTERN PetscErrorCode ISAllGatherColors(MPI_Comm,PetscInt,ISColoringValue*,PetscInt*,ISColoringValue*[]);

/*S
     ISColoring - sets of IS's that define a coloring
              of the underlying indices

   Level: intermediate

    Notes:
        One should not access the *is records below directly because they may not yet
    have been created. One should use ISColoringGetIS() to make sure they are
    created when needed.

    Developer Note: this is not a PetscObject

.seealso:  ISColoringCreate(), ISColoringGetIS(), ISColoringView(), ISColoringGetIS()
S*/
struct _n_ISColoring {
  PetscInt        refct;
  PetscInt        n;                /* number of colors */
  IS              *is;              /* for each color indicates columns */
  MPI_Comm        comm;
  ISColoringValue *colors;          /* for each column indicates color */
  PetscInt        N;                /* number of columns */
  ISColoringType  ctype;
};
typedef struct _n_ISColoring* ISColoring;

PETSC_EXTERN PetscErrorCode ISColoringCreate(MPI_Comm,PetscInt,PetscInt,const ISColoringValue[],ISColoring*);
PETSC_EXTERN PetscErrorCode ISColoringDestroy(ISColoring*);
PETSC_EXTERN PetscErrorCode ISColoringView(ISColoring,PetscViewer);
PETSC_EXTERN PetscErrorCode ISColoringGetIS(ISColoring,PetscInt*,IS*[]);
PETSC_EXTERN PetscErrorCode ISColoringRestoreIS(ISColoring,IS*[]);
PETSC_EXTERN PetscErrorCode ISColoringReference(ISColoring);
PETSC_EXTERN PetscErrorCode ISColoringSetType(ISColoring,ISColoringType);


/* --------------------------------------------------------------------------*/

PETSC_EXTERN PetscErrorCode ISPartitioningToNumbering(IS,IS*);
PETSC_EXTERN PetscErrorCode ISPartitioningCount(IS,PetscInt,PetscInt[]);

PETSC_EXTERN PetscErrorCode ISCompressIndicesGeneral(PetscInt,PetscInt,PetscInt,PetscInt,const IS[],IS[]);
PETSC_EXTERN PetscErrorCode ISCompressIndicesSorted(PetscInt,PetscInt,PetscInt,const IS[],IS[]);
PETSC_EXTERN PetscErrorCode ISExpandIndicesGeneral(PetscInt,PetscInt,PetscInt,PetscInt,const IS[],IS[]);

/*S
     PetscLayout - defines layout of vectors and matrices across processes (which rows are owned by which processes)

   Level: developer


.seealso:  PetscLayoutCreate(), PetscLayoutDestroy()
S*/
typedef struct _n_PetscLayout* PetscLayout;
struct _n_PetscLayout{
  MPI_Comm               comm;
  PetscInt               n,N;         /* local, global vector size */
  PetscInt               rstart,rend; /* local start, local end + 1 */
  PetscInt               *range;      /* the offset of each processor */
  PetscInt               bs;          /* number of elements in each block (generally for multi-component problems) Do NOT multiply above numbers by bs */
  PetscInt               refcnt;      /* MPI Vecs obtained with VecDuplicate() and from MatGetVecs() reuse map of input object */
  ISLocalToGlobalMapping mapping;     /* mapping used in Vec/MatSetValuesLocal() */
  ISLocalToGlobalMapping bmapping;    /* mapping used in Vec/MatSetValuesBlockedLocal() */
  PetscInt               *trstarts;   /* local start for each thread */
};

PETSC_EXTERN PetscErrorCode PetscLayoutCreate(MPI_Comm,PetscLayout*);
PETSC_EXTERN PetscErrorCode PetscLayoutSetUp(PetscLayout);
PETSC_EXTERN PetscErrorCode PetscLayoutDestroy(PetscLayout*);
PETSC_EXTERN PetscErrorCode PetscLayoutDuplicate(PetscLayout,PetscLayout*);
PETSC_EXTERN PetscErrorCode PetscLayoutReference(PetscLayout,PetscLayout*);
PETSC_EXTERN PetscErrorCode PetscLayoutSetLocalSize(PetscLayout,PetscInt);
PETSC_EXTERN PetscErrorCode PetscLayoutGetLocalSize(PetscLayout,PetscInt *);
PETSC_EXTERN PetscErrorCode PetscLayoutSetSize(PetscLayout,PetscInt);
PETSC_EXTERN PetscErrorCode PetscLayoutGetSize(PetscLayout,PetscInt *);
PETSC_EXTERN PetscErrorCode PetscLayoutSetBlockSize(PetscLayout,PetscInt);
PETSC_EXTERN PetscErrorCode PetscLayoutGetBlockSize(PetscLayout,PetscInt*);
PETSC_EXTERN PetscErrorCode PetscLayoutGetRange(PetscLayout,PetscInt *,PetscInt *);
PETSC_EXTERN PetscErrorCode PetscLayoutGetRanges(PetscLayout,const PetscInt *[]);
PETSC_EXTERN PetscErrorCode PetscLayoutSetISLocalToGlobalMapping(PetscLayout,ISLocalToGlobalMapping);
PETSC_EXTERN PetscErrorCode PetscLayoutSetISLocalToGlobalMappingBlock(PetscLayout,ISLocalToGlobalMapping);
PETSC_EXTERN PetscErrorCode PetscSFSetGraphLayout(PetscSF,PetscLayout,PetscInt,const PetscInt*,PetscCopyMode,const PetscInt*);

#undef __FUNCT__
#define __FUNCT__ "PetscLayoutFindOwner"
/*@C
     PetscLayoutFindOwner - Find the owning rank for a global index

    Not Collective

   Input Parameters:
+    map - the layout
-    idx - global index to find the owner of

   Output Parameter:
.    owner - the owning rank

   Level: developer

    Fortran Notes:
      Not available from Fortran

@*/
PETSC_STATIC_INLINE PetscErrorCode PetscLayoutFindOwner(PetscLayout map,PetscInt idx,PetscInt *owner)
{
  PetscErrorCode ierr;
  PetscMPIInt    lo = 0,hi,t;

  PetscFunctionBegin;
  *owner = -1;                  /* GCC erroneously issues warning about possibly uninitialized use when error condition */
  if (!((map->n >= 0) && (map->N >= 0) && (map->range))) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONGSTATE,"PetscLayoutSetUp() must be called first");
  if (idx < 0 || idx > map->N) SETERRQ1(PETSC_COMM_SELF,PETSC_ERR_ARG_OUTOFRANGE,"Index %D is out of range",idx);
  ierr = MPI_Comm_size(map->comm,&hi);CHKERRQ(ierr);
  while (hi - lo > 1) {
    t = lo + (hi - lo) / 2;
    if (idx < map->range[t]) hi = t;
    else                     lo = t;
  }
  *owner = lo;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscLayoutFindOwnerIndex"
/*@C
     PetscLayoutFindOwnerIndex - Find the owning rank and the local index for a global index

    Not Collective

   Input Parameters:
+    map   - the layout
-    idx   - global index to find the owner of

   Output Parameter:
+    owner - the owning rank
-    lidx  - local index used by the owner for idx

   Level: developer

    Fortran Notes:
      Not available from Fortran

@*/
PETSC_STATIC_INLINE PetscErrorCode PetscLayoutFindOwnerIndex(PetscLayout map,PetscInt idx,PetscInt *owner, PetscInt *lidx)
{
  PetscErrorCode ierr;
  PetscMPIInt    lo = 0,hi,t;

  PetscFunctionBegin;
  if (!((map->n >= 0) && (map->N >= 0) && (map->range))) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_ARG_WRONGSTATE,"PetscLayoutSetUp() must be called first");
  if (idx < 0 || idx > map->N) SETERRQ1(PETSC_COMM_SELF,PETSC_ERR_ARG_OUTOFRANGE,"Index %D is out of range",idx);
  ierr = MPI_Comm_size(map->comm,&hi);CHKERRQ(ierr);
  while (hi - lo > 1) {
    t = lo + (hi - lo) / 2;
    if (idx < map->range[t]) hi = t;
    else                     lo = t;
  }
  *owner = lo;
  *lidx  = idx-map->range[lo];
  PetscFunctionReturn(0);
}

PETSC_EXTERN PetscClassId PETSC_SECTION_CLASSID;

/*S
  PetscSection - Mapping from integers in a designated range to contiguous sets of integers.

  In contrast to IS, which maps from integers to single integers, the range of a PetscSection is in the space of
  contiguous sets of integers. These ranges are frequently interpreted as domains of other array-like objects,
  especially other PetscSections, Vecs, and ISs. The domain is set with PetscSectionSetChart() and does not need to
  start at 0. For each point in the domain of a PetscSection, the output set is represented through an offset and a
  count, which are set using PetscSectionSetOffset() and PetscSectionSetDof() respectively. Lookup is typically using
  accessors or routines like VecGetValuesSection().

  Level: developer

.seealso:  PetscSectionCreate(), PetscSectionDestroy()
S*/
typedef struct _p_PetscSection *PetscSection;
PETSC_EXTERN PetscErrorCode PetscSectionCreate(MPI_Comm,PetscSection*);
PETSC_EXTERN PetscErrorCode PetscSectionClone(PetscSection, PetscSection*);
PETSC_EXTERN PetscErrorCode PetscSectionGetNumFields(PetscSection, PetscInt *);
PETSC_EXTERN PetscErrorCode PetscSectionSetNumFields(PetscSection, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetFieldName(PetscSection, PetscInt, const char *[]);
PETSC_EXTERN PetscErrorCode PetscSectionSetFieldName(PetscSection, PetscInt, const char []);
PETSC_EXTERN PetscErrorCode PetscSectionGetFieldComponents(PetscSection, PetscInt, PetscInt *);
PETSC_EXTERN PetscErrorCode PetscSectionSetFieldComponents(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetChart(PetscSection, PetscInt *, PetscInt *);
PETSC_EXTERN PetscErrorCode PetscSectionSetChart(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetDof(PetscSection, PetscInt, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetDof(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionAddDof(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetFieldDof(PetscSection, PetscInt, PetscInt, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetFieldDof(PetscSection, PetscInt, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionAddFieldDof(PetscSection, PetscInt, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetConstraintDof(PetscSection, PetscInt, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetConstraintDof(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionAddConstraintDof(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetFieldConstraintDof(PetscSection, PetscInt, PetscInt, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetFieldConstraintDof(PetscSection, PetscInt, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionAddFieldConstraintDof(PetscSection, PetscInt, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetConstraintIndices(PetscSection, PetscInt, const PetscInt**);
PETSC_EXTERN PetscErrorCode PetscSectionSetConstraintIndices(PetscSection, PetscInt, const PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionGetFieldConstraintIndices(PetscSection, PetscInt, PetscInt, const PetscInt**);
PETSC_EXTERN PetscErrorCode PetscSectionSetFieldConstraintIndices(PetscSection, PetscInt, PetscInt, const PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetUpBC(PetscSection);
PETSC_EXTERN PetscErrorCode PetscSectionSetUp(PetscSection);
PETSC_EXTERN PetscErrorCode PetscSectionGetMaxDof(PetscSection, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionGetStorageSize(PetscSection, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionGetConstrainedStorageSize(PetscSection, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionGetOffset(PetscSection, PetscInt, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetOffset(PetscSection, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetFieldOffset(PetscSection, PetscInt, PetscInt, PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSectionSetFieldOffset(PetscSection, PetscInt, PetscInt, PetscInt);
PETSC_EXTERN PetscErrorCode PetscSectionGetOffsetRange(PetscSection, PetscInt *, PetscInt *);
PETSC_EXTERN PetscErrorCode PetscSectionView(PetscSection, PetscViewer);
PETSC_EXTERN PetscErrorCode PetscSectionReset(PetscSection);
PETSC_EXTERN PetscErrorCode PetscSectionDestroy(PetscSection*);
PETSC_EXTERN PetscErrorCode PetscSectionCreateGlobalSection(PetscSection, PetscSF, PetscBool, PetscSection *);
PETSC_EXTERN PetscErrorCode PetscSectionCreateGlobalSectionCensored(PetscSection, PetscSF, PetscBool, PetscInt, const PetscInt [], PetscSection *);
PETSC_EXTERN PetscErrorCode PetscSectionCreateSubsection(PetscSection, PetscInt, PetscInt [], PetscSection *);
PETSC_EXTERN PetscErrorCode PetscSectionCreateSubmeshSection(PetscSection, IS, PetscSection *);
PETSC_EXTERN PetscErrorCode PetscSectionGetPointLayout(MPI_Comm, PetscSection, PetscLayout *);
PETSC_EXTERN PetscErrorCode PetscSectionGetValueLayout(MPI_Comm, PetscSection, PetscLayout *);

/* PetscSF support */
PETSC_EXTERN PetscErrorCode PetscSFConvertPartition(PetscSF, PetscSection, IS, ISLocalToGlobalMapping *, PetscSF *);
PETSC_EXTERN PetscErrorCode PetscSFCreateRemoteOffsets(PetscSF, PetscSection, PetscSection, PetscInt **);
PETSC_EXTERN PetscErrorCode PetscSFDistributeSection(PetscSF, PetscSection, PetscInt **, PetscSection);
PETSC_EXTERN PetscErrorCode PetscSFCreateSectionSF(PetscSF, PetscSection, PetscInt [], PetscSection, PetscSF *);

/* Reset __FUNCT__ in case the user does not define it themselves */
#undef __FUNCT__
#define __FUNCT__ "User provided function"

#endif
