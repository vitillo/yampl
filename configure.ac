#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([ATLAS IPC Library], [1.0], [ravitillo@lbl.gov])
AC_CONFIG_SRCDIR([include/YAMPLSocketFactory.h])
AC_CONFIG_HEADERS([config/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([rt], [clock_gettime])
AC_CHECK_LIB([uuid], [uuid_unparse])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h stdlib.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ftruncate gettimeofday memset mkfifo munmap strcasecmp strdup strerror])

AC_CONFIG_FILES([Makefile yampl.pc])
#AC_CONFIG_SUBDIRS([zeromq])
AC_OUTPUT
