
#if !defined(__pcbddc_h)
#define __pcbddc_h

#include <../src/ksp/pc/impls/is/pcis.h>

/* BDDC requires metis 5.0.1 for multilevel */
#include "metis.h"
#define MetisInt    idx_t
#define MetisScalar real_t


/* Structure for graph partitioning (adapted from Metis) */
struct _PCBDDCGraph {
  PetscInt    nvtxs;
  PetscInt    ncmps;
  PetscInt    *xadj;
  PetscInt    *adjncy;
  PetscInt    **neighbours_set;
  PetscInt    *where;
  PetscInt    *which_dof;
  PetscInt    *cptr;
  PetscInt    *queue;
  PetscInt    *count;
  PetscMPIInt *where_ncmps;
  PetscBool   *touched;
};

typedef enum {SCATTERS_BDDC,GATHERS_BDDC} CoarseCommunicationsType;
typedef struct _PCBDDCGraph *PCBDDCGraph;

/* Private context (data structure) for the BDDC preconditioner.  */
typedef struct {
  /* First MUST come the folowing line, for the stuff that is common to FETI and Neumann-Neumann. */
  PC_IS pcis;

  /* Coarse stuffs needed by BDDC application in KSP */
  PetscInt    coarse_size;
  Mat         coarse_mat;
  Vec         coarse_vec;
  Vec         coarse_rhs;
  KSP         coarse_ksp;
  Mat         coarse_phi_B;
  Mat         coarse_phi_D;
  PetscInt    local_primal_size;
  PetscInt    *local_primal_indices;
  PetscMPIInt *local_primal_displacements;
  PetscMPIInt *local_primal_sizes;
  PetscMPIInt replicated_primal_size;
  PetscMPIInt *replicated_local_primal_indices;
  PetscScalar *replicated_local_primal_values;
  VecScatter  coarse_loc_to_glob;

  /* Local stuffs needed by BDDC application in KSP */
  Vec        vec1_P;
  Vec        vec1_C;
  Mat        local_auxmat1;
  Mat        local_auxmat2;
  Vec        vec1_R;
  Vec        vec2_R;
  VecScatter R_to_B;
  VecScatter R_to_D;
  KSP        ksp_R;
  KSP        ksp_D;
  Vec        vec4_D;

  /* Quantities defining constraining details (local) of the preconditioner */
  /* These quantities define the preconditioner itself */
  IS        *ISForFaces;
  IS        *ISForEdges;
  IS        ISForVertices;
  PetscInt  n_ISForFaces;
  PetscInt  n_ISForEdges;
  PetscInt  n_constraints;
  PetscInt  n_vertices;
  Mat       ConstraintMatrix;
  PetscBool usechangeofbasis;
  PetscBool usechangeonfaces;
  Mat       ChangeOfBasisMatrix;
  Vec       original_rhs;
  Vec       temp_solution;
  Mat       local_mat;
  PetscBool use_exact_dirichlet;

  /* Some defaults on selecting vertices and constraints*/
  PetscBool vertices_flag;
  PetscBool constraints_flag;
  PetscBool faces_flag;
  PetscBool edges_flag;

  /* Some customization is possible */
  PCBDDCGraph              mat_graph;
  MatNullSpace             NullSpace;
  MatNullSpace             CoarseNullSpace;
  PetscBool                use_nnsp_true;
  PetscInt                 n_ISForDofs;
  IS                       *ISForDofs;
  IS                       NeumannBoundaries;
  IS                       DirichletBoundaries;
  PetscBool                inexact_prec_type;
  CoarseProblemType        coarse_problem_type;
  CoarseCommunicationsType coarse_communications_type;
  PetscInt                 coarsening_ratio;
  PetscInt                 current_level;
  PetscInt                 max_levels;

  /* For verbose output of some bddc data structures */
  PetscBool   dbg_flag;
  PetscViewer dbg_viewer;
} PC_BDDC;

/* prototypes for functions contained in bddc.c */
static PetscErrorCode PCBDDCSetUseExactDirichlet(PC,PetscBool);
static PetscErrorCode PCBDDCSetLevel(PC,PetscInt);
static PetscErrorCode PCBDDCAdaptNullSpace(PC);
static PetscErrorCode PCBDDCCoarseSetUp(PC);
static PetscErrorCode PCBDDCFindConnectedComponents(PCBDDCGraph,PetscInt);
static PetscErrorCode PCBDDCSetupCoarseEnvironment(PC,PetscScalar*);
static PetscErrorCode PCBDDCManageLocalBoundaries(PC);
static PetscErrorCode PCBDDCApplyInterfacePreconditioner(PC);
static PetscErrorCode PCBDDCSolveSaddlePoint(PC);
static PetscErrorCode PCBDDCScatterCoarseDataBegin(PC,Vec,Vec,InsertMode,ScatterMode);
static PetscErrorCode PCBDDCScatterCoarseDataEnd(PC,Vec,Vec,InsertMode,ScatterMode);
static PetscErrorCode PCBDDCCreateConstraintMatrix(PC);

/* feti-dp */
typedef struct {
  PetscInt   n_lambda;
  Vec        lambda_local;
  Vec        temp_solution_B;
  Vec        temp_solution_D;
  Mat        B_delta;
  Mat        B_Ddelta;
  VecScatter l2g_lambda;
  PC         pc;
} FETIDPMat_ctx;

typedef struct {
  Vec        lambda_local;
  Mat        B_Ddelta;
  VecScatter l2g_lambda;
  PC         pc;
} FETIDPPC_ctx;

/* inexact solvers with nullspace correction */
typedef struct {
  Mat basis_mat;
  Mat Kbasis_mat;
  Mat Lbasis_mat;
  PC  local_pc;
  Vec work_small_1;
  Vec work_small_2;
  Vec work_full_1;
  Vec work_full_2;
} NullSpaceCorrection_ctx;

static PetscErrorCode PCBDDCAdaptLocalProblem(PC,IS);
static PetscErrorCode PCBDDCDestroyNullSpaceCorrectionPC(PC);
static PetscErrorCode PCBDDCApplyNullSpaceCorrectionPC(PC,Vec,Vec);
static PetscErrorCode PCBDDCCreateFETIDPMatContext(PC,FETIDPMat_ctx**);
static PetscErrorCode PCBDDCDestroyFETIDPMat(Mat);
static PetscErrorCode PCBDDCSetupFETIDPMatContext(FETIDPMat_ctx*);
static PetscErrorCode PCBDDCCreateFETIDPPCContext(PC,FETIDPPC_ctx**);
static PetscErrorCode PCBDDCDestroyFETIDPPC(PC);
static PetscErrorCode PCBDDCSetupFETIDPPCContext(Mat,FETIDPPC_ctx*);
static PetscErrorCode FETIDPPCApply(PC,Vec,Vec);
static PetscErrorCode FETIDPMatMult(Mat,Vec,Vec);

#endif /* __pcbddc_h */
