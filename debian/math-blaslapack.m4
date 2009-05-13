dnl Math and linear algebra checks by Adam Powell last modified 2002-10-16.
dnl Copyright 2002 Adam Powell, redistributable under the terms of the GNU
dnl General Public License version 2 or later.

AC_DEFUN([MATH_BLASLAPACK_CHECKS],[
	dnl ffm is free fast math by Joachim Wesner and Kazushige Goto for
	dnl Linux/Alpha.
	AC_CHECK_HEADERS(libffm.h)
	AC_CHECK_LIB(ffm, dsqrtiv,
		MATH_LIBS="$MATH_LIBS -lffm" MATH_INCLUDES="-DFFM")

	dnl Alphas need this sometimes
	AC_CHECKING([whether -mieee is needed to avoid SIGFPE on divide by zero])
	case $build/$CC in
		alpha*/gcc* )
			MIEEE_CFLAGS="-mieee"
			AC_MSG_RESULT([yes])
			;;
		* )
			MIEEE_CFLAGS=""
			AC_MSG_RESULT([not needed])
			;;
	esac
	AC_SUBST(MIEEE_CFLAGS)

	dnl Used to check for IRIX multi-processor BLAS, but result was
	dnl EXTREMELY slow (here just for historical purposes)
	dnl AC_CHECK_LIB(blas_mp, dgemm_, BLAS_LIBS="-mp -lblas_mp", [
	dnl   AC_CHECK_LIB(f77blas, dgemm_, BLAS_LIBS="-lf77blas -latlas", [
	dnl     LIBS="$aLIBS $MATH_LIBS $FLIBS"])])
	dnl For now, I don't see a need for mixed blas/lapackgf-3 or
	dnl blas-3gf/lapack, only blas-3gf/lapackgf-3 and blas/lapack are checked.
	AC_F77_LIBRARY_LDFLAGS
	aLIBS="$LIBS"
	LIBS="$aLIBS $FLIBS"
	AC_CHECK_LIB(blas-3gf, dgemm_, [
		HAVE_BLAS="yes" LIBS="-lblas-3gf $aLIBS $MATH_LIBS $FLIBS"
		AC_CHECK_LIB(lapackgf-3, dgetrf_,
			HAVE_LAPACK="yes"
			BLASLAPACK_LIBS="-lblas-3gf -llapackgf-3 $FLIBS",
			dnl This second check is for Debian woody on Alpha
			LIBS="$aLIBS -lblas-3gf /lib/libgcc_s.so.1 $MATH_LIBS $FLIBS"
			AC_CHECK_LIB(lapackgf-3, dgetri_,
				HAVE_LAPACK="yes"
				BLASLAPACK_LIBS="-lblas-3gf -llapackgf-3 /lib/libgcc_s.so.1 $MATH_LIBS $FLIBS",
				HAVE_LAPACK="no"
				BLASLAPACK_LIBS="-lblas-3gf $MATH_LIBS $FLIBS"))], [
		AC_CHECK_LIB(blas, dgemv_, [
			HAVE_BLAS="yes" LIBS="$aLIBS -lblas $MATH_LIBS $FLIBS"
			AC_CHECK_LIB(lapack, dgetrf_,
				HAVE_LAPACK="yes"
				BLASLAPACK_LIBS="-lblas -llapack $MATH_LIBS $FLIBS",
				BLASLAPACK_LIBS="-lblas $MATH_LIBS $FLIBS")])])
	LIBS="$aLIBS"

	dnl cpml is the Compaq Portable Math Library for Linux/Alpha, and often
	dnl comes with cxml which has superfast blas/lapack included, so this
	dnl overrides the above BLAS and LAPACK checks.
	aLIBS="$LIBS"
	LIBS="$aLIBS -lots"
	AC_CHECK_LIB(cpml, sqrt,
		MATH_LIBS="-lots -lcpml $MATH_LIBS"
		MATH_INCLUDES="$MATH_INCLUDES" LIBS="-lots -lcpml $aLIBS" [
			AC_CHECK_LIB(cxml, dgemm_,
				BLASLAPACK_LIBS="-lcxml $MATH_LIBS"
				HAVE_BLAS="yes" HAVE_LAPACK="yes")])
	LIBS="$aLIBS"

	AC_SUBST(MATH_LIBS)
	AC_SUBST(MATH_INCLUDES)
	AC_SUBST(BLASLAPACK_LIBS)
	AC_SUBST(HAVE_BLAS)
	AC_SUBST(HAVE_LAPACK)
])
