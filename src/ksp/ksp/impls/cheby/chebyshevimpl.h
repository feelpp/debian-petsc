/*
    Private data structure for Chebyshev Iteration
*/

#if !defined(__CHEBY)
#define __CHEBY

typedef struct {
  PetscReal emin,emax;    /* store user provided estimates of extreme eigenvalues */
  KSP       kspest;       /* KSP used to estimate eigenvalues */
  PC        pcnone;       /* Dummy PC to drop in so PCSetFromOptions doesn't get called extra times */
  PetscReal tform[4];     /* transform from Krylov estimates to Chebyshev bounds */
  PetscBool estimate_current;
  PetscBool hybrid;       /* flag for using Hybrid Chebyshev */
  PetscInt  chebysteps;   /* number of Chebyshev steps in Hybrid Chebyshev */
  PetscInt  eststeps;     /* number of adaptive/est steps in Hybrid Chebyshev */
  PetscInt  its;          /* total hybrid iterations, used to determine when toGMRES step in hybrid impl */
  PetscBool purification; /* Hybrid method uses the GMRES/adaptive/est steps to imporve the approximate solution */
  PetscRandom random;
} KSP_Chebyshev;

#endif
