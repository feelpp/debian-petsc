static char help[] = "Tests for cell geometry\n\n";

/* TODO
*/

#include <petscdmplex.h>

#undef __FUNCT__
#define __FUNCT__ "ChangeCoordinates"
PetscErrorCode ChangeCoordinates(DM dm, PetscInt spaceDim, PetscScalar vertexCoords[])
{
  PetscSection   coordSection;
  Vec            coordinates;
  PetscScalar   *coords;
  PetscInt       vStart, vEnd, v, d, coordSize;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMPlexGetDepthStratum(dm, 0, &vStart, &vEnd);CHKERRQ(ierr);
  ierr = DMPlexGetCoordinateSection(dm, &coordSection);CHKERRQ(ierr);
  ierr = PetscSectionSetNumFields(coordSection, 1);CHKERRQ(ierr);
  ierr = PetscSectionSetFieldComponents(coordSection, 0, spaceDim);CHKERRQ(ierr);
  ierr = PetscSectionSetChart(coordSection, vStart, vEnd);CHKERRQ(ierr);
  for (v = vStart; v < vEnd; ++v) {
    ierr = PetscSectionSetDof(coordSection, v, spaceDim);CHKERRQ(ierr);
    ierr = PetscSectionSetFieldDof(coordSection, v, 0, spaceDim);CHKERRQ(ierr);
  }
  ierr = PetscSectionSetUp(coordSection);CHKERRQ(ierr);
  ierr = PetscSectionGetStorageSize(coordSection, &coordSize);CHKERRQ(ierr);
  ierr = VecCreate(PetscObjectComm((PetscObject) dm), &coordinates);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) coordinates, "coordinates");CHKERRQ(ierr);
  ierr = VecSetSizes(coordinates, coordSize, PETSC_DETERMINE);CHKERRQ(ierr);
  ierr = VecSetFromOptions(coordinates);CHKERRQ(ierr);
  ierr = VecGetArray(coordinates, &coords);CHKERRQ(ierr);
  for (v = vStart; v < vEnd; ++v) {
    PetscInt off;

    ierr = PetscSectionGetOffset(coordSection, v, &off);CHKERRQ(ierr);
    for (d = 0; d < spaceDim; ++d) {
      coords[off+d] = vertexCoords[(v-vStart)*spaceDim+d];
    }
  }
  ierr = VecRestoreArray(coordinates, &coords);CHKERRQ(ierr);
  ierr = DMSetCoordinatesLocal(dm, coordinates);CHKERRQ(ierr);
  ierr = VecDestroy(&coordinates);CHKERRQ(ierr);
  ierr = DMSetFromOptions(dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "CheckFEMGeometry"
PetscErrorCode CheckFEMGeometry(DM dm, PetscInt cell, PetscInt spaceDim, PetscReal v0Ex[], PetscReal JEx[], PetscReal invJEx[], PetscReal detJEx)
{
  PetscReal      v0[3], J[9], invJ[9], detJ;
  PetscInt       d, i, j;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMPlexComputeCellGeometry(dm, cell, v0, J, invJ, &detJ);CHKERRQ(ierr);
  for (d = 0; d < spaceDim; ++d) {
    if (v0[d] != v0Ex[d]) {
      switch (spaceDim) {
      case 2: SETERRQ4(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid v0 (%g, %g) != (%g, %g)", v0[0], v0[1], v0Ex[0], v0Ex[1]);break;
      case 3: SETERRQ6(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid v0 (%g, %g, %g) != (%g, %g, %g)", v0[0], v0[1], v0[2], v0Ex[0], v0Ex[1], v0Ex[2]);break;
      default: SETERRQ1(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid space dimension %d", spaceDim);
      }
    }
  }
  for (i = 0; i < spaceDim; ++i) {
    for (j = 0; j < spaceDim; ++j) {
      if (fabs(J[i*spaceDim+j]    - JEx[i*spaceDim+j])    > 1.0e-9) SETERRQ4(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid J[%d,%d]: %g != %g", i, j, J[i*spaceDim+j], JEx[i*spaceDim+j]);
      if (fabs(invJ[i*spaceDim+j] - invJEx[i*spaceDim+j]) > 1.0e-9) SETERRQ4(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid invJ[%d,%d]: %g != %g", i, j, invJ[i*spaceDim+j], invJEx[i*spaceDim+j]);
    }
  }
  if (fabs(detJ - detJEx) > 1.0e-9) SETERRQ2(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid |J| = %g != %g", detJ, detJEx);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "CheckFVMGeometry"
PetscErrorCode CheckFVMGeometry(DM dm, PetscInt cell, PetscInt spaceDim, PetscReal centroidEx[], PetscReal normalEx[], PetscReal volEx)
{
  PetscReal      centroid[3], normal[3], vol;
  PetscInt       d;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = DMPlexComputeCellGeometryFVM(dm, cell, &vol, centroid, normal);CHKERRQ(ierr);
  for (d = 0; d < spaceDim; ++d) {
    if (fabs(centroid[d] - centroidEx[d]) > 1.0e-9) SETERRQ3(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid centroid[%d]: %g != %g", d, centroid[d], centroidEx[d]);
    if (fabs(normal[d] - normalEx[d]) > 1.0e-9) SETERRQ3(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid normal[%d]: %g != %g", d, normal[d], normalEx[d]);
  }
  if (fabs(volEx - vol) > 1.0e-9) SETERRQ2(PETSC_COMM_SELF, PETSC_ERR_PLIB, "Invalid volume = %g != %g", vol, volEx);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TestTriangle"
PetscErrorCode TestTriangle(MPI_Comm comm, PetscBool interpolate, PetscBool transform)
{
  DM             dm;
  PetscRandom    r, ang, ang2;
  PetscInt       dim, t;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  /* Create reference triangle */
  dim  = 2;
  ierr = DMCreate(comm, &dm);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) dm, "triangle");CHKERRQ(ierr);
  ierr = DMSetType(dm, DMPLEX);CHKERRQ(ierr);
  ierr = DMPlexSetDimension(dm, dim);CHKERRQ(ierr);
  {
    PetscInt    numPoints[2]        = {3, 1};
    PetscInt    coneSize[4]         = {3, 0, 0, 0};
    PetscInt    cones[3]            = {1, 2, 3};
    PetscInt    coneOrientations[3] = {0, 0, 0};
    PetscScalar vertexCoords[6]     = {-1.0, -1.0, 1.0, -1.0, -1.0, 1.0};

    ierr = DMPlexCreateFromDAG(dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
    if (interpolate) {
      DM idm;

      ierr = DMPlexInterpolate(dm, &idm);CHKERRQ(ierr);
      ierr = PetscObjectSetName((PetscObject) idm, "triangle");CHKERRQ(ierr);
      ierr = DMPlexCopyCoordinates(dm, idm);CHKERRQ(ierr);
      ierr = DMDestroy(&dm);CHKERRQ(ierr);
      dm   = idm;
    }
    ierr = DMSetFromOptions(dm);CHKERRQ(ierr);
  }
  /* Check reference geometry: determinant is scaled by reference volume (2.0) */
  {
    PetscReal v0Ex[2]       = {-1.0, -1.0};
    PetscReal JEx[4]        = {1.0, 0.0, 0.0, 1.0};
    PetscReal invJEx[4]     = {1.0, 0.0, 0.0, 1.0};
    PetscReal detJEx        = 1.0;
    PetscReal centroidEx[2] = {-0.333333333333, -0.333333333333};
    PetscReal normalEx[2]   = {0.0, 0.0};
    PetscReal volEx         = 2.0;

    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Check random triangles: rotate, scale, then translate */
  if (transform) {
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &r);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(r);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(r, 0.0, 10.0);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang, 0.0, 2*PETSC_PI);CHKERRQ(ierr);
    for (t = 0; t < 100; ++t) {
      PetscScalar vertexCoords[6] = {-1.0, -1.0, 1.0, -1.0, -1.0, 1.0}, trans[2];
      PetscReal   v0Ex[2]         = {-1.0, -1.0};
      PetscReal   JEx[4]          = {1.0, 0.0, 0.0, 1.0}, R[4], rot[2], rotM[4];
      PetscReal   invJEx[4]       = {1.0, 0.0, 0.0, 1.0};
      PetscReal   detJEx          = 1.0, scale, phi;
      PetscReal   centroidEx[2]   = {-0.333333333333, -0.333333333333};
      PetscReal   normalEx[2]     = {0.0, 0.0};
      PetscReal   volEx           = 2.0;
      PetscInt    d, e, f, p;

      ierr = PetscRandomGetValueReal(r, &scale);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang, &phi);CHKERRQ(ierr);
      R[0] = cos(phi); R[1] = -sin(phi);
      R[2] = sin(phi); R[3] =  cos(phi);
      for (p = 0; p < 3; ++p) {
        for (d = 0; d < dim; ++d) {
          for (e = 0, rot[d] = 0.0; e < dim; ++e) {
            rot[d] += R[d*dim+e] * vertexCoords[p*dim+e];
          }
        }
        for (d = 0; d < dim; ++d) vertexCoords[p*dim+d] = rot[d];
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * centroidEx[e];
        }
      }
      for (d = 0; d < dim; ++d) centroidEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += R[d*dim+f] * JEx[f*dim+e];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += invJEx[d*dim+f] * R[e*dim+f];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          invJEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        ierr = PetscRandomGetValueReal(r, &trans[d]);CHKERRQ(ierr);
        for (p = 0; p < 3; ++p) {
          vertexCoords[p*dim+d] *= scale;
          vertexCoords[p*dim+d] += trans[d];
        }
        v0Ex[d] = vertexCoords[d];
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e]    *= scale;
          invJEx[d*dim+e] /= scale;
        }
        detJEx *= scale;
        centroidEx[d] *= scale;
        centroidEx[d] += trans[d];
        volEx *= scale;
      }
      ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
      ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
      if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
    }
    ierr = PetscRandomDestroy(&r);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang);CHKERRQ(ierr);
  }
  /* Move to 3D: Check reference geometry: determinant is scaled by reference volume (2.0) */
  dim = 3;
  {
    PetscScalar vertexCoords[9] = {-1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0, 0.0};
    PetscReal v0Ex[3]       = {-1.0, -1.0, 0.0};
    PetscReal JEx[9]        = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal invJEx[9]     = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal detJEx        = 1.0;
    PetscReal centroidEx[3] = {-0.333333333333, -0.333333333333, 0.0};
    PetscReal normalEx[3]   = {0.0, 0.0, 1.0};
    PetscReal volEx         = 2.0;

    ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Rotated reference element */
  {
    PetscScalar vertexCoords[9] = {0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0};
    PetscReal   v0Ex[3]         = {0.0, -1.0, -1.0};
    PetscReal   JEx[9]          = {0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0};
    PetscReal   invJEx[9]       = {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0};
    PetscReal   detJEx          = 1.0;
    PetscReal   centroidEx[3]   = {0.0, -0.333333333333, -0.333333333333};
    PetscReal   normalEx[3]     = {1.0, 0.0, 0.0};
    PetscReal   volEx           = 2.0;

    ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Check random triangles: scale, translate, then rotate */
  if (transform) {
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &r);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(r);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(r, 0.0, 10.0);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang, 0.0, 2*PETSC_PI);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang2, 0.0, PETSC_PI);CHKERRQ(ierr);
    for (t = 0; t < 100; ++t) {
      PetscScalar vertexCoords[9] = {-1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0, 0.0}, trans[3];
      PetscReal   v0Ex[3]         = {-1.0, -1.0, 0.0};
      PetscReal   JEx[9]          = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, R[9], rot[3], rotM[9];
      PetscReal   invJEx[9]       = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
      PetscReal   detJEx          = 1.0, scale, phi, theta, psi = 0.0;
      PetscReal   centroidEx[3]   = {-0.333333333333, -0.333333333333, 0.0};
      PetscReal   normalEx[3]     = {0.0, 0.0, 1.0};
      PetscReal   volEx           = 2.0;
      PetscInt    d, e, f, p;

      ierr = PetscRandomGetValueReal(r, &scale);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang, &phi);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang2, &theta);CHKERRQ(ierr);
      for (d = 0; d < dim; ++d) {
        ierr = PetscRandomGetValueReal(r, &trans[d]);CHKERRQ(ierr);
        for (p = 0; p < 3; ++p) {
          vertexCoords[p*dim+d] *= scale;
          vertexCoords[p*dim+d] += trans[d];
        }
        centroidEx[d] *= scale;
        centroidEx[d] += trans[d];
        for (e = 0; e < dim-1; ++e) {
          JEx[d*dim+e]    *= scale;
          invJEx[d*dim+e] /= scale;
        }
        if (d < dim-1) {
          detJEx *= scale;
          volEx  *= scale;
        }
      }
      R[0] = cos(theta)*cos(psi); R[1] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi); R[2] = sin(phi)*sin(psi) + cos(phi)*sin(theta)*cos(psi);
      R[3] = cos(theta)*sin(psi); R[4] = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi); R[5] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
      R[6] = -sin(theta);         R[7] = sin(phi)*cos(theta);                              R[8] = cos(phi)*cos(theta);
      for (p = 0; p < 3; ++p) {
        for (d = 0; d < dim; ++d) {
          for (e = 0, rot[d] = 0.0; e < dim; ++e) {
            rot[d] += R[d*dim+e] * vertexCoords[p*dim+e];
          }
        }
        for (d = 0; d < dim; ++d) vertexCoords[p*dim+d] = rot[d];
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * centroidEx[e];
        }
      }
      for (d = 0; d < dim; ++d) centroidEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * normalEx[e];
        }
      }
      for (d = 0; d < dim; ++d) normalEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        v0Ex[d] = vertexCoords[d];
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += R[d*dim+f] * JEx[f*dim+e];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += invJEx[d*dim+f] * R[e*dim+f];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          invJEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
      ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
      if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
    }
    ierr = PetscRandomDestroy(&r);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang2);CHKERRQ(ierr);
  }
  /* Cleanup */
  ierr = DMDestroy(&dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TestQuadrilateral"
PetscErrorCode TestQuadrilateral(MPI_Comm comm, PetscBool interpolate, PetscBool transform)
{
  DM             dm;
  PetscRandom    r, ang, ang2;
  PetscInt       dim, t;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  /* Create reference quadrilateral */
  dim  = 2;
  ierr = DMCreate(comm, &dm);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) dm, "quadrilateral");CHKERRQ(ierr);
  ierr = DMSetType(dm, DMPLEX);CHKERRQ(ierr);
  ierr = DMPlexSetDimension(dm, dim);CHKERRQ(ierr);
  {
    PetscInt    numPoints[2]        = {4, 1};
    PetscInt    coneSize[5]         = {4, 0, 0, 0, 0};
    PetscInt    cones[4]            = {1, 2, 3, 4};
    PetscInt    coneOrientations[4] = {0, 0, 0, 0};
    PetscScalar vertexCoords[8]     = {-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0};

    ierr = DMPlexCreateFromDAG(dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
    if (interpolate) {
      DM idm;

      ierr = DMPlexInterpolate(dm, &idm);CHKERRQ(ierr);
      ierr = PetscObjectSetName((PetscObject) idm, "quadrilateral");CHKERRQ(ierr);
      ierr = DMPlexCopyCoordinates(dm, idm);CHKERRQ(ierr);
      ierr = DMDestroy(&dm);CHKERRQ(ierr);
      dm   = idm;
    }
    ierr = DMSetFromOptions(dm);CHKERRQ(ierr);
  }
  /* Check reference geometry: determinant is scaled by reference volume (2.0) */
  {
    PetscReal v0Ex[2]       = {-1.0, -1.0};
    PetscReal JEx[4]        = {1.0, 0.0, 0.0, 1.0};
    PetscReal invJEx[4]     = {1.0, 0.0, 0.0, 1.0};
    PetscReal detJEx        = 1.0;
    PetscReal centroidEx[2] = {0.0, 0.0};
    PetscReal normalEx[2]   = {0.0, 0.0};
    PetscReal volEx         = 4.0;

    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Check random quadrilaterals: rotate, scale, then translate */
  if (transform) {
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &r);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(r);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(r, 0.0, 10.0);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang, 0.0, 2*PETSC_PI);CHKERRQ(ierr);
    for (t = 0; t < 100; ++t) {
      PetscScalar vertexCoords[8] = {-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0}, trans[2];
      PetscReal   v0Ex[2]         = {-1.0, -1.0};
      PetscReal   JEx[4]          = {1.0, 0.0, 0.0, 1.0}, R[4], rot[2], rotM[4];
      PetscReal   invJEx[4]       = {1.0, 0.0, 0.0, 1.0};
      PetscReal   detJEx          = 1.0, scale, phi;
      PetscReal   centroidEx[2]   = {0.0, 0.0};
      PetscReal   normalEx[2]     = {0.0, 0.0};
      PetscReal   volEx           = 4.0;
      PetscInt    d, e, f, p;

      ierr = PetscRandomGetValueReal(r, &scale);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang, &phi);CHKERRQ(ierr);
      R[0] = cos(phi); R[1] = -sin(phi);
      R[2] = sin(phi); R[3] =  cos(phi);
      for (p = 0; p < 4; ++p) {
        for (d = 0; d < dim; ++d) {
          for (e = 0, rot[d] = 0.0; e < dim; ++e) {
            rot[d] += R[d*dim+e] * vertexCoords[p*dim+e];
          }
        }
        for (d = 0; d < dim; ++d) vertexCoords[p*dim+d] = rot[d];
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * centroidEx[e];
        }
      }
      for (d = 0; d < dim; ++d) centroidEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += R[d*dim+f] * JEx[f*dim+e];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += invJEx[d*dim+f] * R[e*dim+f];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          invJEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        ierr = PetscRandomGetValueReal(r, &trans[d]);CHKERRQ(ierr);
        for (p = 0; p < 4; ++p) {
          vertexCoords[p*dim+d] *= scale;
          vertexCoords[p*dim+d] += trans[d];
        }
        v0Ex[d] = vertexCoords[d];
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e]    *= scale;
          invJEx[d*dim+e] /= scale;
        }
        detJEx *= scale;
        centroidEx[d] *= scale;
        centroidEx[d] += trans[d];
        volEx *= scale;
      }
      ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
      ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
      if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
    }
    ierr = PetscRandomDestroy(&r);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang);CHKERRQ(ierr);
  }
  /* Move to 3D: Check reference geometry: determinant is scaled by reference volume (4.0) */
  dim = 3;
  {
    PetscScalar vertexCoords[12] = {-1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0};
    PetscReal v0Ex[3]            = {-1.0, -1.0, 0.0};
    PetscReal JEx[9]             = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal invJEx[9]          = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal detJEx             = 1.0;
    PetscReal centroidEx[3]      = {0.0, 0.0, 0.0};
    PetscReal normalEx[3]        = {0.0, 0.0, 1.0};
    PetscReal volEx              = 4.0;

    ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Check random quadrilaterals: scale, translate, then rotate */
  if (transform) {
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &r);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(r);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(r, 0.0, 10.0);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang, 0.0, 2*PETSC_PI);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang2, 0.0, PETSC_PI);CHKERRQ(ierr);
    for (t = 0; t < 100; ++t) {
      PetscScalar vertexCoords[12] = {-1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0}, trans[3];
      PetscReal   v0Ex[3]          = {-1.0, -1.0, 0.0};
      PetscReal   JEx[9]           = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, R[9], rot[3], rotM[9];
      PetscReal   invJEx[9]        = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
      PetscReal   detJEx           = 1.0, scale, phi, theta, psi = 0.0;
      PetscReal   centroidEx[3]    = {0.0, 0.0, 0.0};
      PetscReal   normalEx[3]      = {0.0, 0.0, 1.0};
      PetscReal   volEx            = 4.0;
      PetscInt    d, e, f, p;

      ierr = PetscRandomGetValueReal(r, &scale);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang, &phi);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang2, &theta);CHKERRQ(ierr);
      for (d = 0; d < dim; ++d) {
        ierr = PetscRandomGetValueReal(r, &trans[d]);CHKERRQ(ierr);
        for (p = 0; p < 4; ++p) {
          vertexCoords[p*dim+d] *= scale;
          vertexCoords[p*dim+d] += trans[d];
        }
        centroidEx[d] *= scale;
        centroidEx[d] += trans[d];
        for (e = 0; e < dim-1; ++e) {
          JEx[d*dim+e]    *= scale;
          invJEx[d*dim+e] /= scale;
        }
        if (d < dim-1) {
          detJEx *= scale;
          volEx  *= scale;
        }
      }
      R[0] = cos(theta)*cos(psi); R[1] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi); R[2] = sin(phi)*sin(psi) + cos(phi)*sin(theta)*cos(psi);
      R[3] = cos(theta)*sin(psi); R[4] = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi); R[5] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
      R[6] = -sin(theta);         R[7] = sin(phi)*cos(theta);                              R[8] = cos(phi)*cos(theta);
      for (p = 0; p < 4; ++p) {
        for (d = 0; d < dim; ++d) {
          for (e = 0, rot[d] = 0.0; e < dim; ++e) {
            rot[d] += R[d*dim+e] * vertexCoords[p*dim+e];
          }
        }
        for (d = 0; d < dim; ++d) vertexCoords[p*dim+d] = rot[d];
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * centroidEx[e];
        }
      }
      for (d = 0; d < dim; ++d) centroidEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * normalEx[e];
        }
      }
      for (d = 0; d < dim; ++d) normalEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        v0Ex[d] = vertexCoords[d];
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += R[d*dim+f] * JEx[f*dim+e];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += invJEx[d*dim+f] * R[e*dim+f];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          invJEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
      ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
      if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
    }
    ierr = PetscRandomDestroy(&r);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang2);CHKERRQ(ierr);
  }
  /* Cleanup */
  ierr = DMDestroy(&dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TestTetrahedron"
PetscErrorCode TestTetrahedron(MPI_Comm comm, PetscBool interpolate, PetscBool transform)
{
  DM             dm;
  PetscRandom    r, ang, ang2;
  PetscInt       dim, t;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  /* Create reference tetrahedron */
  dim  = 3;
  ierr = DMCreate(comm, &dm);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) dm, "tetrahedron");CHKERRQ(ierr);
  ierr = DMSetType(dm, DMPLEX);CHKERRQ(ierr);
  ierr = DMPlexSetDimension(dm, dim);CHKERRQ(ierr);
  {
    PetscInt    numPoints[2]        = {4, 1};
    PetscInt    coneSize[5]         = {4, 0, 0, 0, 0};
    PetscInt    cones[4]            = {1, 2, 3, 4};
    PetscInt    coneOrientations[4] = {0, 0, 0, 0};
    PetscScalar vertexCoords[12]    = {-1.0, -1.0, -1.0,  -1.0, 1.0, -1.0,  1.0, -1.0, -1.0,  -1.0, -1.0, 1.0};

    ierr = DMPlexCreateFromDAG(dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
    if (interpolate) {
      DM idm;

      ierr = DMPlexInterpolate(dm, &idm);CHKERRQ(ierr);
      ierr = PetscObjectSetName((PetscObject) idm, "tetrahedron");CHKERRQ(ierr);
      ierr = DMPlexCopyCoordinates(dm, idm);CHKERRQ(ierr);
      ierr = DMDestroy(&dm);CHKERRQ(ierr);
      dm   = idm;
    }
    ierr = DMSetFromOptions(dm);CHKERRQ(ierr);
  }
  /* Check reference geometry: determinant is scaled by reference volume (4/3) */
  {
    PetscReal v0Ex[3]       = {-1.0, -1.0, -1.0};
    PetscReal JEx[9]        = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal invJEx[9]     = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal detJEx        = 1.0;
    PetscReal centroidEx[3] = {-0.5, -0.5, -0.5};
    PetscReal normalEx[3]   = {0.0, 0.0, 0.0};
    PetscReal volEx         = 4.0/3.0;

    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Check random tetrahedra: rotate, scale, then translate */
  if (transform) {
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &r);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(r);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(r, 0.0, 10.0);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang, 0.0, 2*PETSC_PI);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang2, 0.0, PETSC_PI);CHKERRQ(ierr);
    for (t = 0; t < 100; ++t) {
      PetscScalar vertexCoords[12] = {-1.0, -1.0, -1.0,  -1.0, 1.0, -1.0,  1.0, -1.0, -1.0,  -1.0, -1.0, 1.0}, trans[3];
      PetscReal   v0Ex[3]          = {-1.0, -1.0, -1.0};
      PetscReal   JEx[9]           = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, R[9], rot[3], rotM[9];
      PetscReal   invJEx[9]        = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
      PetscReal   detJEx           = 1.0, scale, phi, theta, psi = 0.0;
      PetscReal   centroidEx[3]    = {-0.5, -0.5, -0.5};
      PetscReal   normalEx[3]      = {0.0, 0.0, 0.0};
      PetscReal   volEx            = 4.0/3.0;
      PetscInt    d, e, f, p;

      ierr = PetscRandomGetValueReal(r, &scale);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang, &phi);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang2, &theta);CHKERRQ(ierr);
      for (d = 0; d < dim; ++d) {
        ierr = PetscRandomGetValueReal(r, &trans[d]);CHKERRQ(ierr);
        for (p = 0; p < 4; ++p) {
          vertexCoords[p*dim+d] *= scale;
          vertexCoords[p*dim+d] += trans[d];
        }
        centroidEx[d] *= scale;
        centroidEx[d] += trans[d];
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e]    *= scale;
          invJEx[d*dim+e] /= scale;
        }
        detJEx *= scale;
        volEx  *= scale;
      }
      R[0] = cos(theta)*cos(psi); R[1] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi); R[2] = sin(phi)*sin(psi) + cos(phi)*sin(theta)*cos(psi);
      R[3] = cos(theta)*sin(psi); R[4] = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi); R[5] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
      R[6] = -sin(theta);         R[7] = sin(phi)*cos(theta);                              R[8] = cos(phi)*cos(theta);
      for (p = 0; p < 4; ++p) {
        for (d = 0; d < dim; ++d) {
          for (e = 0, rot[d] = 0.0; e < dim; ++e) {
            rot[d] += R[d*dim+e] * vertexCoords[p*dim+e];
          }
        }
        for (d = 0; d < dim; ++d) vertexCoords[p*dim+d] = rot[d];
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * centroidEx[e];
        }
      }
      for (d = 0; d < dim; ++d) centroidEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        v0Ex[d] = vertexCoords[d];
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += R[d*dim+f] * JEx[f*dim+e];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += invJEx[d*dim+f] * R[e*dim+f];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          invJEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
      ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
      if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
    }
    ierr = PetscRandomDestroy(&r);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang2);CHKERRQ(ierr);
  }
  /* Cleanup */
  ierr = DMDestroy(&dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TestHexahedron"
PetscErrorCode TestHexahedron(MPI_Comm comm, PetscBool interpolate, PetscBool transform)
{
  DM             dm;
  PetscRandom    r, ang, ang2;
  PetscInt       dim, t;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  /* Create reference hexahedron */
  dim  = 3;
  ierr = DMCreate(comm, &dm);CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) dm, "hexahedron");CHKERRQ(ierr);
  ierr = DMSetType(dm, DMPLEX);CHKERRQ(ierr);
  ierr = DMPlexSetDimension(dm, dim);CHKERRQ(ierr);
  {
    PetscInt    numPoints[2]        = {8, 1};
    PetscInt    coneSize[9]         = {8, 0, 0, 0, 0, 0, 0, 0, 0};
    PetscInt    cones[8]            = {1, 2, 3, 4, 5, 6, 7, 8};
    PetscInt    coneOrientations[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    PetscScalar vertexCoords[24]    = {-1.0, -1.0, -1.0,  -1.0,  1.0, -1.0,  1.0, 1.0, -1.0,   1.0, -1.0, -1.0,
                                       -1.0, -1.0,  1.0,   1.0, -1.0,  1.0,  1.0, 1.0,  1.0,  -1.0,  1.0,  1.0};

    ierr = DMPlexCreateFromDAG(dm, 1, numPoints, coneSize, cones, coneOrientations, vertexCoords);CHKERRQ(ierr);
    if (interpolate) {
      DM idm;

      ierr = DMPlexInterpolate(dm, &idm);CHKERRQ(ierr);
      ierr = PetscObjectSetName((PetscObject) idm, "hexahedron");CHKERRQ(ierr);
      ierr = DMPlexCopyCoordinates(dm, idm);CHKERRQ(ierr);
      ierr = DMDestroy(&dm);CHKERRQ(ierr);
      dm   = idm;
    }
    ierr = DMSetFromOptions(dm);CHKERRQ(ierr);
  }
  /* Check reference geometry: determinant is scaled by reference volume 8.0 */
  {
    PetscReal v0Ex[3]       = {-1.0, -1.0, -1.0};
    PetscReal JEx[9]        = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal invJEx[9]     = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    PetscReal detJEx        = 1.0;
    PetscReal centroidEx[3] = {0.0, 0.0, 0.0};
    PetscReal normalEx[3]   = {0.0, 0.0, 0.0};
    PetscReal volEx         = 8.0;

    ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
    if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
  }
  /* Check random hexahedra: rotate, scale, then translate */
  if (transform) {
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &r);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(r);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(r, 0.0, 10.0);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang, 0.0, 2*PETSC_PI);CHKERRQ(ierr);
    ierr = PetscRandomCreate(PETSC_COMM_SELF, &ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetFromOptions(ang2);CHKERRQ(ierr);
    ierr = PetscRandomSetInterval(ang2, 0.0, PETSC_PI);CHKERRQ(ierr);
    for (t = 0; t < 100; ++t) {
      PetscScalar vertexCoords[24] = {-1.0, -1.0, -1.0,  -1.0,  1.0, -1.0,  1.0, 1.0, -1.0,   1.0, -1.0, -1.0,
                                      -1.0, -1.0,  1.0,   1.0, -1.0,  1.0,  1.0, 1.0,  1.0,  -1.0,  1.0,  1.0}, trans[3];
      PetscReal   v0Ex[3]          = {-1.0, -1.0, -1.0};
      PetscReal   JEx[9]           = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, R[9], rot[3], rotM[9];
      PetscReal   invJEx[9]        = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
      PetscReal   detJEx           = 1.0, scale, phi, theta, psi = 0.0;
      PetscReal   centroidEx[3]    = {0.0, 0.0, 0.0};
      PetscReal   normalEx[3]      = {0.0, 0.0, 0.0};
      PetscReal   volEx            = 8.0;
      PetscInt    d, e, f, p;

      ierr = PetscRandomGetValueReal(r, &scale);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang, &phi);CHKERRQ(ierr);
      ierr = PetscRandomGetValueReal(ang2, &theta);CHKERRQ(ierr);
      for (d = 0; d < dim; ++d) {
        ierr = PetscRandomGetValueReal(r, &trans[d]);CHKERRQ(ierr);
        for (p = 0; p < 8; ++p) {
          vertexCoords[p*dim+d] *= scale;
          vertexCoords[p*dim+d] += trans[d];
        }
        centroidEx[d] *= scale;
        centroidEx[d] += trans[d];
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e]    *= scale;
          invJEx[d*dim+e] /= scale;
        }
        detJEx *= scale;
        volEx  *= scale;
      }
      R[0] = cos(theta)*cos(psi); R[1] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi); R[2] = sin(phi)*sin(psi) + cos(phi)*sin(theta)*cos(psi);
      R[3] = cos(theta)*sin(psi); R[4] = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi); R[5] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
      R[6] = -sin(theta);         R[7] = sin(phi)*cos(theta);                              R[8] = cos(phi)*cos(theta);
      for (p = 0; p < 8; ++p) {
        for (d = 0; d < dim; ++d) {
          for (e = 0, rot[d] = 0.0; e < dim; ++e) {
            rot[d] += R[d*dim+e] * vertexCoords[p*dim+e];
          }
        }
        for (d = 0; d < dim; ++d) vertexCoords[p*dim+d] = rot[d];
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0, rot[d] = 0.0; e < dim; ++e) {
          rot[d] += R[d*dim+e] * centroidEx[e];
        }
      }
      for (d = 0; d < dim; ++d) centroidEx[d] = rot[d];
      for (d = 0; d < dim; ++d) {
        v0Ex[d] = vertexCoords[d];
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += R[d*dim+f] * JEx[f*dim+e];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          JEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          for (f = 0, rotM[d*dim+e] = 0.0; f < dim; ++f) {
            rotM[d*dim+e] += invJEx[d*dim+f] * R[e*dim+f];
          }
        }
      }
      for (d = 0; d < dim; ++d) {
        for (e = 0; e < dim; ++e) {
          invJEx[d*dim+e] = rotM[d*dim+e];
        }
      }
      ierr = ChangeCoordinates(dm, dim, vertexCoords);CHKERRQ(ierr);
      ierr = CheckFEMGeometry(dm, 0, dim, v0Ex, JEx, invJEx, detJEx);CHKERRQ(ierr);
      if (interpolate) {ierr = CheckFVMGeometry(dm, 0, dim, centroidEx, normalEx, volEx);CHKERRQ(ierr);}
    }
    ierr = PetscRandomDestroy(&r);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang);CHKERRQ(ierr);
    ierr = PetscRandomDestroy(&ang2);CHKERRQ(ierr);
  }
  /* Cleanup */
  ierr = DMDestroy(&dm);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc, char **argv)
{
  PetscBool      transform = PETSC_FALSE;
  PetscErrorCode ierr;

  ierr = PetscInitialize(&argc, &argv, NULL, help);CHKERRQ(ierr);
  ierr = PetscOptionsGetBool(NULL, "-transform", &transform, NULL);CHKERRQ(ierr);
  if (transform) {ierr = PetscPrintf(PETSC_COMM_WORLD, "Using random transforms");CHKERRQ(ierr);}
  ierr = TestTriangle(PETSC_COMM_SELF, PETSC_FALSE, transform);CHKERRQ(ierr);
  ierr = TestTriangle(PETSC_COMM_SELF, PETSC_TRUE,  transform);CHKERRQ(ierr);
  ierr = TestQuadrilateral(PETSC_COMM_SELF, PETSC_FALSE, transform);CHKERRQ(ierr);
  ierr = TestQuadrilateral(PETSC_COMM_SELF, PETSC_TRUE,  transform);CHKERRQ(ierr);
  ierr = TestTetrahedron(PETSC_COMM_SELF, PETSC_FALSE, transform);CHKERRQ(ierr);
  ierr = TestTetrahedron(PETSC_COMM_SELF, PETSC_TRUE,  transform);CHKERRQ(ierr);
  ierr = TestHexahedron(PETSC_COMM_SELF, PETSC_FALSE, transform);CHKERRQ(ierr);
  ierr = TestHexahedron(PETSC_COMM_SELF, PETSC_TRUE, transform);CHKERRQ(ierr);
  ierr = PetscFinalize();
  return 0;
}
