
#requiresdefine   'PETSC_HAVE_FORTRAN'
ALL: lib

CFLAGS   =
FFLAGS   =
SOURCEC  = axiscf.c lgcf.c axisf.c histsf.c lgf.c dscatterf.c
OBJSC    = axiscf.o lgcf.o axisf.o histsf.o lgf.o dscatterf.o
SOURCEF  =
SOURCEH  = 
DIRS     =
LIBBASE  = libpetscsys
LOCDIR   = src/sys/draw/utils/ftn-auto/
include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules
include ${PETSC_DIR}/conf/test
