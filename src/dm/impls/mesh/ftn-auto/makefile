
#requiresdefine   'PETSC_HAVE_FORTRAN'
ALL: lib
CPPFLAGS =
CFLAGS   =
FFLAGS   =
SOURCEC  = meshf.c meshexodusf.c sectionf.c meshcreatef.c
OBJSC    = meshf.o meshexodusf.o sectionf.o meshcreatef.o
SOURCEF  =
SOURCEH  = 
DIRS     =
LIBBASE  = libpetscdm
LOCDIR   = src/dm/impls/mesh/ftn-auto/
include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules
include ${PETSC_DIR}/conf/test
