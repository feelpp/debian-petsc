dnl PETSc checks by Adam Powell last modified 2002-10-15
dnl Copyright 2002 Adam Powell, redistributable under the terms of the GNU
dnl General Public License version 2 or later.

dnl Note: to use these, one must put:
dnl include $(PETSC_DIR)/conf/base
dnl very high in the Makefile.am of the directory where the links are made to
dnl PETSc libraries.  This will then allow the use of variables like
dnl $(PETSC_DM_LIB) in _LIBADD fields.
dnl See the illuminator package for a good example of this.

AC_DEFUN([PETSC_CHECKS],[
	dnl PETSc checks, with defaults for PETSC_DIR, PETSC_ARCH and BOPT.
	dnl Note: this trusts PETSc to provide valid flags and libs for
	dnl BLAS/LAPACK and MPI.  I don't see an easy way to verify this.

	AC_CHECK_FILE($PETSC_DIR/bin/petscarch, , [
		AC_CHECK_FILE(/usr/lib/petsc/bin/petscarch,
			PETSC_DIR=/usr/lib/petsc,
			AC_MSG_ERROR(Cannot find petscarch, please set PETSC_DIR for your PETSc install.))])
	AC_SUBST(PETSC_DIR)

	AC_CHECK_PROG(PETSC_ARCH, petscarch, `petscarch`, ,
		$PETSC_DIR/bin, /usr/bin)
	AC_SUBST(PETSC_ARCH)

	AC_CHECK_FILE($PETSC_DIR/$PETSC_ARCH/lib/libpetsc.a, ,
		AC_MSG_ERROR(Library libpetsc.a does not seem to be installed.))

	dnl And now, the lib defs
	MATH_BLASLAPACK_CHECKS

	PETSC_X11_LIBS="-L/usr/X11R6/lib -lX11"
	dnl This is wrong, or rather, only right for one MPI configuration.
	dnl Will have to change it so the PETSc package sets it properly.
	PETSC_MPI_LIBS="-lmpi"

	dnl This kind of mirrors conf/variables, though only for .m4
	dnl files; makefiles should include "$(PETSC_DIR)/conf/base"
	dnl and use e.g. ${PETSC_DM_LIB} etc.
	PETSC_LIBDIR="$PETSC_DIR/$PETSC_ARCH/lib"
	PETSC_EXTERNAL_LIBS="$PETSC_MPI_LIBS $PETSC_X11_LIBS $BLASLAPACK_LIBS -lhdf5 -lstdc++"

	dnl Basic check
	aLIBS="$LIBS"
	LIBS="$aLIBS -L$PETSC_LIBDIR $PETSC_EXTERNAL_LIBS"
	AC_CHECK_LIB(petsc, PetscPrintf, ,
		AC_MSG_ERROR("PETSc libraries not found"))
	LIBS="$aLIBS"

	dnl And lots of definitions
	PETSC_SYS_LIB_BASIC="-lpetsc $PETSC_EXTERNAL_LIBS"
	PETSC_VEC_LIB_BASIC="-lpetscvec $PETSC_SYS_LIB_BASIC"
	PETSC_MAT_LIB_BASIC="-lpetscmat $PETSC_VEC_LIB_BASIC"
	PETSC_DM_LIB_BASIC="-lpetscdm $PETSC_MAT_LIB_BASIC"
	PETSC_KSP_LIB_BASIC="-lpetscksp $PETSC_DM_LIB_BASIC"
	PETSC_SNES_LIB_BASIC="-lpetscsnes $PETSC_KSP_LIB_BASIC"
	PETSC_TS_LIB_BASIC="-lpetscts $PETSC_SNES_LIB_BASIC"

	dnl These are the ones to use in other m4 tests
	PETSC_SYS_LIB="-L$PETSC_LIBDIR $PETSC_SYS_LIB_BASIC"
	PETSC_VEC_LIB="-L$PETSC_LIBDIR $PETSC_VEC_LIB_BASIC"
	PETSC_MAT_LIB="-L$PETSC_LIBDIR $PETSC_MAT_LIB_BASIC"
	PETSC_DM_LIB="-L$PETSC_LIBDIR $PETSC_DM_LIB_BASIC"
	PETSC_KSP_LIB="-L$PETSC_LIBDIR $PETSC_KSP_LIB_BASIC"
	PETSC_SNES_LIB="-L$PETSC_LIBDIR $PETSC_SNES_LIB_BASIC"
	PETSC_TS_LIB="-L$PETSC_LIBDIR $PETSC_TS_LIB_BASIC"
])
