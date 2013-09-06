static char help[] = "test2.c: test of the new generation ExodusII readers: loads a meshand creates sections\n\n";

#include <petscsys.h>
#include <petscdmmesh.hh>

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char ** argv)
{
  DM             dmBody,dmFS;
  PetscBool      inflag;
  char           infilename[PETSC_MAX_PATH_LEN+1];
  PetscErrorCode ierr;
  SectionReal    sec1;
  PetscInt       dof=1;
  int            rank;
  PetscInt       cell,face,conesize,my_num_cells,my_num_faces,my_num_vertices;
  PetscInt       i,c,k;
  PetscReal      *sr_array;


  ierr = PetscInitialize(&argc,&argv,(char*)0,help);CHKERRQ(ierr);
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);

  ierr = PetscOptionsGetString(NULL,"-i",infilename,PETSC_MAX_PATH_LEN,&inflag);CHKERRQ(ierr);
  if (!inflag) {
    ierr = PetscPrintf(PETSC_COMM_WORLD,"No file name given\n");CHKERRQ(ierr);
    PetscFinalize();
    return 0;
  }

  /*
    Reads a mesh
  */
  ierr = DMMeshCreateExodusNG(PETSC_COMM_WORLD,infilename,&dmBody,&dmFS);CHKERRQ(ierr);

  ierr = PetscOptionsGetInt(NULL,"-dof",&dof,NULL);CHKERRQ(ierr);
  ierr = DMMeshSetMaxDof(dmBody,dof);CHKERRQ(ierr);
  ierr = DMMeshSetMaxDof(dmFS,dof);CHKERRQ(ierr);

  /*
    Testing Vertex Sections
  */
  ierr = DMMeshGetVertexSectionReal(dmBody,"sec1",dof,&sec1);CHKERRQ(ierr);

  {
    ALE::Obj<PETSC_MESH_TYPE> meshBody,meshFS;
    ierr = DMMeshGetMesh(dmBody,meshBody);CHKERRQ(ierr);
    ierr = DMMeshGetMesh(dmFS,meshFS);CHKERRQ(ierr);

    my_num_cells    = meshBody->heightStratum(0)->size();
    my_num_vertices = meshBody->depthStratum(0)->size();
    my_num_faces    = meshFS->heightStratum(0)->size();
    for (cell = 0; cell < my_num_cells; cell++) {
      ierr = DMMeshGetConeSize(dmBody,cell,&conesize);CHKERRQ(ierr);
      ierr = PetscMalloc(conesize*dof*sizeof(PetscReal),&sr_array);CHKERRQ(ierr);
      ierr = SectionRealRestrictClosureWithArray(sec1,dmBody,cell,conesize*dof,sr_array);CHKERRQ(ierr);
      for (k = 0,i = 0; i < conesize; i++) {
        for (c = 0; c < dof; c++,k++) {
          sr_array[k] = 1. + 100*c;
        }
      }
      ierr = SectionRealUpdateClosure(sec1,dmBody,cell,sr_array,ADD_VALUES);CHKERRQ(ierr);
      ierr = PetscFree(sr_array);CHKERRQ(ierr);
    }
    ierr = SectionRealComplete(sec1);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"sec1 as a SectionReal\n");CHKERRQ(ierr);
    ierr = SectionRealView(sec1,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

    ierr = SectionRealSet(sec1,0.0);CHKERRQ(ierr);
    for (face = 0; face < my_num_faces; face++) {
      ierr = DMMeshGetConeSize(dmFS,face,&conesize);CHKERRQ(ierr);
      ierr = PetscMalloc(conesize*dof*sizeof(PetscReal),&sr_array);CHKERRQ(ierr);
      ierr = SectionRealRestrictClosureWithArray(sec1,dmFS,face,conesize*dof,sr_array);CHKERRQ(ierr);
      for (k = 0,i = 0; i < conesize; i++) {
        for (c = 0; c < dof; c++,k++) {
          sr_array[k] = 1. + 100*c;
        }
      }
      ierr = SectionRealUpdateClosure(sec1,dmFS,face,sr_array,ADD_VALUES);CHKERRQ(ierr);
      ierr = PetscFree(sr_array);CHKERRQ(ierr);
    }
    ierr = SectionRealComplete(sec1);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"sec1 as a SectionReal\n");CHKERRQ(ierr);
    ierr = SectionRealView(sec1,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  }

  ierr = SectionRealDestroy(&sec1);CHKERRQ(ierr);
  ierr = DMDestroy(&dmBody);CHKERRQ(ierr);
  ierr = DMDestroy(&dmFS);CHKERRQ(ierr);

  ierr = PetscFinalize();
  return 0;
}

