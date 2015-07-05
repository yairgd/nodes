#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([algo trading system], [0.1], [BUG-REPORT-ADDRESS])
#AC_CONFIG_SRCDIR([python/src])
AC_CONFIG_HEADERS([config.h])


#AC_SUBST(MY_TEST_VAR)
#MY_TEST_VAR="this will add MY_TEST_VAR to Makefile"

AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_CXX
AM_PROG_CC_C_O
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([memory.h stdlib.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADER_STDBOOL
AC_CHECK_FUNCS([memset])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([libintl.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([termios.h])



# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_ALLOCA

AC_OUTPUT
AC_PROG_RANLIB


AC_C_INLINE

# Enable debug
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
                    [use debug compiler flags and macros (default is no)])],
    [enable_debug=$enableval],
    [enable_debug=no])


if test "x$enable_debug" = xyes; then
   # AC_MSG_ERROR("comparison tool reorg tests but comparison tool was not specified")	
   if test "x$GCC" = xyes; then
        CFLAGS="-g -O0 -DDEBUG"
   fi    
   if test "x$GXX" = xyes; then
        CXXFLAGS="-g -O0 -DDEBUG"
   fi
fi

# Enable debug
AC_ARG_ENABLE([debug-parser],
    [AS_HELP_STRING([--enable-debug-parser],
                    [use to print parser results (default is no)])],
    [enable_debug_parser=$enableval],
    [enable_debug_parser=no])

if test "x$enable_debug_parser" = xyes; then
   # AC_MSG_ERROR("comparison tool reorg tests but comparison tool was not specified")	
   if test "x$GCC" = xyes; then
        CFLAGS="${CFLAGS} -DDEBUG_PARSER"
   fi    
   if test "x$GXX" = xyes; then
        CXXFLAGS="${CXXFLAGS} -DDEBUG_PARSER"
   fi
fi


AC_ARG_ENABLE([unit-test],
	      [  --enable-unit-test    compile test files],
[case "${enableval}" in
  yes) unit_test=true ;;
  no)  unit_test=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-unit-test]) ;;
esac],[unit_test=false])
AM_CONDITIONAL([UNIT_TEST], [test x$unit_test = xtrue])


LT_INIT
#AC_ENABLE_SHARED
#AC_DISABLE_STATIC
#AC_PROG_LIBTOOL(libtool)

AM_INIT_AUTOMAKE(algotrade, 0.1)
AC_OUTPUT(Makefile src/Makefile test/Makefile python/Makefile)


