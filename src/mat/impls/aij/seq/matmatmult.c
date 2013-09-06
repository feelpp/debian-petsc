
/*
  Defines matrix-matrix product routines for pairs of SeqAIJ matrices
          C = A * B
*/

#include <../src/mat/impls/aij/seq/aij.h> /*I "petscmat.h" I*/
#include <../src/mat/utils/freespace.h>
#include <../src/mat/utils/petscheap.h>
#include <petscbt.h>
#include <../src/mat/impls/dense/seq/dense.h>

static PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ_LLCondensed(Mat,Mat,PetscReal,Mat*);

#undef __FUNCT__
#define __FUNCT__ "MatMatMult_SeqAIJ_SeqAIJ"
PetscErrorCode MatMatMult_SeqAIJ_SeqAIJ(Mat A,Mat B,MatReuse scall,PetscReal fill,Mat *C)
{
  PetscErrorCode ierr;
  PetscBool      scalable=PETSC_FALSE,scalable_fast=PETSC_FALSE,heap = PETSC_FALSE,btheap = PETSC_FALSE,llcondensed = PETSC_FALSE;

  PetscFunctionBegin;
  if (scall == MAT_INITIAL_MATRIX) {
    ierr = PetscObjectOptionsBegin((PetscObject)A);CHKERRQ(ierr);
    ierr = PetscOptionsBool("-matmatmult_scalable","Use a scalable but slower C=A*B","",scalable,&scalable,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsBool("-matmatmult_scalable_fast","Use a scalable but slower C=A*B","",scalable_fast,&scalable_fast,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsBool("-matmatmult_heap","Use heap implementation of symbolic factorization C=A*B","",heap,&heap,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsBool("-matmatmult_btheap","Use btheap implementation of symbolic factorization C=A*B","",btheap,&btheap,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsBool("-matmatmult_llcondensed","Use LLCondensed to for symbolic C=A*B","",llcondensed,&llcondensed,NULL);CHKERRQ(ierr);
    ierr = PetscOptionsEnd();CHKERRQ(ierr);
    ierr = PetscLogEventBegin(MAT_MatMultSymbolic,A,B,0,0);CHKERRQ(ierr);
    if (scalable_fast) {
      ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ_Scalable_fast(A,B,fill,C);CHKERRQ(ierr);
    } else if (scalable) {
      ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ_Scalable(A,B,fill,C);CHKERRQ(ierr);
    } else if (heap) {
      ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ_Heap(A,B,fill,C);CHKERRQ(ierr);
    } else if (btheap) {
      ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ_BTHeap(A,B,fill,C);CHKERRQ(ierr);
    } else if (llcondensed) {
      ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ_LLCondensed(A,B,fill,C);CHKERRQ(ierr);
    } else {
      ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ(A,B,fill,C);CHKERRQ(ierr);
    }
    ierr = PetscLogEventEnd(MAT_MatMultSymbolic,A,B,0,0);CHKERRQ(ierr);
  }

  ierr = PetscLogEventBegin(MAT_MatMultNumeric,A,B,0,0);CHKERRQ(ierr);
  ierr = (*(*C)->ops->matmultnumeric)(A,B,*C);CHKERRQ(ierr);
  ierr = PetscLogEventEnd(MAT_MatMultNumeric,A,B,0,0);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqAIJ_LLCondensed"
static PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ_LLCondensed(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode     ierr;
  Mat_SeqAIJ         *a =(Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  PetscInt           *ai=a->i,*bi=b->i,*ci,*cj;
  PetscInt           am =A->rmap->N,bn=B->cmap->N,bm=B->rmap->N;
  PetscReal          afill;
  PetscInt           i,j,anzi,brow,bnzj,cnzi,*bj,*aj,nlnk_max,*lnk,ndouble=0;
  PetscBT            lnkbt;
  PetscFreeSpaceList free_space=NULL,current_space=NULL;

  PetscFunctionBegin;
  /* Get ci and cj */
  /*---------------*/
  /* Allocate ci array, arrays for fill computation and */
  /* free space for accumulating nonzero column info */
  ierr  = PetscMalloc(((am+1)+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* create and initialize a linked list */
  nlnk_max = a->rmax*b->rmax;
  if (!nlnk_max || nlnk_max > bn) nlnk_max = bn;
  ierr = PetscLLCondensedCreate(nlnk_max,bn,&lnk,&lnkbt);CHKERRQ(ierr);

  /* Initial FreeSpace size is fill*(nnz(A)+nnz(B)) */
  ierr = PetscFreeSpaceGet((PetscInt)(fill*(ai[am]+bi[bm])),&free_space);CHKERRQ(ierr);

  current_space = free_space;

  /* Determine ci and cj */
  for (i=0; i<am; i++) {
    anzi = ai[i+1] - ai[i];
    aj   = a->j + ai[i];
    for (j=0; j<anzi; j++) {
      brow = aj[j];
      bnzj = bi[brow+1] - bi[brow];
      bj   = b->j + bi[brow];
      /* add non-zero cols of B into the sorted linked list lnk */
      ierr = PetscLLCondensedAddSorted(bnzj,bj,lnk,lnkbt);CHKERRQ(ierr);
    }
    cnzi = lnk[0];

    /* If free space is not available, make more free space */
    /* Double the amount of total space in the list */
    if (current_space->local_remaining<cnzi) {
      ierr = PetscFreeSpaceGet(cnzi+current_space->total_array_size,&current_space);CHKERRQ(ierr);
      ndouble++;
    }

    /* Copy data into free space, then initialize lnk */
    ierr = PetscLLCondensedClean(bn,cnzi,current_space->array,lnk,lnkbt);CHKERRQ(ierr);

    current_space->array           += cnzi;
    current_space->local_used      += cnzi;
    current_space->local_remaining -= cnzi;

    ci[i+1] = ci[i] + cnzi;
  }

  /* Column indices are in the list of free space */
  /* Allocate space for cj, initialize cj, and */
  /* destroy list of free space and other temporary array(s) */
  ierr = PetscMalloc((ci[am]+1)*sizeof(PetscInt),&cj);CHKERRQ(ierr);
  ierr = PetscFreeSpaceContiguous(&free_space,cj);CHKERRQ(ierr);
  ierr = PetscLLCondensedDestroy(lnk,lnkbt);CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bn,ci,cj,NULL,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->rmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c                         = (Mat_SeqAIJ*)((*C)->data);
  c->free_a                 = PETSC_FALSE;
  c->free_ij                = PETSC_TRUE;
  c->nonew                  = 0;
  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqAIJ; /* fast, needs non-scalable O(bn) array 'abdense' */

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = ndouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",ndouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultNumeric_SeqAIJ_SeqAIJ"
PetscErrorCode MatMatMultNumeric_SeqAIJ_SeqAIJ(Mat A,Mat B,Mat C)
{
  PetscErrorCode ierr;
  PetscLogDouble flops=0.0;
  Mat_SeqAIJ     *a   = (Mat_SeqAIJ*)A->data;
  Mat_SeqAIJ     *b   = (Mat_SeqAIJ*)B->data;
  Mat_SeqAIJ     *c   = (Mat_SeqAIJ*)C->data;
  PetscInt       *ai  =a->i,*aj=a->j,*bi=b->i,*bj=b->j,*bjj,*ci=c->i,*cj=c->j;
  PetscInt       am   =A->rmap->n,cm=C->rmap->n;
  PetscInt       i,j,k,anzi,bnzi,cnzi,brow;
  PetscScalar    *aa=a->a,*ba=b->a,*baj,*ca,valtmp;
  PetscScalar    *ab_dense;

  PetscFunctionBegin;
  /* printf("MatMatMultNumeric_SeqAIJ_SeqAIJ...ca %p\n",c->a); */
  if (!c->a) { /* first call of MatMatMultNumeric_SeqAIJ_SeqAIJ, allocate ca and matmult_abdense */
    ierr      = PetscMalloc((ci[cm]+1)*sizeof(MatScalar),&ca);CHKERRQ(ierr);
    c->a      = ca;
    c->free_a = PETSC_TRUE;

    ierr = PetscMalloc(B->cmap->N*sizeof(PetscScalar),&ab_dense);CHKERRQ(ierr);
    ierr = PetscMemzero(ab_dense,B->cmap->N*sizeof(PetscScalar));CHKERRQ(ierr);

    c->matmult_abdense = ab_dense;
  } else {
    ca       = c->a;
    ab_dense = c->matmult_abdense;
  }

  /* clean old values in C */
  ierr = PetscMemzero(ca,ci[cm]*sizeof(MatScalar));CHKERRQ(ierr);
  /* Traverse A row-wise. */
  /* Build the ith row in C by summing over nonzero columns in A, */
  /* the rows of B corresponding to nonzeros of A. */
  for (i=0; i<am; i++) {
    anzi = ai[i+1] - ai[i];
    for (j=0; j<anzi; j++) {
      brow = aj[j];
      bnzi = bi[brow+1] - bi[brow];
      bjj  = bj + bi[brow];
      baj  = ba + bi[brow];
      /* perform dense axpy */
      valtmp = aa[j];
      for (k=0; k<bnzi; k++) {
        ab_dense[bjj[k]] += valtmp*baj[k];
      }
      flops += 2*bnzi;
    }
    aj += anzi; aa += anzi;

    cnzi = ci[i+1] - ci[i];
    for (k=0; k<cnzi; k++) {
      ca[k]          += ab_dense[cj[k]];
      ab_dense[cj[k]] = 0.0; /* zero ab_dense */
    }
    flops += cnzi;
    cj    += cnzi; ca += cnzi;
  }
  ierr = MatAssemblyBegin(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = PetscLogFlops(flops);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultNumeric_SeqAIJ_SeqAIJ_Scalable"
PetscErrorCode MatMatMultNumeric_SeqAIJ_SeqAIJ_Scalable(Mat A,Mat B,Mat C)
{
  PetscErrorCode ierr;
  PetscLogDouble flops=0.0;
  Mat_SeqAIJ     *a   = (Mat_SeqAIJ*)A->data;
  Mat_SeqAIJ     *b   = (Mat_SeqAIJ*)B->data;
  Mat_SeqAIJ     *c   = (Mat_SeqAIJ*)C->data;
  PetscInt       *ai  = a->i,*aj=a->j,*bi=b->i,*bj=b->j,*bjj,*ci=c->i,*cj=c->j;
  PetscInt       am   = A->rmap->N,cm=C->rmap->N;
  PetscInt       i,j,k,anzi,bnzi,cnzi,brow;
  PetscScalar    *aa=a->a,*ba=b->a,*baj,*ca=c->a,valtmp;
  PetscInt       nextb;

  PetscFunctionBegin;
  /* clean old values in C */
  ierr = PetscMemzero(ca,ci[cm]*sizeof(MatScalar));CHKERRQ(ierr);
  /* Traverse A row-wise. */
  /* Build the ith row in C by summing over nonzero columns in A, */
  /* the rows of B corresponding to nonzeros of A. */
  for (i=0; i<am; i++) {
    anzi = ai[i+1] - ai[i];
    cnzi = ci[i+1] - ci[i];
    for (j=0; j<anzi; j++) {
      brow = aj[j];
      bnzi = bi[brow+1] - bi[brow];
      bjj  = bj + bi[brow];
      baj  = ba + bi[brow];
      /* perform sparse axpy */
      valtmp = aa[j];
      nextb  = 0;
      for (k=0; nextb<bnzi; k++) {
        if (cj[k] == bjj[nextb]) { /* ccol == bcol */
          ca[k] += valtmp*baj[nextb++];
        }
      }
      flops += 2*bnzi;
    }
    aj += anzi; aa += anzi;
    cj += cnzi; ca += cnzi;
  }

  ierr = MatAssemblyBegin(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = PetscLogFlops(flops);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqAIJ_Scalable_fast"
PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ_Scalable_fast(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode     ierr;
  Mat_SeqAIJ         *a  = (Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  PetscInt           *ai = a->i,*bi=b->i,*ci,*cj;
  PetscInt           am  = A->rmap->N,bn=B->cmap->N,bm=B->rmap->N;
  MatScalar          *ca;
  PetscReal          afill;
  PetscInt           i,j,anzi,brow,bnzj,cnzi,*bj,*aj,nlnk_max,*lnk,ndouble=0;
  PetscFreeSpaceList free_space=NULL,current_space=NULL;

  PetscFunctionBegin;
  /* Get ci and cj - same as MatMatMultSymbolic_SeqAIJ_SeqAIJ except using PetscLLxxx_fast() */
  /*-----------------------------------------------------------------------------------------*/
  /* Allocate arrays for fill computation and free space for accumulating nonzero column */
  ierr  = PetscMalloc(((am+1)+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* create and initialize a linked list */
  nlnk_max = a->rmax*b->rmax;
  if (!nlnk_max || nlnk_max > bn) nlnk_max = bn; /* in case rmax is not defined for A or B */
  ierr = PetscLLCondensedCreate_fast(nlnk_max,&lnk);CHKERRQ(ierr);

  /* Initial FreeSpace size is fill*(nnz(A)+nnz(B)) */
  ierr          = PetscFreeSpaceGet((PetscInt)(fill*(ai[am]+bi[bm])),&free_space);CHKERRQ(ierr);
  current_space = free_space;

  /* Determine ci and cj */
  for (i=0; i<am; i++) {
    anzi = ai[i+1] - ai[i];
    aj   = a->j + ai[i];
    for (j=0; j<anzi; j++) {
      brow = aj[j];
      bnzj = bi[brow+1] - bi[brow];
      bj   = b->j + bi[brow];
      /* add non-zero cols of B into the sorted linked list lnk */
      ierr = PetscLLCondensedAddSorted_fast(bnzj,bj,lnk);CHKERRQ(ierr);
    }
    cnzi = lnk[1];

    /* If free space is not available, make more free space */
    /* Double the amount of total space in the list */
    if (current_space->local_remaining<cnzi) {
      ierr = PetscFreeSpaceGet(cnzi+current_space->total_array_size,&current_space);CHKERRQ(ierr);
      ndouble++;
    }

    /* Copy data into free space, then initialize lnk */
    ierr = PetscLLCondensedClean_fast(cnzi,current_space->array,lnk);CHKERRQ(ierr);

    current_space->array           += cnzi;
    current_space->local_used      += cnzi;
    current_space->local_remaining -= cnzi;

    ci[i+1] = ci[i] + cnzi;
  }

  /* Column indices are in the list of free space */
  /* Allocate space for cj, initialize cj, and */
  /* destroy list of free space and other temporary array(s) */
  ierr = PetscMalloc((ci[am]+1)*sizeof(PetscInt),&cj);CHKERRQ(ierr);
  ierr = PetscFreeSpaceContiguous(&free_space,cj);CHKERRQ(ierr);
  ierr = PetscLLCondensedDestroy_fast(lnk);CHKERRQ(ierr);

  /* Allocate space for ca */
  ierr = PetscMalloc((ci[am]+1)*sizeof(MatScalar),&ca);CHKERRQ(ierr);
  ierr = PetscMemzero(ca,(ci[am]+1)*sizeof(MatScalar));CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bn,ci,cj,ca,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->rmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c          = (Mat_SeqAIJ*)((*C)->data);
  c->free_a  = PETSC_TRUE;
  c->free_ij = PETSC_TRUE;
  c->nonew   = 0;

  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqAIJ_Scalable; /* slower, less memory */

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = ndouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",ndouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqAIJ_Scalable"
PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ_Scalable(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode     ierr;
  Mat_SeqAIJ         *a  = (Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  PetscInt           *ai = a->i,*bi=b->i,*ci,*cj;
  PetscInt           am  = A->rmap->N,bn=B->cmap->N,bm=B->rmap->N;
  MatScalar          *ca;
  PetscReal          afill;
  PetscInt           i,j,anzi,brow,bnzj,cnzi,*bj,*aj,nlnk_max,*lnk,ndouble=0;
  PetscFreeSpaceList free_space=NULL,current_space=NULL;

  PetscFunctionBegin;
  /* Get ci and cj - same as MatMatMultSymbolic_SeqAIJ_SeqAIJ except using PetscLLxxx_Scalalbe() */
  /*---------------------------------------------------------------------------------------------*/
  /* Allocate arrays for fill computation and free space for accumulating nonzero column */
  ierr  = PetscMalloc(((am+1)+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* create and initialize a linked list */
  nlnk_max = a->rmax*b->rmax;
  if (!nlnk_max || nlnk_max > bn) nlnk_max = bn; /* in case rmax is not defined for A or B */
  ierr = PetscLLCondensedCreate_Scalable(nlnk_max,&lnk);CHKERRQ(ierr);

  /* Initial FreeSpace size is fill*(nnz(A)+nnz(B)) */
  ierr          = PetscFreeSpaceGet((PetscInt)(fill*(ai[am]+bi[bm])),&free_space);CHKERRQ(ierr);
  current_space = free_space;

  /* Determine ci and cj */
  for (i=0; i<am; i++) {
    anzi = ai[i+1] - ai[i];
    aj   = a->j + ai[i];
    for (j=0; j<anzi; j++) {
      brow = aj[j];
      bnzj = bi[brow+1] - bi[brow];
      bj   = b->j + bi[brow];
      /* add non-zero cols of B into the sorted linked list lnk */
      ierr = PetscLLCondensedAddSorted_Scalable(bnzj,bj,lnk);CHKERRQ(ierr);
    }
    cnzi = lnk[0];

    /* If free space is not available, make more free space */
    /* Double the amount of total space in the list */
    if (current_space->local_remaining<cnzi) {
      ierr = PetscFreeSpaceGet(cnzi+current_space->total_array_size,&current_space);CHKERRQ(ierr);
      ndouble++;
    }

    /* Copy data into free space, then initialize lnk */
    ierr = PetscLLCondensedClean_Scalable(cnzi,current_space->array,lnk);CHKERRQ(ierr);

    current_space->array           += cnzi;
    current_space->local_used      += cnzi;
    current_space->local_remaining -= cnzi;

    ci[i+1] = ci[i] + cnzi;
  }

  /* Column indices are in the list of free space */
  /* Allocate space for cj, initialize cj, and */
  /* destroy list of free space and other temporary array(s) */
  ierr = PetscMalloc((ci[am]+1)*sizeof(PetscInt),&cj);CHKERRQ(ierr);
  ierr = PetscFreeSpaceContiguous(&free_space,cj);CHKERRQ(ierr);
  ierr = PetscLLCondensedDestroy_Scalable(lnk);CHKERRQ(ierr);

  /* Allocate space for ca */
  /*-----------------------*/
  ierr = PetscMalloc((ci[am]+1)*sizeof(MatScalar),&ca);CHKERRQ(ierr);
  ierr = PetscMemzero(ca,(ci[am]+1)*sizeof(MatScalar));CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bn,ci,cj,ca,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->rmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c          = (Mat_SeqAIJ*)((*C)->data);
  c->free_a  = PETSC_TRUE;
  c->free_ij = PETSC_TRUE;
  c->nonew   = 0;

  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqAIJ_Scalable; /* slower, less memory */

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = ndouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",ndouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqAIJ_Heap"
PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ_Heap(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode     ierr;
  Mat_SeqAIJ         *a = (Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  const PetscInt     *ai=a->i,*bi=b->i,*aj=a->j,*bj=b->j;
  PetscInt           *ci,*cj,*bb;
  PetscInt           am=A->rmap->N,bn=B->cmap->N,bm=B->rmap->N;
  PetscReal          afill;
  PetscInt           i,j,col,ndouble = 0;
  PetscFreeSpaceList free_space=NULL,current_space=NULL;
  PetscHeap          h;

  PetscFunctionBegin;
  /* Get ci and cj - by merging sorted rows using a heap */
  /*---------------------------------------------------------------------------------------------*/
  /* Allocate arrays for fill computation and free space for accumulating nonzero column */
  ierr  = PetscMalloc(((am+1)+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* Initial FreeSpace size is fill*(nnz(A)+nnz(B)) */
  ierr          = PetscFreeSpaceGet((PetscInt)(fill*(ai[am]+bi[bm])),&free_space);CHKERRQ(ierr);
  current_space = free_space;

  ierr = PetscHeapCreate(a->rmax,&h);CHKERRQ(ierr);
  ierr = PetscMalloc(a->rmax*sizeof(PetscInt),&bb);CHKERRQ(ierr);

  /* Determine ci and cj */
  for (i=0; i<am; i++) {
    const PetscInt anzi  = ai[i+1] - ai[i]; /* number of nonzeros in this row of A, this is the number of rows of B that we merge */
    const PetscInt *acol = aj + ai[i]; /* column indices of nonzero entries in this row */
    ci[i+1] = ci[i];
    /* Populate the min heap */
    for (j=0; j<anzi; j++) {
      bb[j] = bi[acol[j]];         /* bb points at the start of the row */
      if (bb[j] < bi[acol[j]+1]) { /* Add if row is nonempty */
        ierr = PetscHeapAdd(h,j,bj[bb[j]++]);CHKERRQ(ierr);
      }
    }
    /* Pick off the min element, adding it to free space */
    ierr = PetscHeapPop(h,&j,&col);CHKERRQ(ierr);
    while (j >= 0) {
      if (current_space->local_remaining < 1) { /* double the size, but don't exceed 16 MiB */
        ierr = PetscFreeSpaceGet(PetscMin(2*current_space->total_array_size,16 << 20),&current_space);CHKERRQ(ierr);
        ndouble++;
      }
      *(current_space->array++) = col;
      current_space->local_used++;
      current_space->local_remaining--;
      ci[i+1]++;

      /* stash if anything else remains in this row of B */
      if (bb[j] < bi[acol[j]+1]) {ierr = PetscHeapStash(h,j,bj[bb[j]++]);CHKERRQ(ierr);}
      while (1) {               /* pop and stash any other rows of B that also had an entry in this column */
        PetscInt j2,col2;
        ierr = PetscHeapPeek(h,&j2,&col2);CHKERRQ(ierr);
        if (col2 != col) break;
        ierr = PetscHeapPop(h,&j2,&col2);CHKERRQ(ierr);
        if (bb[j2] < bi[acol[j2]+1]) {ierr = PetscHeapStash(h,j2,bj[bb[j2]++]);CHKERRQ(ierr);}
      }
      /* Put any stashed elements back into the min heap */
      ierr = PetscHeapUnstash(h);CHKERRQ(ierr);
      ierr = PetscHeapPop(h,&j,&col);CHKERRQ(ierr);
    }
  }
  ierr = PetscFree(bb);CHKERRQ(ierr);
  ierr = PetscHeapDestroy(&h);CHKERRQ(ierr);

  /* Column indices are in the list of free space */
  /* Allocate space for cj, initialize cj, and */
  /* destroy list of free space and other temporary array(s) */
  ierr = PetscMalloc(ci[am]*sizeof(PetscInt),&cj);CHKERRQ(ierr);
  ierr = PetscFreeSpaceContiguous(&free_space,cj);CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bn,ci,cj,NULL,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->rmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c          = (Mat_SeqAIJ*)((*C)->data);
  c->free_a  = PETSC_TRUE;
  c->free_ij = PETSC_TRUE;
  c->nonew   = 0;

  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqAIJ;

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = ndouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",ndouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqAIJ_BTHeap"
PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ_BTHeap(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode     ierr;
  Mat_SeqAIJ         *a  = (Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  const PetscInt     *ai = a->i,*bi=b->i,*aj=a->j,*bj=b->j;
  PetscInt           *ci,*cj,*bb;
  PetscInt           am=A->rmap->N,bn=B->cmap->N,bm=B->rmap->N;
  PetscReal          afill;
  PetscInt           i,j,col,ndouble = 0;
  PetscFreeSpaceList free_space=NULL,current_space=NULL;
  PetscHeap          h;
  PetscBT            bt;

  PetscFunctionBegin;
  /* Get ci and cj - using a heap for the sorted rows, but use BT so that each index is only added once */
  /*---------------------------------------------------------------------------------------------*/
  /* Allocate arrays for fill computation and free space for accumulating nonzero column */
  ierr  = PetscMalloc(((am+1)+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* Initial FreeSpace size is fill*(nnz(A)+nnz(B)) */
  ierr = PetscFreeSpaceGet((PetscInt)(fill*(ai[am]+bi[bm])),&free_space);CHKERRQ(ierr);

  current_space = free_space;

  ierr = PetscHeapCreate(a->rmax,&h);CHKERRQ(ierr);
  ierr = PetscMalloc(a->rmax*sizeof(PetscInt),&bb);CHKERRQ(ierr);
  ierr = PetscBTCreate(bn,&bt);CHKERRQ(ierr);

  /* Determine ci and cj */
  for (i=0; i<am; i++) {
    const PetscInt anzi  = ai[i+1] - ai[i]; /* number of nonzeros in this row of A, this is the number of rows of B that we merge */
    const PetscInt *acol = aj + ai[i]; /* column indices of nonzero entries in this row */
    const PetscInt *fptr = current_space->array; /* Save beginning of the row so we can clear the BT later */
    ci[i+1] = ci[i];
    /* Populate the min heap */
    for (j=0; j<anzi; j++) {
      PetscInt brow = acol[j];
      for (bb[j] = bi[brow]; bb[j] < bi[brow+1]; bb[j]++) {
        PetscInt bcol = bj[bb[j]];
        if (!PetscBTLookupSet(bt,bcol)) { /* new entry */
          ierr = PetscHeapAdd(h,j,bcol);CHKERRQ(ierr);
          bb[j]++;
          break;
        }
      }
    }
    /* Pick off the min element, adding it to free space */
    ierr = PetscHeapPop(h,&j,&col);CHKERRQ(ierr);
    while (j >= 0) {
      if (current_space->local_remaining < 1) { /* double the size, but don't exceed 16 MiB */
        fptr = NULL;                      /* need PetscBTMemzero */
        ierr = PetscFreeSpaceGet(PetscMin(2*current_space->total_array_size,16 << 20),&current_space);CHKERRQ(ierr);
        ndouble++;
      }
      *(current_space->array++) = col;
      current_space->local_used++;
      current_space->local_remaining--;
      ci[i+1]++;

      /* stash if anything else remains in this row of B */
      for (; bb[j] < bi[acol[j]+1]; bb[j]++) {
        PetscInt bcol = bj[bb[j]];
        if (!PetscBTLookupSet(bt,bcol)) { /* new entry */
          ierr = PetscHeapAdd(h,j,bcol);CHKERRQ(ierr);
          bb[j]++;
          break;
        }
      }
      ierr = PetscHeapPop(h,&j,&col);CHKERRQ(ierr);
    }
    if (fptr) {                 /* Clear the bits for this row */
      for (; fptr<current_space->array; fptr++) {ierr = PetscBTClear(bt,*fptr);CHKERRQ(ierr);}
    } else {                    /* We reallocated so we don't remember (easily) how to clear only the bits we changed */
      ierr = PetscBTMemzero(bn,bt);CHKERRQ(ierr);
    }
  }
  ierr = PetscFree(bb);CHKERRQ(ierr);
  ierr = PetscHeapDestroy(&h);CHKERRQ(ierr);
  ierr = PetscBTDestroy(&bt);CHKERRQ(ierr);

  /* Column indices are in the list of free space */
  /* Allocate space for cj, initialize cj, and */
  /* destroy list of free space and other temporary array(s) */
  ierr = PetscMalloc(ci[am]*sizeof(PetscInt),&cj);CHKERRQ(ierr);
  ierr = PetscFreeSpaceContiguous(&free_space,cj);CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bn,ci,cj,NULL,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->rmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c          = (Mat_SeqAIJ*)((*C)->data);
  c->free_a  = PETSC_TRUE;
  c->free_ij = PETSC_TRUE;
  c->nonew   = 0;

  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqAIJ;

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = ndouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",ndouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqAIJ"
/* concatenate unique entries and then sort */
PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqAIJ(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode     ierr;
  Mat_SeqAIJ         *a  = (Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  const PetscInt     *ai = a->i,*bi=b->i,*aj=a->j,*bj=b->j;
  PetscInt           *ci,*cj;
  PetscInt           am=A->rmap->N,bn=B->cmap->N,bm=B->rmap->N;
  PetscReal          afill;
  PetscInt           i,j,ndouble = 0;
  PetscSegBuffer     seg,segrow;
  char               *seen;

  PetscFunctionBegin;
  ierr  = PetscMalloc((am+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* Initial FreeSpace size is fill*(nnz(A)+nnz(B)) */
  ierr = PetscSegBufferCreate(sizeof(PetscInt),(PetscInt)(fill*(ai[am]+bi[bm])),&seg);CHKERRQ(ierr);
  ierr = PetscSegBufferCreate(sizeof(PetscInt),100,&segrow);CHKERRQ(ierr);
  ierr = PetscMalloc(bn*sizeof(char),&seen);CHKERRQ(ierr);
  ierr = PetscMemzero(seen,bn*sizeof(char));CHKERRQ(ierr);

  /* Determine ci and cj */
  for (i=0; i<am; i++) {
    const PetscInt anzi  = ai[i+1] - ai[i]; /* number of nonzeros in this row of A, this is the number of rows of B that we merge */
    const PetscInt *acol = aj + ai[i]; /* column indices of nonzero entries in this row */
    PetscInt packlen = 0,*PETSC_RESTRICT crow;
    /* Pack segrow */
    for (j=0; j<anzi; j++) {
      PetscInt brow = acol[j],bjstart = bi[brow],bjend = bi[brow+1],k;
      for (k=bjstart; k<bjend; k++) {
        PetscInt bcol = bj[k];
        if (!seen[bcol]) { /* new entry */
          PetscInt *PETSC_RESTRICT slot;
          ierr = PetscSegBufferGetInts(segrow,1,&slot);CHKERRQ(ierr);
          *slot = bcol;
          seen[bcol] = 1;
          packlen++;
        }
      }
    }
    ierr = PetscSegBufferGetInts(seg,packlen,&crow);CHKERRQ(ierr);
    ierr = PetscSegBufferExtractTo(segrow,crow);CHKERRQ(ierr);
    ierr = PetscSortInt(packlen,crow);CHKERRQ(ierr);
    ci[i+1] = ci[i] + packlen;
    for (j=0; j<packlen; j++) seen[crow[j]] = 0;
  }
  ierr = PetscSegBufferDestroy(&segrow);CHKERRQ(ierr);
  ierr = PetscFree(seen);CHKERRQ(ierr);

  /* Column indices are in the segmented buffer */
  ierr = PetscSegBufferExtractAlloc(seg,&cj);CHKERRQ(ierr);
  ierr = PetscSegBufferDestroy(&seg);CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bn,ci,cj,NULL,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->rmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c          = (Mat_SeqAIJ*)((*C)->data);
  c->free_a  = PETSC_TRUE;
  c->free_ij = PETSC_TRUE;
  c->nonew   = 0;

  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqAIJ;

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = ndouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",ndouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
  PetscFunctionReturn(0);
}

/* This routine is not used. Should be removed! */
#undef __FUNCT__
#define __FUNCT__ "MatMatTransposeMult_SeqAIJ_SeqAIJ"
PetscErrorCode MatMatTransposeMult_SeqAIJ_SeqAIJ(Mat A,Mat B,MatReuse scall,PetscReal fill,Mat *C)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (scall == MAT_INITIAL_MATRIX) {
    ierr = PetscLogEventBegin(MAT_MatTransposeMultSymbolic,A,B,0,0);CHKERRQ(ierr);
    ierr = MatMatTransposeMultSymbolic_SeqAIJ_SeqAIJ(A,B,fill,C);CHKERRQ(ierr);
    ierr = PetscLogEventEnd(MAT_MatTransposeMultSymbolic,A,B,0,0);CHKERRQ(ierr);
  }
  ierr = PetscLogEventBegin(MAT_MatTransposeMultNumeric,A,B,0,0);CHKERRQ(ierr);
  ierr = MatMatTransposeMultNumeric_SeqAIJ_SeqAIJ(A,B,*C);CHKERRQ(ierr);
  ierr = PetscLogEventEnd(MAT_MatTransposeMultNumeric,A,B,0,0);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscContainerDestroy_Mat_MatMatTransMult"
PetscErrorCode PetscContainerDestroy_Mat_MatMatTransMult(void *ptr)
{
  PetscErrorCode      ierr;
  Mat_MatMatTransMult *multtrans=(Mat_MatMatTransMult*)ptr;

  PetscFunctionBegin;
  ierr = MatTransposeColoringDestroy(&multtrans->matcoloring);CHKERRQ(ierr);
  ierr = MatDestroy(&multtrans->Bt_den);CHKERRQ(ierr);
  ierr = MatDestroy(&multtrans->ABt_den);CHKERRQ(ierr);
  ierr = PetscFree(multtrans);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatDestroy_SeqAIJ_MatMatMultTrans"
PetscErrorCode MatDestroy_SeqAIJ_MatMatMultTrans(Mat A)
{
  PetscErrorCode      ierr;
  PetscContainer      container;
  Mat_MatMatTransMult *multtrans=NULL;

  PetscFunctionBegin;
  ierr = PetscObjectQuery((PetscObject)A,"Mat_MatMatTransMult",(PetscObject*)&container);CHKERRQ(ierr);
  if (!container) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_PLIB,"Container does not exit");
  ierr = PetscContainerGetPointer(container,(void**)&multtrans);CHKERRQ(ierr);

  A->ops->destroy = multtrans->destroy;
  if (A->ops->destroy) {
    ierr = (*A->ops->destroy)(A);CHKERRQ(ierr);
  }
  ierr = PetscObjectCompose((PetscObject)A,"Mat_MatMatTransMult",0);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatTransposeMultSymbolic_SeqAIJ_SeqAIJ"
PetscErrorCode MatMatTransposeMultSymbolic_SeqAIJ_SeqAIJ(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode      ierr;
  Mat                 Bt;
  PetscInt            *bti,*btj;
  Mat_MatMatTransMult *multtrans;
  PetscContainer      container;

  PetscFunctionBegin;
  /* create symbolic Bt */
  ierr = MatGetSymbolicTranspose_SeqAIJ(B,&bti,&btj);CHKERRQ(ierr);
  ierr = MatCreateSeqAIJWithArrays(PETSC_COMM_SELF,B->cmap->n,B->rmap->n,bti,btj,NULL,&Bt);CHKERRQ(ierr);

  Bt->rmap->bs = A->cmap->bs;
  Bt->cmap->bs = B->cmap->bs;

  /* get symbolic C=A*Bt */
  ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ(A,Bt,fill,C);CHKERRQ(ierr);

  /* create a supporting struct for reuse intermidiate dense matrices with matcoloring */
  ierr = PetscNew(Mat_MatMatTransMult,&multtrans);CHKERRQ(ierr);

  /* attach the supporting struct to C */
  ierr = PetscContainerCreate(PETSC_COMM_SELF,&container);CHKERRQ(ierr);
  ierr = PetscContainerSetPointer(container,multtrans);CHKERRQ(ierr);
  ierr = PetscContainerSetUserDestroy(container,PetscContainerDestroy_Mat_MatMatTransMult);CHKERRQ(ierr);
  ierr = PetscObjectCompose((PetscObject)(*C),"Mat_MatMatTransMult",(PetscObject)container);CHKERRQ(ierr);
  ierr = PetscContainerDestroy(&container);CHKERRQ(ierr);

  multtrans->usecoloring = PETSC_FALSE;
  multtrans->destroy     = (*C)->ops->destroy;
  (*C)->ops->destroy     = MatDestroy_SeqAIJ_MatMatMultTrans;

  ierr = PetscOptionsGetBool(NULL,"-matmattransmult_color",&multtrans->usecoloring,NULL);CHKERRQ(ierr);
  if (multtrans->usecoloring) {
    /* Create MatTransposeColoring from symbolic C=A*B^T */
    MatTransposeColoring matcoloring;
    ISColoring           iscoloring;
    Mat                  Bt_dense,C_dense;

    ierr = MatGetColoring(*C,MATCOLORINGLF,&iscoloring);CHKERRQ(ierr);
    ierr = MatTransposeColoringCreate(*C,iscoloring,&matcoloring);CHKERRQ(ierr);

    multtrans->matcoloring = matcoloring;

    ierr = ISColoringDestroy(&iscoloring);CHKERRQ(ierr);

    /* Create Bt_dense and C_dense = A*Bt_dense */
    ierr = MatCreate(PETSC_COMM_SELF,&Bt_dense);CHKERRQ(ierr);
    ierr = MatSetSizes(Bt_dense,A->cmap->n,matcoloring->ncolors,A->cmap->n,matcoloring->ncolors);CHKERRQ(ierr);
    ierr = MatSetType(Bt_dense,MATSEQDENSE);CHKERRQ(ierr);
    ierr = MatSeqDenseSetPreallocation(Bt_dense,NULL);CHKERRQ(ierr);

    Bt_dense->assembled = PETSC_TRUE;
    multtrans->Bt_den   = Bt_dense;

    ierr = MatCreate(PETSC_COMM_SELF,&C_dense);CHKERRQ(ierr);
    ierr = MatSetSizes(C_dense,A->rmap->n,matcoloring->ncolors,A->rmap->n,matcoloring->ncolors);CHKERRQ(ierr);
    ierr = MatSetType(C_dense,MATSEQDENSE);CHKERRQ(ierr);
    ierr = MatSeqDenseSetPreallocation(C_dense,NULL);CHKERRQ(ierr);

    Bt_dense->assembled = PETSC_TRUE;
    multtrans->ABt_den  = C_dense;

#if defined(PETSC_USE_INFO)
    {
      Mat_SeqAIJ *c = (Mat_SeqAIJ*)(*C)->data;
      ierr = PetscInfo5(*C,"Bt_dense: %D,%D; Cnz %D / (cm*ncolors %D) = %g\n",A->cmap->n,matcoloring->ncolors,c->nz,A->rmap->n*matcoloring->ncolors,(PetscReal)(c->nz)/(A->rmap->n*matcoloring->ncolors));CHKERRQ(ierr);
    }
#endif
  }
  /* clean up */
  ierr = MatDestroy(&Bt);CHKERRQ(ierr);
  ierr = MatRestoreSymbolicTranspose_SeqAIJ(B,&bti,&btj);CHKERRQ(ierr);



#if defined(INEFFICIENT_ALGORITHM)
  /* The algorithm below computes am*bm sparse inner-product - inefficient! It will be deleted later. */
  PetscFreeSpaceList free_space=NULL,current_space=NULL;
  Mat_SeqAIJ         *a        =(Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c;
  PetscInt           *ai       =a->i,*aj=a->j,*bi=b->i,*bj=b->j,*ci,*cj,*acol,*bcol;
  PetscInt           am        =A->rmap->N,bm=B->rmap->N;
  PetscInt           i,j,anzi,bnzj,cnzi,nlnk,*lnk,nspacedouble=0,ka,kb,index[1];
  MatScalar          *ca;
  PetscBT            lnkbt;
  PetscReal          afill;

  /* Allocate row pointer array ci  */
  ierr  = PetscMalloc(((am+1)+1)*sizeof(PetscInt),&ci);CHKERRQ(ierr);
  ci[0] = 0;

  /* Create and initialize a linked list for C columns */
  nlnk = bm+1;
  ierr = PetscLLCreate(bm,bm,nlnk,lnk,lnkbt);CHKERRQ(ierr);

  /* Initial FreeSpace with size fill*(nnz(A)+nnz(B)) */
  ierr          = PetscFreeSpaceGet((PetscInt)(fill*(ai[am]+bi[bm])),&free_space);CHKERRQ(ierr);
  current_space = free_space;

  /* Determine symbolic info for each row of the product A*B^T: */
  for (i=0; i<am; i++) {
    anzi = ai[i+1] - ai[i];
    cnzi = 0;
    acol = aj + ai[i];
    for (j=0; j<bm; j++) {
      bnzj = bi[j+1] - bi[j];
      bcol = bj + bi[j];
      /* sparse inner-product c(i,j)=A[i,:]*B[j,:]^T */
      ka = 0; kb = 0;
      while (ka < anzi && kb < bnzj) {
        while (acol[ka] < bcol[kb] && ka < anzi) ka++;
        if (ka == anzi) break;
        while (acol[ka] > bcol[kb] && kb < bnzj) kb++;
        if (kb == bnzj) break;
        if (acol[ka] == bcol[kb]) { /* add nonzero c(i,j) to lnk */
          index[0] = j;
          ierr     = PetscLLAdd(1,index,bm,nlnk,lnk,lnkbt);CHKERRQ(ierr);
          cnzi++;
          break;
        }
      }
    }

    /* If free space is not available, make more free space */
    /* Double the amount of total space in the list */
    if (current_space->local_remaining<cnzi) {
      ierr = PetscFreeSpaceGet(cnzi+current_space->total_array_size,&current_space);CHKERRQ(ierr);
      nspacedouble++;
    }

    /* Copy data into free space, then initialize lnk */
    ierr = PetscLLClean(bm,bm,cnzi,lnk,current_space->array,lnkbt);CHKERRQ(ierr);

    current_space->array           += cnzi;
    current_space->local_used      += cnzi;
    current_space->local_remaining -= cnzi;

    ci[i+1] = ci[i] + cnzi;
  }


  /* Column indices are in the list of free space.
     Allocate array cj, copy column indices to cj, and destroy list of free space */
  ierr = PetscMalloc((ci[am]+1)*sizeof(PetscInt),&cj);CHKERRQ(ierr);
  ierr = PetscFreeSpaceContiguous(&free_space,cj);CHKERRQ(ierr);
  ierr = PetscLLDestroy(lnk,lnkbt);CHKERRQ(ierr);

  /* Allocate space for ca */
  ierr = PetscMalloc((ci[am]+1)*sizeof(MatScalar),&ca);CHKERRQ(ierr);
  ierr = PetscMemzero(ca,(ci[am]+1)*sizeof(MatScalar));CHKERRQ(ierr);

  /* put together the new symbolic matrix */
  ierr = MatCreateSeqAIJWithArrays(PetscObjectComm((PetscObject)A),am,bm,ci,cj,ca,C);CHKERRQ(ierr);

  (*C)->rmap->bs = A->cmap->bs;
  (*C)->cmap->bs = B->cmap->bs;

  /* MatCreateSeqAIJWithArrays flags matrix so PETSc doesn't free the user's arrays. */
  /* These are PETSc arrays, so change flags so arrays can be deleted by PETSc */
  c          = (Mat_SeqAIJ*)((*C)->data);
  c->free_a  = PETSC_TRUE;
  c->free_ij = PETSC_TRUE;
  c->nonew   = 0;

  /* set MatInfo */
  afill = (PetscReal)ci[am]/(ai[am]+bi[bm]) + 1.e-5;
  if (afill < 1.0) afill = 1.0;
  c->maxnz                     = ci[am];
  c->nz                        = ci[am];
  (*C)->info.mallocs           = nspacedouble;
  (*C)->info.fill_ratio_given  = fill;
  (*C)->info.fill_ratio_needed = afill;

#if defined(PETSC_USE_INFO)
  if (ci[am]) {
    ierr = PetscInfo3((*C),"Reallocs %D; Fill ratio: given %G needed %G.\n",nspacedouble,fill,afill);CHKERRQ(ierr);
    ierr = PetscInfo1((*C),"Use MatMatTransposeMult(A,B,MatReuse,%G,&C) for best performance.;\n",afill);CHKERRQ(ierr);
  } else {
    ierr = PetscInfo((*C),"Empty matrix product\n");CHKERRQ(ierr);
  }
#endif
#endif
  PetscFunctionReturn(0);
}

/* #define USE_ARRAY - for sparse dot product. Slower than !USE_ARRAY */
#undef __FUNCT__
#define __FUNCT__ "MatMatTransposeMultNumeric_SeqAIJ_SeqAIJ"
PetscErrorCode MatMatTransposeMultNumeric_SeqAIJ_SeqAIJ(Mat A,Mat B,Mat C)
{
  PetscErrorCode      ierr;
  Mat_SeqAIJ          *a   =(Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c=(Mat_SeqAIJ*)C->data;
  PetscInt            *ai  =a->i,*aj=a->j,*bi=b->i,*bj=b->j,anzi,bnzj,nexta,nextb,*acol,*bcol,brow;
  PetscInt            cm   =C->rmap->n,*ci=c->i,*cj=c->j,i,j,cnzi,*ccol;
  PetscLogDouble      flops=0.0;
  MatScalar           *aa  =a->a,*aval,*ba=b->a,*bval,*ca,*cval;
  Mat_MatMatTransMult *multtrans;
  PetscContainer      container;
#if defined(USE_ARRAY)
  MatScalar *spdot;
#endif

  PetscFunctionBegin;
  /* clear old values in C */
  if (!c->a) {
    ierr      = PetscMalloc((ci[cm]+1)*sizeof(MatScalar),&ca);CHKERRQ(ierr);
    c->a      = ca;
    c->free_a = PETSC_TRUE;
  } else {
    ca =  c->a;
  }
  ierr = PetscMemzero(ca,ci[cm]*sizeof(MatScalar));CHKERRQ(ierr);

  ierr = PetscObjectQuery((PetscObject)C,"Mat_MatMatTransMult",(PetscObject*)&container);CHKERRQ(ierr);
  if (!container) SETERRQ(PETSC_COMM_SELF,PETSC_ERR_PLIB,"Container does not exit");
  ierr = PetscContainerGetPointer(container,(void**)&multtrans);CHKERRQ(ierr);
  if (multtrans->usecoloring) {
    MatTransposeColoring matcoloring = multtrans->matcoloring;
    Mat                  Bt_dense;
    PetscInt             m,n;
    Mat                  C_dense = multtrans->ABt_den;

    Bt_dense = multtrans->Bt_den;
    ierr     = MatGetLocalSize(Bt_dense,&m,&n);CHKERRQ(ierr);

    /* Get Bt_dense by Apply MatTransposeColoring to B */
    ierr = MatTransColoringApplySpToDen(matcoloring,B,Bt_dense);CHKERRQ(ierr);

    /* C_dense = A*Bt_dense */
    ierr = MatMatMultNumeric_SeqAIJ_SeqDense(A,Bt_dense,C_dense);CHKERRQ(ierr);

    /* Recover C from C_dense */
    ierr = MatTransColoringApplyDenToSp(matcoloring,C_dense,C);CHKERRQ(ierr);
    PetscFunctionReturn(0);
  }

#if defined(USE_ARRAY)
  /* allocate an array for implementing sparse inner-product */
  ierr = PetscMalloc((A->cmap->n+1)*sizeof(MatScalar),&spdot);CHKERRQ(ierr);
  ierr = PetscMemzero(spdot,(A->cmap->n+1)*sizeof(MatScalar));CHKERRQ(ierr);
#endif

  for (i=0; i<cm; i++) {
    anzi = ai[i+1] - ai[i];
    acol = aj + ai[i];
    aval = aa + ai[i];
    cnzi = ci[i+1] - ci[i];
    ccol = cj + ci[i];
    cval = ca + ci[i];
    for (j=0; j<cnzi; j++) {
      brow = ccol[j];
      bnzj = bi[brow+1] - bi[brow];
      bcol = bj + bi[brow];
      bval = ba + bi[brow];

      /* perform sparse inner-product c(i,j)=A[i,:]*B[j,:]^T */
#if defined(USE_ARRAY)
      /* put ba to spdot array */
      for (nextb=0; nextb<bnzj; nextb++) spdot[bcol[nextb]] = bval[nextb];
      /* c(i,j)=A[i,:]*B[j,:]^T */
      for (nexta=0; nexta<anzi; nexta++) {
        cval[j] += spdot[acol[nexta]]*aval[nexta];
      }
      /* zero spdot array */
      for (nextb=0; nextb<bnzj; nextb++) spdot[bcol[nextb]] = 0.0;
#else
      nexta = 0; nextb = 0;
      while (nexta<anzi && nextb<bnzj) {
        while (nexta < anzi && acol[nexta] < bcol[nextb]) nexta++;
        if (nexta == anzi) break;
        while (nextb < bnzj && acol[nexta] > bcol[nextb]) nextb++;
        if (nextb == bnzj) break;
        if (acol[nexta] == bcol[nextb]) {
          cval[j] += aval[nexta]*bval[nextb];
          nexta++; nextb++;
          flops += 2;
        }
      }
#endif
    }
  }
  ierr = MatAssemblyBegin(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = PetscLogFlops(flops);CHKERRQ(ierr);
#if defined(USE_ARRAY)
  ierr = PetscFree(spdot);
#endif
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatTransposeMatMult_SeqAIJ_SeqAIJ"
PetscErrorCode MatTransposeMatMult_SeqAIJ_SeqAIJ(Mat A,Mat B,MatReuse scall,PetscReal fill,Mat *C)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (scall == MAT_INITIAL_MATRIX) {
    ierr = MatTransposeMatMultSymbolic_SeqAIJ_SeqAIJ(A,B,fill,C);CHKERRQ(ierr);
  }
  ierr = MatTransposeMatMultNumeric_SeqAIJ_SeqAIJ(A,B,*C);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatTransposeMatMultSymbolic_SeqAIJ_SeqAIJ"
PetscErrorCode MatTransposeMatMultSymbolic_SeqAIJ_SeqAIJ(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode ierr;
  Mat            At;
  PetscInt       *ati,*atj;

  PetscFunctionBegin;
  /* create symbolic At */
  ierr = MatGetSymbolicTranspose_SeqAIJ(A,&ati,&atj);CHKERRQ(ierr);
  ierr = MatCreateSeqAIJWithArrays(PETSC_COMM_SELF,A->cmap->n,A->rmap->n,ati,atj,NULL,&At);CHKERRQ(ierr);

  At->rmap->bs = A->cmap->bs;
  At->cmap->bs = B->cmap->bs;

  /* get symbolic C=At*B */
  ierr = MatMatMultSymbolic_SeqAIJ_SeqAIJ(At,B,fill,C);CHKERRQ(ierr);

  /* clean up */
  ierr = MatDestroy(&At);CHKERRQ(ierr);
  ierr = MatRestoreSymbolicTranspose_SeqAIJ(A,&ati,&atj);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatTransposeMatMultNumeric_SeqAIJ_SeqAIJ"
PetscErrorCode MatTransposeMatMultNumeric_SeqAIJ_SeqAIJ(Mat A,Mat B,Mat C)
{
  PetscErrorCode ierr;
  Mat_SeqAIJ     *a   =(Mat_SeqAIJ*)A->data,*b=(Mat_SeqAIJ*)B->data,*c=(Mat_SeqAIJ*)C->data;
  PetscInt       am   =A->rmap->n,anzi,*ai=a->i,*aj=a->j,*bi=b->i,*bj,bnzi,nextb;
  PetscInt       cm   =C->rmap->n,*ci=c->i,*cj=c->j,crow,*cjj,i,j,k;
  PetscLogDouble flops=0.0;
  MatScalar      *aa  =a->a,*ba,*ca,*caj;

  PetscFunctionBegin;
  if (!c->a) {
    ierr = PetscMalloc((ci[cm]+1)*sizeof(MatScalar),&ca);CHKERRQ(ierr);

    c->a      = ca;
    c->free_a = PETSC_TRUE;
  } else {
    ca = c->a;
  }
  /* clear old values in C */
  ierr = PetscMemzero(ca,ci[cm]*sizeof(MatScalar));CHKERRQ(ierr);

  /* compute A^T*B using outer product (A^T)[:,i]*B[i,:] */
  for (i=0; i<am; i++) {
    bj   = b->j + bi[i];
    ba   = b->a + bi[i];
    bnzi = bi[i+1] - bi[i];
    anzi = ai[i+1] - ai[i];
    for (j=0; j<anzi; j++) {
      nextb = 0;
      crow  = *aj++;
      cjj   = cj + ci[crow];
      caj   = ca + ci[crow];
      /* perform sparse axpy operation.  Note cjj includes bj. */
      for (k=0; nextb<bnzi; k++) {
        if (cjj[k] == *(bj+nextb)) { /* ccol == bcol */
          caj[k] += (*aa)*(*(ba+nextb));
          nextb++;
        }
      }
      flops += 2*bnzi;
      aa++;
    }
  }

  /* Assemble the final matrix and clean up */
  ierr = MatAssemblyBegin(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = PetscLogFlops(flops);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMult_SeqAIJ_SeqDense"
PetscErrorCode MatMatMult_SeqAIJ_SeqDense(Mat A,Mat B,MatReuse scall,PetscReal fill,Mat *C)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (scall == MAT_INITIAL_MATRIX) {
    ierr = PetscLogEventBegin(MAT_MatMultSymbolic,A,B,0,0);CHKERRQ(ierr);
    ierr = MatMatMultSymbolic_SeqAIJ_SeqDense(A,B,fill,C);CHKERRQ(ierr);
    ierr = PetscLogEventEnd(MAT_MatMultSymbolic,A,B,0,0);CHKERRQ(ierr);
  }
  ierr = PetscLogEventBegin(MAT_MatMultNumeric,A,B,0,0);CHKERRQ(ierr);
  ierr = MatMatMultNumeric_SeqAIJ_SeqDense(A,B,*C);CHKERRQ(ierr);
  ierr = PetscLogEventEnd(MAT_MatMultNumeric,A,B,0,0);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultSymbolic_SeqAIJ_SeqDense"
PetscErrorCode MatMatMultSymbolic_SeqAIJ_SeqDense(Mat A,Mat B,PetscReal fill,Mat *C)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = MatMatMultSymbolic_SeqDense_SeqDense(A,B,0.0,C);CHKERRQ(ierr);

  (*C)->ops->matmultnumeric = MatMatMultNumeric_SeqAIJ_SeqDense;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatMatMultNumeric_SeqAIJ_SeqDense"
PetscErrorCode MatMatMultNumeric_SeqAIJ_SeqDense(Mat A,Mat B,Mat C)
{
  Mat_SeqAIJ     *a=(Mat_SeqAIJ*)A->data;
  PetscErrorCode ierr;
  PetscScalar    *b,*c,r1,r2,r3,r4,*b1,*b2,*b3,*b4;
  MatScalar      *aa;
  PetscInt       cm  = C->rmap->n, cn=B->cmap->n, bm=B->rmap->n, col, i,j,n,*aj, am = A->rmap->n;
  PetscInt       am2 = 2*am, am3 = 3*am,  bm4 = 4*bm,colam;

  PetscFunctionBegin;
  if (!cm || !cn) PetscFunctionReturn(0);
  if (bm != A->cmap->n) SETERRQ2(PETSC_COMM_SELF,PETSC_ERR_ARG_SIZ,"Number columns in A %D not equal rows in B %D\n",A->cmap->n,bm);
  if (A->rmap->n != C->rmap->n) SETERRQ2(PETSC_COMM_SELF,PETSC_ERR_ARG_SIZ,"Number rows in C %D not equal rows in A %D\n",C->rmap->n,A->rmap->n);
  if (B->cmap->n != C->cmap->n) SETERRQ2(PETSC_COMM_SELF,PETSC_ERR_ARG_SIZ,"Number columns in B %D not equal columns in C %D\n",B->cmap->n,C->cmap->n);
  ierr = MatDenseGetArray(B,&b);CHKERRQ(ierr);
  ierr = MatDenseGetArray(C,&c);CHKERRQ(ierr);
  b1   = b; b2 = b1 + bm; b3 = b2 + bm; b4 = b3 + bm;
  for (col=0; col<cn-4; col += 4) {  /* over columns of C */
    colam = col*am;
    for (i=0; i<am; i++) {        /* over rows of C in those columns */
      r1 = r2 = r3 = r4 = 0.0;
      n  = a->i[i+1] - a->i[i];
      aj = a->j + a->i[i];
      aa = a->a + a->i[i];
      for (j=0; j<n; j++) {
        r1 += (*aa)*b1[*aj];
        r2 += (*aa)*b2[*aj];
        r3 += (*aa)*b3[*aj];
        r4 += (*aa++)*b4[*aj++];
      }
      c[colam + i]       = r1;
      c[colam + am + i]  = r2;
      c[colam + am2 + i] = r3;
      c[colam + am3 + i] = r4;
    }
    b1 += bm4;
    b2 += bm4;
    b3 += bm4;
    b4 += bm4;
  }
  for (; col<cn; col++) {     /* over extra columns of C */
    for (i=0; i<am; i++) {  /* over rows of C in those columns */
      r1 = 0.0;
      n  = a->i[i+1] - a->i[i];
      aj = a->j + a->i[i];
      aa = a->a + a->i[i];

      for (j=0; j<n; j++) {
        r1 += (*aa++)*b1[*aj++];
      }
      c[col*am + i] = r1;
    }
    b1 += bm;
  }
  ierr = PetscLogFlops(cn*(2.0*a->nz));CHKERRQ(ierr);
  ierr = MatDenseRestoreArray(B,&b);CHKERRQ(ierr);
  ierr = MatDenseRestoreArray(C,&c);CHKERRQ(ierr);
  ierr = MatAssemblyBegin(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(C,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

/*
   Note very similar to MatMult_SeqAIJ(), should generate both codes from same base
*/
#undef __FUNCT__
#define __FUNCT__ "MatMatMultNumericAdd_SeqAIJ_SeqDense"
PetscErrorCode MatMatMultNumericAdd_SeqAIJ_SeqDense(Mat A,Mat B,Mat C)
{
  Mat_SeqAIJ     *a=(Mat_SeqAIJ*)A->data;
  PetscErrorCode ierr;
  PetscScalar    *b,*c,r1,r2,r3,r4,*b1,*b2,*b3,*b4;
  MatScalar      *aa;
  PetscInt       cm  = C->rmap->n, cn=B->cmap->n, bm=B->rmap->n, col, i,j,n,*aj, am = A->rmap->n,*ii,arm;
  PetscInt       am2 = 2*am, am3 = 3*am,  bm4 = 4*bm,colam,*ridx;

  PetscFunctionBegin;
  if (!cm || !cn) PetscFunctionReturn(0);
  ierr = MatDenseGetArray(B,&b);CHKERRQ(ierr);
  ierr = MatDenseGetArray(C,&c);CHKERRQ(ierr);
  b1   = b; b2 = b1 + bm; b3 = b2 + bm; b4 = b3 + bm;

  if (a->compressedrow.use) { /* use compressed row format */
    for (col=0; col<cn-4; col += 4) {  /* over columns of C */
      colam = col*am;
      arm   = a->compressedrow.nrows;
      ii    = a->compressedrow.i;
      ridx  = a->compressedrow.rindex;
      for (i=0; i<arm; i++) {        /* over rows of C in those columns */
        r1 = r2 = r3 = r4 = 0.0;
        n  = ii[i+1] - ii[i];
        aj = a->j + ii[i];
        aa = a->a + ii[i];
        for (j=0; j<n; j++) {
          r1 += (*aa)*b1[*aj];
          r2 += (*aa)*b2[*aj];
          r3 += (*aa)*b3[*aj];
          r4 += (*aa++)*b4[*aj++];
        }
        c[colam       + ridx[i]] += r1;
        c[colam + am  + ridx[i]] += r2;
        c[colam + am2 + ridx[i]] += r3;
        c[colam + am3 + ridx[i]] += r4;
      }
      b1 += bm4;
      b2 += bm4;
      b3 += bm4;
      b4 += bm4;
    }
    for (; col<cn; col++) {     /* over extra columns of C */
      colam = col*am;
      arm   = a->compressedrow.nrows;
      ii    = a->compressedrow.i;
      ridx  = a->compressedrow.rindex;
      for (i=0; i<arm; i++) {  /* over rows of C in those columns */
        r1 = 0.0;
        n  = ii[i+1] - ii[i];
        aj = a->j + ii[i];
        aa = a->a + ii[i];

        for (j=0; j<n; j++) {
          r1 += (*aa++)*b1[*aj++];
        }
        c[colam + ridx[i]] += r1;
      }
      b1 += bm;
    }
  } else {
    for (col=0; col<cn-4; col += 4) {  /* over columns of C */
      colam = col*am;
      for (i=0; i<am; i++) {        /* over rows of C in those columns */
        r1 = r2 = r3 = r4 = 0.0;
        n  = a->i[i+1] - a->i[i];
        aj = a->j + a->i[i];
        aa = a->a + a->i[i];
        for (j=0; j<n; j++) {
          r1 += (*aa)*b1[*aj];
          r2 += (*aa)*b2[*aj];
          r3 += (*aa)*b3[*aj];
          r4 += (*aa++)*b4[*aj++];
        }
        c[colam + i]       += r1;
        c[colam + am + i]  += r2;
        c[colam + am2 + i] += r3;
        c[colam + am3 + i] += r4;
      }
      b1 += bm4;
      b2 += bm4;
      b3 += bm4;
      b4 += bm4;
    }
    for (; col<cn; col++) {     /* over extra columns of C */
      colam = col*am;
      for (i=0; i<am; i++) {  /* over rows of C in those columns */
        r1 = 0.0;
        n  = a->i[i+1] - a->i[i];
        aj = a->j + a->i[i];
        aa = a->a + a->i[i];

        for (j=0; j<n; j++) {
          r1 += (*aa++)*b1[*aj++];
        }
        c[colam + i] += r1;
      }
      b1 += bm;
    }
  }
  ierr = PetscLogFlops(cn*2.0*a->nz);CHKERRQ(ierr);
  ierr = MatDenseRestoreArray(B,&b);CHKERRQ(ierr);
  ierr = MatDenseRestoreArray(C,&c);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatTransColoringApplySpToDen_SeqAIJ"
PetscErrorCode  MatTransColoringApplySpToDen_SeqAIJ(MatTransposeColoring coloring,Mat B,Mat Btdense)
{
  PetscErrorCode ierr;
  Mat_SeqAIJ     *b       = (Mat_SeqAIJ*)B->data;
  Mat_SeqDense   *btdense = (Mat_SeqDense*)Btdense->data;
  PetscInt       *bi      = b->i,*bj=b->j;
  PetscInt       m        = Btdense->rmap->n,n=Btdense->cmap->n,j,k,l,col,anz,*btcol,brow,ncolumns;
  MatScalar      *btval,*btval_den,*ba=b->a;
  PetscInt       *columns=coloring->columns,*colorforcol=coloring->colorforcol,ncolors=coloring->ncolors;

  PetscFunctionBegin;
  btval_den=btdense->v;
  ierr     = PetscMemzero(btval_den,(m*n)*sizeof(MatScalar));CHKERRQ(ierr);
  for (k=0; k<ncolors; k++) {
    ncolumns = coloring->ncolumns[k];
    for (l=0; l<ncolumns; l++) { /* insert a row of B to a column of Btdense */
      col   = *(columns + colorforcol[k] + l);
      btcol = bj + bi[col];
      btval = ba + bi[col];
      anz   = bi[col+1] - bi[col];
      for (j=0; j<anz; j++) {
        brow            = btcol[j];
        btval_den[brow] = btval[j];
      }
    }
    btval_den += m;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatTransColoringApplyDenToSp_SeqAIJ"
PetscErrorCode MatTransColoringApplyDenToSp_SeqAIJ(MatTransposeColoring matcoloring,Mat Cden,Mat Csp)
{
  PetscErrorCode ierr;
  Mat_SeqAIJ     *csp = (Mat_SeqAIJ*)Csp->data;
  PetscInt       k,l,*row,*idx,m,ncolors=matcoloring->ncolors,nrows;
  PetscScalar    *ca_den,*cp_den,*ca=csp->a;
  PetscInt       *rows=matcoloring->rows,*spidx=matcoloring->columnsforspidx,*colorforrow=matcoloring->colorforrow;

  PetscFunctionBegin;
  ierr   = MatGetLocalSize(Csp,&m,NULL);CHKERRQ(ierr);
  ierr   = MatDenseGetArray(Cden,&ca_den);CHKERRQ(ierr);
  cp_den = ca_den;
  for (k=0; k<ncolors; k++) {
    nrows = matcoloring->nrows[k];
    row   = rows  + colorforrow[k];
    idx   = spidx + colorforrow[k];
    for (l=0; l<nrows; l++) {
      ca[idx[l]] = cp_den[row[l]];
    }
    cp_den += m;
  }
  ierr = MatDenseRestoreArray(Cden,&ca_den);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

/*
 MatGetColumnIJ_SeqAIJ_Color() and MatRestoreColumnIJ_SeqAIJ_Color() are customized from
 MatGetColumnIJ_SeqAIJ() and MatRestoreColumnIJ_SeqAIJ() by adding an output
 spidx[], index of a->j, to be used for setting 'columnsforspidx' in MatTransposeColoringCreate_SeqAIJ().
 */
#undef __FUNCT__
#define __FUNCT__ "MatGetColumnIJ_SeqAIJ_Color"
PetscErrorCode MatGetColumnIJ_SeqAIJ_Color(Mat A,PetscInt oshift,PetscBool symmetric,PetscBool inodecompressed,PetscInt *nn,const PetscInt *ia[],const PetscInt *ja[],PetscInt *spidx[],PetscBool  *done)
{
  Mat_SeqAIJ     *a = (Mat_SeqAIJ*)A->data;
  PetscErrorCode ierr;
  PetscInt       i,*collengths,*cia,*cja,n = A->cmap->n,m = A->rmap->n;
  PetscInt       nz = a->i[m],row,*jj,mr,col;
  PetscInt       *cspidx;

  PetscFunctionBegin;
  *nn = n;
  if (!ia) PetscFunctionReturn(0);
  if (symmetric) {
    SETERRQ(PetscObjectComm((PetscObject)A),PETSC_ERR_SUP,"MatGetColumnIJ_SeqAIJ_Color() not supported for the case symmetric");
    ierr = MatToSymmetricIJ_SeqAIJ(A->rmap->n,a->i,a->j,0,oshift,(PetscInt**)ia,(PetscInt**)ja);CHKERRQ(ierr);
  } else {
    ierr = PetscMalloc((n+1)*sizeof(PetscInt),&collengths);CHKERRQ(ierr);
    ierr = PetscMemzero(collengths,n*sizeof(PetscInt));CHKERRQ(ierr);
    ierr = PetscMalloc((n+1)*sizeof(PetscInt),&cia);CHKERRQ(ierr);
    ierr = PetscMalloc((nz+1)*sizeof(PetscInt),&cja);CHKERRQ(ierr);
    ierr = PetscMalloc((nz+1)*sizeof(PetscInt),&cspidx);CHKERRQ(ierr);
    jj   = a->j;
    for (i=0; i<nz; i++) {
      collengths[jj[i]]++;
    }
    cia[0] = oshift;
    for (i=0; i<n; i++) {
      cia[i+1] = cia[i] + collengths[i];
    }
    ierr = PetscMemzero(collengths,n*sizeof(PetscInt));CHKERRQ(ierr);
    jj   = a->j;
    for (row=0; row<m; row++) {
      mr = a->i[row+1] - a->i[row];
      for (i=0; i<mr; i++) {
        col = *jj++;

        cspidx[cia[col] + collengths[col] - oshift] = a->i[row] + i; /* index of a->j */
        cja[cia[col] + collengths[col]++ - oshift]  = row + oshift;
      }
    }
    ierr   = PetscFree(collengths);CHKERRQ(ierr);
    *ia    = cia; *ja = cja;
    *spidx = cspidx;
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatRestoreColumnIJ_SeqAIJ_Color"
PetscErrorCode MatRestoreColumnIJ_SeqAIJ_Color(Mat A,PetscInt oshift,PetscBool symmetric,PetscBool inodecompressed,PetscInt *n,const PetscInt *ia[],const PetscInt *ja[],PetscInt *spidx[],PetscBool  *done)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (!ia) PetscFunctionReturn(0);

  ierr = PetscFree(*ia);CHKERRQ(ierr);
  ierr = PetscFree(*ja);CHKERRQ(ierr);
  ierr = PetscFree(*spidx);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "MatTransposeColoringCreate_SeqAIJ"
PetscErrorCode MatTransposeColoringCreate_SeqAIJ(Mat mat,ISColoring iscoloring,MatTransposeColoring c)
{
  PetscErrorCode ierr;
  PetscInt       i,n,nrows,N,j,k,m,ncols,col,cm;
  const PetscInt *is,*ci,*cj,*row_idx;
  PetscInt       nis = iscoloring->n,*rowhit,bs = 1;
  IS             *isa;
  PetscBool      flg1,flg2;
  Mat_SeqAIJ     *csp = (Mat_SeqAIJ*)mat->data;
  PetscInt       *colorforrow,*rows,*rows_i,*columnsforspidx,*columnsforspidx_i,*idxhit,*spidx;
  PetscInt       *colorforcol,*columns,*columns_i;

  PetscFunctionBegin;
  ierr = ISColoringGetIS(iscoloring,PETSC_IGNORE,&isa);CHKERRQ(ierr);

  /* this is ugly way to get blocksize but cannot call MatGetBlockSize() because AIJ can have bs > 1 */
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATSEQBAIJ,&flg1);CHKERRQ(ierr);
  ierr = PetscObjectTypeCompare((PetscObject)mat,MATMPIBAIJ,&flg2);CHKERRQ(ierr);
  if (flg1 || flg2) {
    ierr = MatGetBlockSize(mat,&bs);CHKERRQ(ierr);
  }

  N         = mat->cmap->N/bs;
  c->M      = mat->rmap->N/bs;  /* set total rows, columns and local rows */
  c->N      = mat->cmap->N/bs;
  c->m      = mat->rmap->N/bs;
  c->rstart = 0;

  c->ncolors = nis;
  ierr       = PetscMalloc(nis*sizeof(PetscInt),&c->ncolumns);CHKERRQ(ierr);
  ierr       = PetscMalloc(nis*sizeof(PetscInt),&c->nrows);CHKERRQ(ierr);
  ierr       = PetscMalloc2(csp->nz+1,PetscInt,&rows,csp->nz+1,PetscInt,&columnsforspidx);CHKERRQ(ierr);
  ierr       = PetscMalloc((nis+1)*sizeof(PetscInt),&colorforrow);CHKERRQ(ierr);

  colorforrow[0]    = 0;
  rows_i            = rows;
  columnsforspidx_i = columnsforspidx;

  ierr = PetscMalloc((nis+1)*sizeof(PetscInt),&colorforcol);CHKERRQ(ierr);
  ierr = PetscMalloc((N+1)*sizeof(PetscInt),&columns);CHKERRQ(ierr);

  colorforcol[0] = 0;
  columns_i      = columns;

  ierr = MatGetColumnIJ_SeqAIJ_Color(mat,0,PETSC_FALSE,PETSC_FALSE,&ncols,&ci,&cj,&spidx,NULL);CHKERRQ(ierr); /* column-wise storage of mat */

  cm   = c->m;
  ierr = PetscMalloc((cm+1)*sizeof(PetscInt),&rowhit);CHKERRQ(ierr);
  ierr = PetscMalloc((cm+1)*sizeof(PetscInt),&idxhit);CHKERRQ(ierr);
  for (i=0; i<nis; i++) {
    ierr = ISGetLocalSize(isa[i],&n);CHKERRQ(ierr);
    ierr = ISGetIndices(isa[i],&is);CHKERRQ(ierr);

    c->ncolumns[i] = n;
    if (n) {
      ierr = PetscMemcpy(columns_i,is,n*sizeof(PetscInt));CHKERRQ(ierr);
    }
    colorforcol[i+1] = colorforcol[i] + n;
    columns_i       += n;

    /* fast, crude version requires O(N*N) work */
    ierr = PetscMemzero(rowhit,cm*sizeof(PetscInt));CHKERRQ(ierr);

    /* loop over columns*/
    for (j=0; j<n; j++) {
      col     = is[j];
      row_idx = cj + ci[col];
      m       = ci[col+1] - ci[col];
      /* loop over columns marking them in rowhit */
      for (k=0; k<m; k++) {
        idxhit[*row_idx]   = spidx[ci[col] + k];
        rowhit[*row_idx++] = col + 1;
      }
    }
    /* count the number of hits */
    nrows = 0;
    for (j=0; j<cm; j++) {
      if (rowhit[j]) nrows++;
    }
    c->nrows[i]      = nrows;
    colorforrow[i+1] = colorforrow[i] + nrows;

    nrows = 0;
    for (j=0; j<cm; j++) {
      if (rowhit[j]) {
        rows_i[nrows]            = j;
        columnsforspidx_i[nrows] = idxhit[j];
        nrows++;
      }
    }
    ierr    = ISRestoreIndices(isa[i],&is);CHKERRQ(ierr);
    rows_i += nrows; columnsforspidx_i += nrows;
  }
  ierr = MatRestoreColumnIJ_SeqAIJ_Color(mat,0,PETSC_FALSE,PETSC_FALSE,&ncols,&ci,&cj,&spidx,NULL);CHKERRQ(ierr);
  ierr = PetscFree(rowhit);CHKERRQ(ierr);
  ierr = ISColoringRestoreIS(iscoloring,&isa);CHKERRQ(ierr);
#if defined(PETSC_USE_DEBUG)
  if (csp->nz != colorforrow[nis]) SETERRQ2(PETSC_COMM_SELF,PETSC_ERR_PLIB,"csp->nz %d != colorforrow[nis] %d",csp->nz,colorforrow[nis]);
#endif

  c->colorforrow     = colorforrow;
  c->rows            = rows;
  c->columnsforspidx = columnsforspidx;
  c->colorforcol     = colorforcol;
  c->columns         = columns;

  ierr = PetscFree(idxhit);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}
