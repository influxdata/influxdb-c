#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([influxdb-c], [1.0], [influxdb@googlegroups.com])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([src/config.h])

# Automake.
AM_INIT_AUTOMAKE([1.11.1 dist-bzip2 no-dist-gzip foreign
                  color-tests parallel-tests
                  nostdinc silent-rules -Wall])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([curl], [libcurl],,
    [AC_MSG_ERROR([No pkg-config for libcurl])])
AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_LIBS)

AC_CHECK_LIB([curl], [curl_easy_init],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_escape],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_setopt],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_cleanup],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_free],,[AC_MSG_ERROR([libcurl required!])])

AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_LIBS)


JSON_LIBS=
PKG_CHECK_MODULES(JSON, json-c,, [AC_MSG_WARN("json-c not found")])

if test "$JSON_LIBS" == ""; then
PKG_CHECK_MODULES(JSON, json,
		  [AC_DEFINE([HAVE_JSON_0],[1],[Use JSON0])],
		  [AC_MSG_ERROR("json0 missing")])
fi

AC_SUBST(JSON_CFLAGS)
AC_SUBST(JSON_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdio.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([calloc strchr strcpy strncat sprintf])

AM_PROG_AR
LT_INIT

AC_OUTPUT( Makefile src/Makefile tests/Makefile )
