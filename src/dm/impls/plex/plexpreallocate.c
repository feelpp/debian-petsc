#include <petsc-private/dmpleximpl.h>   /*I      "petscdmplex.h"   I*/
#include <petsc-private/isimpl.h>
#include <petscsf.h>

#undef __FUNCT__
#define __FUNCT__ "DMPlexGetAdjacency_Internal"
static PetscErrorCode DMPlexGetAdjacency_Internal(DM dm, PetscInt p, PetscBool useClosure, const PetscInt *tmpClosure, PetscInt *adjSize, PetscInt adj[])
{
  const PetscInt *star  = tmpClosure;
  PetscInt        numAdj = 0, maxAdjSize = *adjSize, starSize, s;
  PetscErrorCode  ierr;

  PetscFunctionBegin;
  ierr = DMPlexGetTransitiveClosure(dm, p, useClosure, &starSize, (PetscInt**) &star);CHKERRQ(ierr);
  for (s = 2; s < starSize*2; s += 2) {
    const PetscInt *closure = NULL;
    PetscInt        closureSize, c, q;

    ierr = DMPlexGetTransitiveClosure(dm, star[s], (PetscBool)!useClosure, &closureSize, (PetscInt**) &closure);CHKERRQ(ierr);
    for (c = 0; c < closureSize*2; c += 2) {
      for (q = 0; q < numAdj || (adj[numAdj++] = closure[c],0); ++q) {
        if (closure[c] == adj[q]) break;
      }
      if (numAdj > maxAdjSize) SETERRQ1(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid mesh exceeded adjacency allocation (%D)", maxAdjSize);
    }
    ierr = DMPlexRestoreTransitiveClosure(dm, star[s], (PetscBool)!useClosure, &closureSize, (PetscInt**) &closure);CHKERRQ(ierr);
  }
  *adjSize = numAdj;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "DMPlexSetPreallocationCenterDimension"
PetscErrorCode DMPlexSetPreallocationCenterDimension(DM dm, PetscInt preallocCenterDim)
{
  DM_Plex *mesh = (DM_Plex*) dm->data;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(dm, DM_CLASSID, 1);
  mesh->preallocCenterDim = preallocCenterDim;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "DMPlexPreallocateOperator"
PetscErrorCode DMPlexPreallocateOperator(DM dm, PetscInt bs, PetscSection section, PetscSection sectionGlobal, PetscInt dnz[], PetscInt onz[], PetscInt dnzu[], PetscInt onzu[], Mat A, PetscBool fillMatrix)
{
  DM_Plex           *mesh = (DM_Plex*) dm->data;
  MPI_Comm           comm;
  PetscSF            sf, sfDof, sfAdj;
  PetscSection       leafSectionAdj, rootSectionAdj, sectionAdj;
  PetscInt           nleaves, l, p;
  const PetscInt    *leaves;
  const PetscSFNode *remotes;
  PetscInt           dim, pStart, pEnd, numDof, globalOffStart, globalOffEnd, numCols;
  PetscInt          *tmpClosure, *tmpAdj, *adj, *rootAdj, *cols, *remoteOffsets;
  PetscInt           depth, maxConeSize, maxSupportSize, maxClosureSize, maxAdjSize, adjSize;
  PetscLayout        rLayout;
  PetscInt           locRows, rStart, rEnd, r;
  PetscMPIInt        size;
  PetscBool          useClosure, debug = PETSC_FALSE;
  PetscErrorCode     ierr;

  PetscFunctionBegin;
  ierr = PetscObjectGetComm((PetscObject)dm,&comm);CHKERRQ(ierr);
  ierr = PetscOptionsGetBool(NULL, "-dm_view_preallocation", &debug, NULL);CHKERRQ(ierr);
  ierr = MPI_Comm_size(comm, &size);CHKERRQ(ierr);
  ierr = DMPlexGetDimension(dm, &dim);CHKERRQ(ierr);
  ierr = DMGetPointSF(dm, &sf);CHKERRQ(ierr);
  /* Create dof SF based on point SF */
  if (debug) {
    ierr = PetscPrintf(comm, "Input Section for Preallocation:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(section, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = PetscPrintf(comm, "Input Global Section for Preallocation:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(sectionGlobal, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = PetscPrintf(comm, "Input SF for Preallocation:\n");CHKERRQ(ierr);
    ierr = PetscSFView(sf, NULL);CHKERRQ(ierr);
  }
  ierr = PetscSFCreateRemoteOffsets(sf, section, section, &remoteOffsets);CHKERRQ(ierr);
  ierr = PetscSFCreateSectionSF(sf, section, remoteOffsets, section, &sfDof);CHKERRQ(ierr);
  if (debug) {
    ierr = PetscPrintf(comm, "Dof SF for Preallocation:\n");CHKERRQ(ierr);
    ierr = PetscSFView(sfDof, NULL);CHKERRQ(ierr);
  }
  /* Create section for dof adjacency (dof ==> # adj dof) */
  /*   FEM:   Two points p and q are adjacent if q \in closure(star(p)), preallocCenterDim = dim   */
  /*   FVM:   Two points p and q are adjacent if q \in star(cone(p)),    preallocCenterDim = dim-1 */
  /*   FVM++: Two points p and q are adjacent if q \in star(closure(p)), preallocCenterDim = 0     */
  if (mesh->preallocCenterDim == dim) {
    useClosure = PETSC_FALSE;
  } else if (mesh->preallocCenterDim == 0) {
    useClosure = PETSC_TRUE;
  } else SETERRQ1(comm, PETSC_ERR_ARG_OUTOFRANGE, "Do not support preallocation with center points of dimension %d", mesh->preallocCenterDim);

  ierr = PetscSectionGetChart(section, &pStart, &pEnd);CHKERRQ(ierr);
  ierr = PetscSectionGetStorageSize(section, &numDof);CHKERRQ(ierr);
  ierr = PetscSectionCreate(comm, &leafSectionAdj);CHKERRQ(ierr);
  ierr = PetscSectionSetChart(leafSectionAdj, 0, numDof);CHKERRQ(ierr);
  ierr = PetscSectionCreate(comm, &rootSectionAdj);CHKERRQ(ierr);
  ierr = PetscSectionSetChart(rootSectionAdj, 0, numDof);CHKERRQ(ierr);
  /*   Fill in the ghost dofs on the interface */
  ierr = PetscSFGetGraph(sf, NULL, &nleaves, &leaves, &remotes);CHKERRQ(ierr);
  ierr = DMPlexGetDepth(dm, &depth);CHKERRQ(ierr);
  ierr = DMPlexGetMaxSizes(dm, &maxConeSize, &maxSupportSize);CHKERRQ(ierr);

  maxClosureSize = 2*PetscMax(PetscPowInt(mesh->maxConeSize,depth+1),PetscPowInt(mesh->maxSupportSize,depth+1));
  maxAdjSize     = PetscPowInt(mesh->maxConeSize,depth+1) * PetscPowInt(mesh->maxSupportSize,depth+1) + 1;

  ierr = PetscMalloc2(maxClosureSize,PetscInt,&tmpClosure,maxAdjSize,PetscInt,&tmpAdj);CHKERRQ(ierr);

  /*
   ** The bootstrapping process involves six rounds with similar structure of visiting neighbors of each point.
    1. Visit unowned points on interface, count adjacencies placing in leafSectionAdj
       Reduce those counts to rootSectionAdj (now redundantly counting some interface points)
    2. Visit owned points on interface, count adjacencies placing in rootSectionAdj
       Create sfAdj connecting rootSectionAdj and leafSectionAdj
    3. Visit unowned points on interface, write adjacencies to adj
       Gather adj to rootAdj (note that there is redundancy in rootAdj when multiple procs find the same adjacencies)
    4. Visit owned points on interface, write adjacencies to rootAdj
       Remove redundancy in rootAdj
   ** The last two traversals use transitive closure
    5. Visit all owned points in the subdomain, count dofs for each point (sectionAdj)
       Allocate memory addressed by sectionAdj (cols)
    6. Visit all owned points in the subdomain, insert dof adjacencies into cols
   ** Knowing all the column adjacencies, check ownership and sum into dnz and onz
  */

  for (l = 0; l < nleaves; ++l) {
    PetscInt dof, off, d, q;
    PetscInt p = leaves[l], numAdj = maxAdjSize;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    ierr = DMPlexGetAdjacency_Internal(dm, p, useClosure, tmpClosure, &numAdj, tmpAdj);CHKERRQ(ierr);
    for (q = 0; q < numAdj; ++q) {
      const PetscInt padj = tmpAdj[q];
      PetscInt ndof, ncdof;

      if ((padj < pStart) || (padj >= pEnd)) continue;
      ierr = PetscSectionGetDof(section, padj, &ndof);CHKERRQ(ierr);
      ierr = PetscSectionGetConstraintDof(section, padj, &ncdof);CHKERRQ(ierr);
      for (d = off; d < off+dof; ++d) {
        ierr = PetscSectionAddDof(leafSectionAdj, d, ndof-ncdof);CHKERRQ(ierr);
      }
    }
  }
  ierr = PetscSectionSetUp(leafSectionAdj);CHKERRQ(ierr);
  if (debug) {
    ierr = PetscPrintf(comm, "Adjacency Section for Preallocation on Leaves:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(leafSectionAdj, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  }
  /* Get maximum remote adjacency sizes for owned dofs on interface (roots) */
  if (size > 1) {
    ierr = PetscSFReduceBegin(sfDof, MPIU_INT, leafSectionAdj->atlasDof, rootSectionAdj->atlasDof, MPI_SUM);CHKERRQ(ierr);
    ierr = PetscSFReduceEnd(sfDof, MPIU_INT, leafSectionAdj->atlasDof, rootSectionAdj->atlasDof, MPI_SUM);CHKERRQ(ierr);
  }
  if (debug) {
    ierr = PetscPrintf(comm, "Adjancency Section for Preallocation on Roots:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(rootSectionAdj, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  }
  /* Add in local adjacency sizes for owned dofs on interface (roots) */
  for (p = pStart; p < pEnd; ++p) {
    PetscInt numAdj = maxAdjSize, adof, dof, off, d, q;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    if (!dof) continue;
    ierr = PetscSectionGetDof(rootSectionAdj, off, &adof);CHKERRQ(ierr);
    if (adof <= 0) continue;
    ierr = DMPlexGetAdjacency_Internal(dm, p, useClosure, tmpClosure, &numAdj, tmpAdj);CHKERRQ(ierr);
    for (q = 0; q < numAdj; ++q) {
      const PetscInt padj = tmpAdj[q];
      PetscInt ndof, ncdof;

      if ((padj < pStart) || (padj >= pEnd)) continue;
      ierr = PetscSectionGetDof(section, padj, &ndof);CHKERRQ(ierr);
      ierr = PetscSectionGetConstraintDof(section, padj, &ncdof);CHKERRQ(ierr);
      for (d = off; d < off+dof; ++d) {
        ierr = PetscSectionAddDof(rootSectionAdj, d, ndof-ncdof);CHKERRQ(ierr);
      }
    }
  }
  ierr = PetscSectionSetUp(rootSectionAdj);CHKERRQ(ierr);
  if (debug) {
    ierr = PetscPrintf(comm, "Adjancency Section for Preallocation on Roots after local additions:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(rootSectionAdj, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  }
  /* Create adj SF based on dof SF */
  ierr = PetscSFCreateRemoteOffsets(sfDof, rootSectionAdj, leafSectionAdj, &remoteOffsets);CHKERRQ(ierr);
  ierr = PetscSFCreateSectionSF(sfDof, rootSectionAdj, remoteOffsets, leafSectionAdj, &sfAdj);CHKERRQ(ierr);
  if (debug) {
    ierr = PetscPrintf(comm, "Adjacency SF for Preallocation:\n");CHKERRQ(ierr);
    ierr = PetscSFView(sfAdj, NULL);CHKERRQ(ierr);
  }
  ierr = PetscSFDestroy(&sfDof);CHKERRQ(ierr);
  /* Create leaf adjacency */
  ierr = PetscSectionSetUp(leafSectionAdj);CHKERRQ(ierr);
  ierr = PetscSectionGetStorageSize(leafSectionAdj, &adjSize);CHKERRQ(ierr);
  ierr = PetscMalloc(adjSize * sizeof(PetscInt), &adj);CHKERRQ(ierr);
  ierr = PetscMemzero(adj, adjSize * sizeof(PetscInt));CHKERRQ(ierr);
  for (l = 0; l < nleaves; ++l) {
    PetscInt dof, off, d, q;
    PetscInt p = leaves[l], numAdj = maxAdjSize;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    ierr = DMPlexGetAdjacency_Internal(dm, p, useClosure, tmpClosure, &numAdj, tmpAdj);CHKERRQ(ierr);
    for (d = off; d < off+dof; ++d) {
      PetscInt aoff, i = 0;

      ierr = PetscSectionGetOffset(leafSectionAdj, d, &aoff);CHKERRQ(ierr);
      for (q = 0; q < numAdj; ++q) {
        const PetscInt padj = tmpAdj[q];
        PetscInt ndof, ncdof, ngoff, nd;

        if ((padj < pStart) || (padj >= pEnd)) continue;
        ierr = PetscSectionGetDof(section, padj, &ndof);CHKERRQ(ierr);
        ierr = PetscSectionGetConstraintDof(section, padj, &ncdof);CHKERRQ(ierr);
        ierr = PetscSectionGetOffset(sectionGlobal, padj, &ngoff);CHKERRQ(ierr);
        for (nd = 0; nd < ndof-ncdof; ++nd) {
          adj[aoff+i] = (ngoff < 0 ? -(ngoff+1) : ngoff) + nd;
          ++i;
        }
      }
    }
  }
  /* Debugging */
  if (debug) {
    IS tmp;
    ierr = PetscPrintf(comm, "Leaf adjacency indices\n");CHKERRQ(ierr);
    ierr = ISCreateGeneral(comm, adjSize, adj, PETSC_USE_POINTER, &tmp);CHKERRQ(ierr);
    ierr = ISView(tmp, NULL);CHKERRQ(ierr);
  }
  /* Gather adjacenct indices to root */
  ierr = PetscSectionGetStorageSize(rootSectionAdj, &adjSize);CHKERRQ(ierr);
  ierr = PetscMalloc(adjSize * sizeof(PetscInt), &rootAdj);CHKERRQ(ierr);
  for (r = 0; r < adjSize; ++r) rootAdj[r] = -1;
  if (size > 1) {
    ierr = PetscSFGatherBegin(sfAdj, MPIU_INT, adj, rootAdj);CHKERRQ(ierr);
    ierr = PetscSFGatherEnd(sfAdj, MPIU_INT, adj, rootAdj);CHKERRQ(ierr);
  }
  ierr = PetscSFDestroy(&sfAdj);CHKERRQ(ierr);
  ierr = PetscFree(adj);CHKERRQ(ierr);
  /* Debugging */
  if (debug) {
    IS tmp;
    ierr = PetscPrintf(comm, "Root adjacency indices after gather\n");CHKERRQ(ierr);
    ierr = ISCreateGeneral(comm, adjSize, rootAdj, PETSC_USE_POINTER, &tmp);CHKERRQ(ierr);
    ierr = ISView(tmp, NULL);CHKERRQ(ierr);
  }
  /* Add in local adjacency indices for owned dofs on interface (roots) */
  for (p = pStart; p < pEnd; ++p) {
    PetscInt numAdj = maxAdjSize, adof, dof, off, d, q;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    if (!dof) continue;
    ierr = PetscSectionGetDof(rootSectionAdj, off, &adof);CHKERRQ(ierr);
    if (adof <= 0) continue;
    ierr = DMPlexGetAdjacency_Internal(dm, p, useClosure, tmpClosure, &numAdj, tmpAdj);CHKERRQ(ierr);
    for (d = off; d < off+dof; ++d) {
      PetscInt adof, aoff, i;

      ierr = PetscSectionGetDof(rootSectionAdj, d, &adof);CHKERRQ(ierr);
      ierr = PetscSectionGetOffset(rootSectionAdj, d, &aoff);CHKERRQ(ierr);
      i    = adof-1;
      for (q = 0; q < numAdj; ++q) {
        const PetscInt padj = tmpAdj[q];
        PetscInt ndof, ncdof, ngoff, nd;

        if ((padj < pStart) || (padj >= pEnd)) continue;
        ierr = PetscSectionGetDof(section, padj, &ndof);CHKERRQ(ierr);
        ierr = PetscSectionGetConstraintDof(section, padj, &ncdof);CHKERRQ(ierr);
        ierr = PetscSectionGetOffset(sectionGlobal, padj, &ngoff);CHKERRQ(ierr);
        for (nd = 0; nd < ndof-ncdof; ++nd) {
          rootAdj[aoff+i] = ngoff < 0 ? -(ngoff+1)+nd : ngoff+nd;
          --i;
        }
      }
    }
  }
  /* Debugging */
  if (debug) {
    IS tmp;
    ierr = PetscPrintf(comm, "Root adjacency indices\n");CHKERRQ(ierr);
    ierr = ISCreateGeneral(comm, adjSize, rootAdj, PETSC_USE_POINTER, &tmp);CHKERRQ(ierr);
    ierr = ISView(tmp, NULL);CHKERRQ(ierr);
  }
  /* Compress indices */
  ierr = PetscSectionSetUp(rootSectionAdj);CHKERRQ(ierr);
  for (p = pStart; p < pEnd; ++p) {
    PetscInt dof, cdof, off, d;
    PetscInt adof, aoff;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetConstraintDof(section, p, &cdof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    if (!dof) continue;
    ierr = PetscSectionGetDof(rootSectionAdj, off, &adof);CHKERRQ(ierr);
    if (adof <= 0) continue;
    for (d = off; d < off+dof-cdof; ++d) {
      ierr = PetscSectionGetDof(rootSectionAdj, d, &adof);CHKERRQ(ierr);
      ierr = PetscSectionGetOffset(rootSectionAdj, d, &aoff);CHKERRQ(ierr);
      ierr = PetscSortRemoveDupsInt(&adof, &rootAdj[aoff]);CHKERRQ(ierr);
      ierr = PetscSectionSetDof(rootSectionAdj, d, adof);CHKERRQ(ierr);
    }
  }
  /* Debugging */
  if (debug) {
    IS tmp;
    ierr = PetscPrintf(comm, "Adjancency Section for Preallocation on Roots after compression:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(rootSectionAdj, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
    ierr = PetscPrintf(comm, "Root adjacency indices after compression\n");CHKERRQ(ierr);
    ierr = ISCreateGeneral(comm, adjSize, rootAdj, PETSC_USE_POINTER, &tmp);CHKERRQ(ierr);
    ierr = ISView(tmp, NULL);CHKERRQ(ierr);
  }
  /* Build adjacency section: Maps global indices to sets of adjacent global indices */
  ierr = PetscSectionGetOffsetRange(sectionGlobal, &globalOffStart, &globalOffEnd);CHKERRQ(ierr);
  ierr = PetscSectionCreate(comm, &sectionAdj);CHKERRQ(ierr);
  ierr = PetscSectionSetChart(sectionAdj, globalOffStart, globalOffEnd);CHKERRQ(ierr);
  for (p = pStart; p < pEnd; ++p) {
    PetscInt  numAdj = maxAdjSize, dof, cdof, off, goff, d, q;
    PetscBool found  = PETSC_TRUE;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetConstraintDof(section, p, &cdof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(sectionGlobal, p, &goff);CHKERRQ(ierr);
    for (d = 0; d < dof-cdof; ++d) {
      PetscInt ldof, rdof;

      ierr = PetscSectionGetDof(leafSectionAdj, off+d, &ldof);CHKERRQ(ierr);
      ierr = PetscSectionGetDof(rootSectionAdj, off+d, &rdof);CHKERRQ(ierr);
      if (ldof > 0) {
        /* We do not own this point */
      } else if (rdof > 0) {
        ierr = PetscSectionSetDof(sectionAdj, goff+d, rdof);CHKERRQ(ierr);
      } else {
        found = PETSC_FALSE;
      }
    }
    if (found) continue;
    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(sectionGlobal, p, &goff);CHKERRQ(ierr);
    ierr = DMPlexGetAdjacency_Internal(dm, p, useClosure, tmpClosure, &numAdj, tmpAdj);CHKERRQ(ierr);
    for (q = 0; q < numAdj; ++q) {
      const PetscInt padj = tmpAdj[q];
      PetscInt ndof, ncdof, noff;

      if ((padj < pStart) || (padj >= pEnd)) continue;
      ierr = PetscSectionGetDof(section, padj, &ndof);CHKERRQ(ierr);
      ierr = PetscSectionGetConstraintDof(section, padj, &ncdof);CHKERRQ(ierr);
      ierr = PetscSectionGetOffset(section, padj, &noff);CHKERRQ(ierr);
      for (d = goff; d < goff+dof-cdof; ++d) {
        ierr = PetscSectionAddDof(sectionAdj, d, ndof-ncdof);CHKERRQ(ierr);
      }
    }
  }
  ierr = PetscSectionSetUp(sectionAdj);CHKERRQ(ierr);
  if (debug) {
    ierr = PetscPrintf(comm, "Adjacency Section for Preallocation:\n");CHKERRQ(ierr);
    ierr = PetscSectionView(sectionAdj, PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  }
  /* Get adjacent indices */
  ierr = PetscSectionGetStorageSize(sectionAdj, &numCols);CHKERRQ(ierr);
  ierr = PetscMalloc(numCols * sizeof(PetscInt), &cols);CHKERRQ(ierr);
  for (p = pStart; p < pEnd; ++p) {
    PetscInt  numAdj = maxAdjSize, dof, cdof, off, goff, d, q;
    PetscBool found  = PETSC_TRUE;

    ierr = PetscSectionGetDof(section, p, &dof);CHKERRQ(ierr);
    ierr = PetscSectionGetConstraintDof(section, p, &cdof);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(section, p, &off);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(sectionGlobal, p, &goff);CHKERRQ(ierr);
    for (d = 0; d < dof-cdof; ++d) {
      PetscInt ldof, rdof;

      ierr = PetscSectionGetDof(leafSectionAdj, off+d, &ldof);CHKERRQ(ierr);
      ierr = PetscSectionGetDof(rootSectionAdj, off+d, &rdof);CHKERRQ(ierr);
      if (ldof > 0) {
        /* We do not own this point */
      } else if (rdof > 0) {
        PetscInt aoff, roff;

        ierr = PetscSectionGetOffset(sectionAdj, goff+d, &aoff);CHKERRQ(ierr);
        ierr = PetscSectionGetOffset(rootSectionAdj, off+d, &roff);CHKERRQ(ierr);
        ierr = PetscMemcpy(&cols[aoff], &rootAdj[roff], rdof * sizeof(PetscInt));CHKERRQ(ierr);
      } else {
        found = PETSC_FALSE;
      }
    }
    if (found) continue;
    ierr = DMPlexGetAdjacency_Internal(dm, p, useClosure, tmpClosure, &numAdj, tmpAdj);CHKERRQ(ierr);
    for (d = goff; d < goff+dof-cdof; ++d) {
      PetscInt adof, aoff, i = 0;

      ierr = PetscSectionGetDof(sectionAdj, d, &adof);CHKERRQ(ierr);
      ierr = PetscSectionGetOffset(sectionAdj, d, &aoff);CHKERRQ(ierr);
      for (q = 0; q < numAdj; ++q) {
        const PetscInt  padj = tmpAdj[q];
        PetscInt        ndof, ncdof, ngoff, nd;
        const PetscInt *ncind;

        /* Adjacent points may not be in the section chart */
        if ((padj < pStart) || (padj >= pEnd)) continue;
        ierr = PetscSectionGetDof(section, padj, &ndof);CHKERRQ(ierr);
        ierr = PetscSectionGetConstraintDof(section, padj, &ncdof);CHKERRQ(ierr);
        ierr = PetscSectionGetConstraintIndices(section, padj, &ncind);CHKERRQ(ierr);
        ierr = PetscSectionGetOffset(sectionGlobal, padj, &ngoff);CHKERRQ(ierr);
        for (nd = 0; nd < ndof-ncdof; ++nd, ++i) {
          cols[aoff+i] = ngoff < 0 ? -(ngoff+1)+nd : ngoff+nd;
        }
      }
      if (i != adof) SETERRQ4(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid number of entries %D != %D for dof %D (point %D)", i, adof, d, p);
    }
  }
  ierr = PetscSectionDestroy(&leafSectionAdj);CHKERRQ(ierr);
  ierr = PetscSectionDestroy(&rootSectionAdj);CHKERRQ(ierr);
  ierr = PetscFree(rootAdj);CHKERRQ(ierr);
  ierr = PetscFree2(tmpClosure, tmpAdj);CHKERRQ(ierr);
  /* Debugging */
  if (debug) {
    IS tmp;
    ierr = PetscPrintf(comm, "Column indices\n");CHKERRQ(ierr);
    ierr = ISCreateGeneral(comm, numCols, cols, PETSC_USE_POINTER, &tmp);CHKERRQ(ierr);
    ierr = ISView(tmp, NULL);CHKERRQ(ierr);
  }
  /* Create allocation vectors from adjacency graph */
  ierr = MatGetLocalSize(A, &locRows, NULL);CHKERRQ(ierr);
  ierr = PetscLayoutCreate(PetscObjectComm((PetscObject)A), &rLayout);CHKERRQ(ierr);
  ierr = PetscLayoutSetLocalSize(rLayout, locRows);CHKERRQ(ierr);
  ierr = PetscLayoutSetBlockSize(rLayout, 1);CHKERRQ(ierr);
  ierr = PetscLayoutSetUp(rLayout);CHKERRQ(ierr);
  ierr = PetscLayoutGetRange(rLayout, &rStart, &rEnd);CHKERRQ(ierr);
  ierr = PetscLayoutDestroy(&rLayout);CHKERRQ(ierr);
  /* Only loop over blocks of rows */
  if (rStart%bs || rEnd%bs) SETERRQ3(PetscObjectComm((PetscObject)A), PETSC_ERR_ARG_WRONG, "Invalid layout [%d, %d) for matrix, must be divisible by block size %d", rStart, rEnd, bs);
  for (r = rStart/bs; r < rEnd/bs; ++r) {
    const PetscInt row = r*bs;
    PetscInt       numCols, cStart, c;

    ierr = PetscSectionGetDof(sectionAdj, row, &numCols);CHKERRQ(ierr);
    ierr = PetscSectionGetOffset(sectionAdj, row, &cStart);CHKERRQ(ierr);
    for (c = cStart; c < cStart+numCols; ++c) {
      if ((cols[c] >= rStart*bs) && (cols[c] < rEnd*bs)) {
        ++dnz[r-rStart];
        if (cols[c] >= row) ++dnzu[r-rStart];
      } else {
        ++onz[r-rStart];
        if (cols[c] >= row) ++onzu[r-rStart];
      }
    }
  }
  if (bs > 1) {
    for (r = 0; r < locRows/bs; ++r) {
      dnz[r]  /= bs;
      onz[r]  /= bs;
      dnzu[r] /= bs;
      onzu[r] /= bs;
    }
  }
  /* Set matrix pattern */
  ierr = MatXAIJSetPreallocation(A, bs, dnz, onz, dnzu, onzu);CHKERRQ(ierr);
  ierr = MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR,PETSC_TRUE);CHKERRQ(ierr);
  /* Fill matrix with zeros */
  if (fillMatrix) {
    PetscScalar *values;
    PetscInt     maxRowLen = 0;

    for (r = rStart; r < rEnd; ++r) {
      PetscInt len;

      ierr      = PetscSectionGetDof(sectionAdj, r, &len);CHKERRQ(ierr);
      maxRowLen = PetscMax(maxRowLen, len);
    }
    ierr = PetscMalloc(maxRowLen * sizeof(PetscScalar), &values);CHKERRQ(ierr);
    ierr = PetscMemzero(values, maxRowLen * sizeof(PetscScalar));CHKERRQ(ierr);
    for (r = rStart; r < rEnd; ++r) {
      PetscInt numCols, cStart;

      ierr = PetscSectionGetDof(sectionAdj, r, &numCols);CHKERRQ(ierr);
      ierr = PetscSectionGetOffset(sectionAdj, r, &cStart);CHKERRQ(ierr);
      ierr = MatSetValues(A, 1, &r, numCols, &cols[cStart], values, INSERT_VALUES);CHKERRQ(ierr);
    }
    ierr = PetscFree(values);CHKERRQ(ierr);
    ierr = MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  }
  ierr = PetscSectionDestroy(&sectionAdj);CHKERRQ(ierr);
  ierr = PetscFree(cols);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#if 0
#undef __FUNCT__
#define __FUNCT__ "DMPlexPreallocateOperator_2"
PetscErrorCode DMPlexPreallocateOperator_2(DM dm, PetscInt bs, PetscSection section, PetscSection sectionGlobal, PetscInt dnz[], PetscInt onz[], PetscInt dnzu[], PetscInt onzu[], Mat A, PetscBool fillMatrix)
{
  PetscInt       *tmpClosure,*tmpAdj,*visits;
  PetscInt        c,cStart,cEnd,pStart,pEnd;
  PetscErrorCode  ierr;

  PetscFunctionBegin;
  ierr = DMPlexGetDimension(dm, &dim);CHKERRQ(ierr);
  ierr = DMPlexGetDepth(dm, &depth);CHKERRQ(ierr);
  ierr = DMPlexGetMaxSizes(dm, &maxConeSize, &maxSupportSize);CHKERRQ(ierr);

  maxClosureSize = 2*PetscMax(PetscPowInt(mesh->maxConeSize,depth+1),PetscPowInt(mesh->maxSupportSize,depth+1));

  ierr    = PetscSectionGetChart(section, &pStart, &pEnd);CHKERRQ(ierr);
  npoints = pEnd - pStart;

  ierr = PetscMalloc3(maxClosureSize,PetscInt,&tmpClosure,npoints,PetscInt,&lvisits,npoints,PetscInt,&visits);CHKERRQ(ierr);
  ierr = PetscMemzero(lvisits,(pEnd-pStart)*sizeof(PetscInt));CHKERRQ(ierr);
  ierr = PetscMemzero(visits,(pEnd-pStart)*sizeof(PetscInt));CHKERRQ(ierr);
  ierr = DMPlexGetHeightStratum(dm, 0, &cStart, &cEnd);CHKERRQ(ierr);
  for (c=cStart; c<cEnd; c++) {
    PetscInt *support = tmpClosure;
    ierr = DMPlexGetTransitiveClosure(dm, c, PETSC_FALSE, &supportSize, (PetscInt**)&support);CHKERRQ(ierr);
    for (p=0; p<supportSize; p++) lvisits[support[p]]++;
  }
  ierr = PetscSFReduceBegin(sf,MPIU_INT,lvisits,visits,MPI_SUM);CHKERRQ(ierr);
  ierr = PetscSFReduceEnd  (sf,MPIU_INT,lvisits,visits,MPI_SUM);CHKERRQ(ierr);
  ierr = PetscSFBcastBegin(sf,MPIU_INT,visits,lvisits);CHKERRQ(ierr);
  ierr = PetscSFBcastEnd  (sf,MPIU_INT,visits,lvisits);CHKERRQ(ierr);

  ierr = PetscSFGetRanks();CHKERRQ(ierr);


  ierr = PetscMalloc2(maxClosureSize*maxClosureSize,PetscInt,&cellmat,npoints,PetscInt,&owner);CHKERRQ(ierr);
  for (c=cStart; c<cEnd; c++) {
    ierr = PetscMemzero(cellmat,maxClosureSize*maxClosureSize*sizeof(PetscInt));CHKERRQ(ierr);
    /*
     Depth-first walk of transitive closure.
     At each leaf frame f of transitive closure that we see, add 1/visits[f] to each pair (p,q) not marked as done in cellmat.
     This contribution is added to dnz if owning ranks of p and q match, to onz otherwise.
     */
  }

  ierr = PetscSFReduceBegin(sf,MPIU_INT,ldnz,dnz,MPI_SUM);CHKERRQ(ierr);
  ierr = PetscSFReduceEnd  (sf,MPIU_INT,lonz,onz,MPI_SUM);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}
#endif
