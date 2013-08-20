
/*
     Routines that call the kernel minpack coloring subroutines
*/

#include <petsc-private/matimpl.h>
#include <../src/mat/color/color.h>

/*
    MatFDColoringDegreeSequence_Minpack - Calls the MINPACK routine seqr() that
      computes the degree sequence required by MINPACK coloring routines.
*/
#undef __FUNCT__
#define __FUNCT__ "MatFDColoringDegreeSequence_Minpack"
PetscErrorCode MatFDColoringDegreeSequence_Minpack(PetscInt m,const PetscInt *cja,const PetscInt *cia,const PetscInt *rja,const PetscInt *ria,PetscInt **seq)
{
  PetscInt       *work;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscMalloc(m*sizeof(PetscInt),&work);CHKERRQ(ierr);
  ierr = PetscMalloc(m*sizeof(PetscInt),seq);CHKERRQ(ierr);

  MINPACKdegr(&m,cja,cia,rja,ria,*seq,work);

  ierr = PetscFree(work);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

/*
    MatFDColoringMinimumNumberofColors_Private - For a given sparse
        matrix computes the minimum number of colors needed.

*/
#undef __FUNCT__
#define __FUNCT__ "MatFDColoringMinimumNumberofColors_Private"
PetscErrorCode MatFDColoringMinimumNumberofColors_Private(PetscInt m,PetscInt *ia,PetscInt *minc)
{
  PetscInt i,c = 0;

  PetscFunctionBegin;
  for (i=0; i<m; i++) c = PetscMax(c,ia[i+1]-ia[i]);
  *minc = c;
  PetscFunctionReturn(0);
}

/* ----------------------------------------------------------------------------*/
/*
    MatGetColoring_SL_Minpack - Uses the smallest-last (SL) coloring of minpack
*/
#undef __FUNCT__
#define __FUNCT__ "MatGetColoring_SL_Minpack"
PETSC_EXTERN PetscErrorCode MatGetColoring_SL_Minpack(Mat mat,MatColoringType name,ISColoring *iscoloring)
{
  PetscErrorCode  ierr;
  PetscInt        *list,*work,clique,*seq,*coloring,n;
  const PetscInt  *ria,*rja,*cia,*cja;
  PetscInt        ncolors,i;
  PetscBool       done;
  Mat             mat_seq = mat;
  PetscMPIInt     size;
  MPI_Comm        comm;
  ISColoring      iscoloring_seq;
  PetscInt        bs = 1,rstart,rend,N_loc,nc;
  ISColoringValue *colors_loc;
  PetscBool       flg1,flg2;

  PetscFunctionBegin;
  /* this is ugly way to get blocksize but cannot call MatGetBlockSize() because AIJ can have bs > 1 */
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATSEQBAIJ,&flg1);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATMPIBAIJ,&flg2);CHKERRQ(ierr);
  if (flg1 || flg2) {
    ierr = MatGetBlockSize(mat,&bs);CHKERRQ(ierr);
  }

  ierr = PetscObjectGetComm((PetscObject)mat,&comm);CHKERRQ(ierr);
  ierr = MPI_Comm_size(comm,&size);CHKERRQ(ierr);
  if (size > 1) {
    /* create a sequential iscoloring on all processors */
    ierr = MatGetSeqNonzeroStructure(mat,&mat_seq);CHKERRQ(ierr);
  }

  ierr = MatGetRowIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,&n,&ria,&rja,&done);CHKERRQ(ierr);
  ierr = MatGetColumnIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,&n,&cia,&cja,&done);CHKERRQ(ierr);
  if (!done) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Ordering requires IJ");

  ierr = MatFDColoringDegreeSequence_Minpack(n,cja,cia,rja,ria,&seq);CHKERRQ(ierr);

  ierr = PetscMalloc2(n,PetscInt,&list,4*n,PetscInt,&work);CHKERRQ(ierr);

  MINPACKslo(&n,cja,cia,rja,ria,seq,list,&clique,work,work+n,work+2*n,work+3*n);

  ierr = PetscMalloc(n*sizeof(PetscInt),&coloring);CHKERRQ(ierr);
  MINPACKseq(&n,cja,cia,rja,ria,list,coloring,&ncolors,work);

  ierr = PetscFree2(list,work);CHKERRQ(ierr);
  ierr = PetscFree(seq);CHKERRQ(ierr);
  ierr = MatRestoreRowIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,NULL,&ria,&rja,&done);CHKERRQ(ierr);
  ierr = MatRestoreColumnIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,NULL,&cia,&cja,&done);CHKERRQ(ierr);

  /* shift coloring numbers to start at zero and shorten */
  if (ncolors > IS_COLORING_MAX-1) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Maximum color size exceeded");
  {
    ISColoringValue *s = (ISColoringValue*) coloring;
    for (i=0; i<n; i++) {
      s[i] = (ISColoringValue) (coloring[i]-1);
    }
    ierr = MatColoringPatch(mat_seq,ncolors,n,s,iscoloring);CHKERRQ(ierr);
  }

  if (size > 1) {
    ierr = MatDestroySeqNonzeroStructure(&mat_seq);CHKERRQ(ierr);

    /* convert iscoloring_seq to a parallel iscoloring */
    iscoloring_seq = *iscoloring;
    rstart         = mat->rmap->rstart/bs;
    rend           = mat->rmap->rend/bs;
    N_loc          = rend - rstart; /* number of local nodes */

    /* get local colors for each local node */
    ierr = PetscMalloc((N_loc+1)*sizeof(ISColoringValue),&colors_loc);CHKERRQ(ierr);
    for (i=rstart; i<rend; i++) {
      colors_loc[i-rstart] = iscoloring_seq->colors[i];
    }
    /* create a parallel iscoloring */
    nc   = iscoloring_seq->n;
    ierr = ISColoringCreate(comm,nc,N_loc,colors_loc,iscoloring);CHKERRQ(ierr);
    ierr = ISColoringDestroy(&iscoloring_seq);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

/* ----------------------------------------------------------------------------*/
/*
    MatGetColoring_LF_Minpack -
*/
#undef __FUNCT__
#define __FUNCT__ "MatGetColoring_LF_Minpack"
PETSC_EXTERN PetscErrorCode MatGetColoring_LF_Minpack(Mat mat,MatColoringType name,ISColoring *iscoloring)
{
  PetscErrorCode  ierr;
  PetscInt        *list,*work,*seq,*coloring,n;
  const PetscInt  *ria,*rja,*cia,*cja;
  PetscInt        n1, none,ncolors,i;
  PetscBool       done;
  Mat             mat_seq = mat;
  PetscMPIInt     size;
  MPI_Comm        comm;
  ISColoring      iscoloring_seq;
  PetscInt        bs = 1,rstart,rend,N_loc,nc;
  ISColoringValue *colors_loc;
  PetscBool       flg1,flg2;

  PetscFunctionBegin;
  /* this is ugly way to get blocksize but cannot call MatGetBlockSize() because AIJ can have bs > 1 */
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATSEQBAIJ,&flg1);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATMPIBAIJ,&flg2);CHKERRQ(ierr);
  if (flg1 || flg2) {
    ierr = MatGetBlockSize(mat,&bs);CHKERRQ(ierr);
  }

  ierr = PetscObjectGetComm((PetscObject)mat,&comm);CHKERRQ(ierr);
  ierr = MPI_Comm_size(comm,&size);CHKERRQ(ierr);
  if (size > 1) {
    /* create a sequential iscoloring on all processors */
    ierr = MatGetSeqNonzeroStructure(mat,&mat_seq);CHKERRQ(ierr);
  }

  ierr = MatGetRowIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,&n,&ria,&rja,&done);CHKERRQ(ierr);
  ierr = MatGetColumnIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,&n,&cia,&cja,&done);CHKERRQ(ierr);
  if (!done) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Ordering requires IJ");

  ierr = MatFDColoringDegreeSequence_Minpack(n,cja,cia,rja,ria,&seq);CHKERRQ(ierr);

  ierr = PetscMalloc2(n,PetscInt,&list,4*n,PetscInt,&work);CHKERRQ(ierr);

  n1   = n - 1;
  none = -1;
  MINPACKnumsrt(&n,&n1,seq,&none,list,work+2*n,work+n);
  ierr = PetscMalloc(n*sizeof(PetscInt),&coloring);CHKERRQ(ierr);
  MINPACKseq(&n,cja,cia,rja,ria,list,coloring,&ncolors,work);

  ierr = PetscFree2(list,work);CHKERRQ(ierr);
  ierr = PetscFree(seq);CHKERRQ(ierr);

  ierr = MatRestoreRowIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,NULL,&ria,&rja,&done);CHKERRQ(ierr);
  ierr = MatRestoreColumnIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,NULL,&cia,&cja,&done);CHKERRQ(ierr);

  /* shift coloring numbers to start at zero and shorten */
  if (ncolors > IS_COLORING_MAX-1) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Maximum color size exceeded");
  {
    ISColoringValue *s = (ISColoringValue*) coloring;
    for (i=0; i<n; i++) s[i] = (ISColoringValue) (coloring[i]-1);
    ierr = MatColoringPatch(mat_seq,ncolors,n,s,iscoloring);CHKERRQ(ierr);
  }

  if (size > 1) {
    ierr = MatDestroySeqNonzeroStructure(&mat_seq);CHKERRQ(ierr);

    /* convert iscoloring_seq to a parallel iscoloring */
    iscoloring_seq = *iscoloring;
    rstart         = mat->rmap->rstart/bs;
    rend           = mat->rmap->rend/bs;
    N_loc          = rend - rstart; /* number of local nodes */

    /* get local colors for each local node */
    ierr = PetscMalloc((N_loc+1)*sizeof(ISColoringValue),&colors_loc);CHKERRQ(ierr);
    for (i=rstart; i<rend; i++) colors_loc[i-rstart] = iscoloring_seq->colors[i];

    /* create a parallel iscoloring */
    nc   = iscoloring_seq->n;
    ierr = ISColoringCreate(comm,nc,N_loc,colors_loc,iscoloring);CHKERRQ(ierr);
    ierr = ISColoringDestroy(&iscoloring_seq);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

/* ----------------------------------------------------------------------------*/
/*
    MatGetColoring_ID_Minpack -
*/
#undef __FUNCT__
#define __FUNCT__ "MatGetColoring_ID_Minpack"
PETSC_EXTERN PetscErrorCode MatGetColoring_ID_Minpack(Mat mat,MatColoringType name,ISColoring *iscoloring)
{
  PetscErrorCode  ierr;
  PetscInt        *list,*work,clique,*seq,*coloring,n;
  const PetscInt  *ria,*rja,*cia,*cja;
  PetscInt        ncolors,i;
  PetscBool       done;
  Mat             mat_seq = mat;
  PetscMPIInt     size;
  MPI_Comm        comm;
  ISColoring      iscoloring_seq;
  PetscInt        bs = 1,rstart,rend,N_loc,nc;
  ISColoringValue *colors_loc;
  PetscBool       flg1,flg2;

  PetscFunctionBegin;
  /* this is ugly way to get blocksize but cannot call MatGetBlockSize() because AIJ can have bs > 1 */
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATSEQBAIJ,&flg1);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATMPIBAIJ,&flg2);CHKERRQ(ierr);
  if (flg1 || flg2) {
    ierr = MatGetBlockSize(mat,&bs);CHKERRQ(ierr);
  }

  ierr = PetscObjectGetComm((PetscObject)mat,&comm);CHKERRQ(ierr);
  ierr = MPI_Comm_size(comm,&size);CHKERRQ(ierr);
  if (size > 1) {
    /* create a sequential iscoloring on all processors */
    ierr = MatGetSeqNonzeroStructure(mat,&mat_seq);CHKERRQ(ierr);
  }

  ierr = MatGetRowIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,&n,&ria,&rja,&done);CHKERRQ(ierr);
  ierr = MatGetColumnIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,&n,&cia,&cja,&done);CHKERRQ(ierr);
  if (!done) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Ordering requires IJ");

  ierr = MatFDColoringDegreeSequence_Minpack(n,cja,cia,rja,ria,&seq);CHKERRQ(ierr);

  ierr = PetscMalloc2(n,PetscInt,&list,4*n,PetscInt,&work);CHKERRQ(ierr);

  MINPACKido(&n,&n,cja,cia,rja,ria,seq,list,&clique,work,work+n,work+2*n,work+3*n);

  ierr = PetscMalloc(n*sizeof(PetscInt),&coloring);CHKERRQ(ierr);
  MINPACKseq(&n,cja,cia,rja,ria,list,coloring,&ncolors,work);

  ierr = PetscFree2(list,work);CHKERRQ(ierr);
  ierr = PetscFree(seq);CHKERRQ(ierr);

  ierr = MatRestoreRowIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,NULL,&ria,&rja,&done);CHKERRQ(ierr);
  ierr = MatRestoreColumnIJ(mat_seq,1,PETSC_FALSE,PETSC_TRUE,NULL,&cia,&cja,&done);CHKERRQ(ierr);

  /* shift coloring numbers to start at zero and shorten */
  if (ncolors > IS_COLORING_MAX-1) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Maximum color size exceeded");
  {
    ISColoringValue *s = (ISColoringValue*) coloring;
    for (i=0; i<n; i++) {
      s[i] = (ISColoringValue) (coloring[i]-1);
    }
    ierr = MatColoringPatch(mat_seq,ncolors,n,s,iscoloring);CHKERRQ(ierr);
  }

  if (size > 1) {
    ierr = MatDestroySeqNonzeroStructure(&mat_seq);CHKERRQ(ierr);

    /* convert iscoloring_seq to a parallel iscoloring */
    iscoloring_seq = *iscoloring;
    rstart         = mat->rmap->rstart/bs;
    rend           = mat->rmap->rend/bs;
    N_loc          = rend - rstart; /* number of local nodes */

    /* get local colors for each local node */
    ierr = PetscMalloc((N_loc+1)*sizeof(ISColoringValue),&colors_loc);CHKERRQ(ierr);
    for (i=rstart; i<rend; i++) {
      colors_loc[i-rstart] = iscoloring_seq->colors[i];
    }
    /* create a parallel iscoloring */
    nc   = iscoloring_seq->n;
    ierr = ISColoringCreate(comm,nc,N_loc,colors_loc,iscoloring);CHKERRQ(ierr);
    ierr = ISColoringDestroy(&iscoloring_seq);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

/*
   Simplest coloring, each column of the matrix gets its own unique color.
*/
#undef __FUNCT__
#define __FUNCT__ "MatGetColoring_Natural"
PETSC_EXTERN PetscErrorCode MatGetColoring_Natural(Mat mat,MatColoringType color, ISColoring *iscoloring)
{
  PetscErrorCode  ierr;
  PetscInt        start,end,i,bs = 1,n;
  ISColoringValue *colors;
  MPI_Comm        comm;
  PetscBool       flg1,flg2;
  Mat             mat_seq = mat;
  PetscMPIInt     size;
  ISColoring      iscoloring_seq;
  ISColoringValue *colors_loc;
  PetscInt        rstart,rend,N_loc,nc;

  PetscFunctionBegin;
  /* this is ugly way to get blocksize but cannot call MatGetBlockSize() because AIJ can have bs > 1 */
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATSEQBAIJ,&flg1);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATMPIBAIJ,&flg2);CHKERRQ(ierr);
  if (flg1 || flg2) {
    ierr = MatGetBlockSize(mat,&bs);CHKERRQ(ierr);
  }

  ierr = PetscObjectGetComm((PetscObject)mat,&comm);CHKERRQ(ierr);
  ierr = MPI_Comm_size(comm,&size);CHKERRQ(ierr);
  if (size > 1) {
    /* create a sequential iscoloring on all processors */
    ierr = MatGetSeqNonzeroStructure(mat,&mat_seq);CHKERRQ(ierr);
  }

  ierr = MatGetSize(mat_seq,NULL,&n);CHKERRQ(ierr);
  ierr = MatGetOwnershipRange(mat_seq,&start,&end);CHKERRQ(ierr);
  n    = n/bs;
  if (n > IS_COLORING_MAX-1) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_SUP,"Maximum color size exceeded");

  start = start/bs;
  end   = end/bs;
  ierr  = PetscMalloc((end-start+1)*sizeof(PetscInt),&colors);CHKERRQ(ierr);
  for (i=start; i<end; i++) {
    colors[i-start] = (ISColoringValue)i;
  }
  ierr = ISColoringCreate(comm,n,end-start,colors,iscoloring);CHKERRQ(ierr);

  if (size > 1) {
    ierr = MatDestroySeqNonzeroStructure(&mat_seq);CHKERRQ(ierr);

    /* convert iscoloring_seq to a parallel iscoloring */
    iscoloring_seq = *iscoloring;
    rstart         = mat->rmap->rstart/bs;
    rend           = mat->rmap->rend/bs;
    N_loc          = rend - rstart; /* number of local nodes */

    /* get local colors for each local node */
    ierr = PetscMalloc((N_loc+1)*sizeof(ISColoringValue),&colors_loc);CHKERRQ(ierr);
    for (i=rstart; i<rend; i++) {
      colors_loc[i-rstart] = iscoloring_seq->colors[i];
    }
    /* create a parallel iscoloring */
    nc   = iscoloring_seq->n;
    ierr = ISColoringCreate(comm,nc,N_loc,colors_loc,iscoloring);CHKERRQ(ierr);
    ierr = ISColoringDestroy(&iscoloring_seq);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

/* ===========================================================================================*/

PetscFunctionList MatColoringList              = 0;
PetscBool         MatColoringRegisterAllCalled = PETSC_FALSE;

#undef __FUNCT__
#define __FUNCT__ "MatColoringRegister"
/*@C
   MatColoringRegister - Adds a new sparse matrix coloring to the  matrix package.

   Not Collective

   Input Parameters:
+  sname - name of Coloring (for example MATCOLORINGSL)
-  function - function pointer that creates the coloring

   Level: developer

   Sample usage:
.vb
   MatColoringRegister("my_color",MyColor);
.ve

   Then, your partitioner can be chosen with the procedural interface via
$     MatColoringSetType(part,"my_color")
   or at runtime via the option
$     -mat_coloring_type my_color

.keywords: matrix, Coloring, register

.seealso: MatColoringRegisterDestroy(), MatColoringRegisterAll()
@*/
PetscErrorCode  MatColoringRegister(const char sname[],PetscErrorCode (*function)(Mat,MatColoringType,ISColoring*))
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFunctionListAdd(&MatColoringList,sname,function);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatGetColoring"
/*@C
   MatGetColoring - Gets a coloring for a matrix, from its sparsity structure,
      to reduce the number of function evaluations needed to compute a sparse Jacobian via differencing.

   Collective on Mat

   Input Parameters:
.  mat - the matrix
.  type - type of coloring, one of the following:
$      MATCOLORINGNATURAL - natural (one color for each column, very slow)
$      MATCOLORINGSL - smallest-last
$      MATCOLORINGLF - largest-first
$      MATCOLORINGID - incidence-degree

   Output Parameters:
.   iscoloring - the coloring

   Options Database Keys:
   To specify the coloring through the options database, use one of
   the following
$    -mat_coloring_type natural, -mat_coloring_type sl, -mat_coloring_type lf,
$    -mat_coloring_type id
   To see the coloring use
$    -mat_coloring_view

   Level: intermediate

   Notes:
$    A graph coloring C(A)  is a division of vertices  so that two vertices of the same color do not share any common edges.
$    A suitable coloring for a  smoother  is simply C(A).
$    A suitable coloring for efficient Jacobian computation is a division of the columns so that two columns of the same color do not share any common rows.
$         This corresponds to C(A^{T} A).  This is what MatGetColoring() computes.

   The user can define additional colorings; see MatColoringRegister().

   For parallel matrices currently converts to sequential matrix and uses the sequential coloring
   on that.

   The colorings SL, LF, and ID are obtained via the Minpack software that was
   converted to C using f2c.

   For BAIJ matrices this colors the blocks. The true number of colors would be block size times the number of colors
   returned here.

   References:
$     Thomas F. Coleman and Jorge J. More, Estimation of Sparse {J}acobian Matrices and Graph Coloring Problems,
$         SIAM Journal on Numerical Analysis, 1983, pages 187-209, volume 20
$     Jorge J. Mor\'{e} and Danny C. Sorenson and  Burton S. Garbow and Kenneth E. Hillstrom, The {MINPACK} Project,
$         Sources and Development of Mathematical Software, Wayne R. Cowell editor, 1984, pages 88-111

.keywords: matrix, get, coloring

.seealso:  MatGetColoringTypeFromOptions(), MatColoringRegister(), MatFDColoringCreate(),
           SNESComputeJacobianDefaultColor()
@*/
PetscErrorCode  MatGetColoring(Mat mat,MatColoringType type,ISColoring *iscoloring)
{
  PetscBool      flag;
  PetscErrorCode ierr,(*r)(Mat,MatColoringType,ISColoring*);
  char           tname[PETSC_MAX_PATH_LEN];
  MPI_Comm       comm;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(mat,MAT_CLASSID,1);
  PetscValidPointer(iscoloring,3);
  if (!mat->assembled) SETERRQ(PetscObjectComm((PetscObject)mat),PETSC_ERR_ARG_WRONGSTATE,"Not for unassembled matrix");
  if (mat->factortype) SETERRQ(PetscObjectComm((PetscObject)mat),PETSC_ERR_ARG_WRONGSTATE,"Not for factored matrix");

  /* look for type on command line */
  if (!MatColoringRegisterAllCalled) {ierr = MatColoringRegisterAll();CHKERRQ(ierr);}
  ierr = PetscOptionsGetString(((PetscObject)mat)->prefix,"-mat_coloring_type",tname,256,&flag);CHKERRQ(ierr);
  if (flag) type = tname;

  ierr = PetscObjectGetComm((PetscObject)mat,&comm);CHKERRQ(ierr);
  ierr = PetscFunctionListFind(MatColoringList,type,&r);CHKERRQ(ierr);
  if (!r) SETERRQ1(PetscObjectComm((PetscObject)mat),PETSC_ERR_ARG_OUTOFRANGE,"Unknown or unregistered type: %s",type);

  ierr = PetscLogEventBegin(MAT_GetColoring,mat,0,0,0);CHKERRQ(ierr);
  ierr = (*r)(mat,type,iscoloring);CHKERRQ(ierr);
  ierr = PetscLogEventEnd(MAT_GetColoring,mat,0,0,0);CHKERRQ(ierr);

  ierr = PetscInfo1(mat,"Number of colors %d\n",(*iscoloring)->n);CHKERRQ(ierr);
  flag = PETSC_FALSE;
  ierr = PetscOptionsGetBool(NULL,"-mat_coloring_view",&flag,NULL);CHKERRQ(ierr);
  if (flag) {
    PetscViewer viewer;
    ierr = PetscViewerASCIIGetStdout((*iscoloring)->comm,&viewer);CHKERRQ(ierr);
    ierr = ISColoringView(*iscoloring,viewer);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

