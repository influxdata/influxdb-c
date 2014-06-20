#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([influxdb-c], [1.0], [influxdb@googlegroups.com])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/])

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

AC_CHECK_LIB([curl], [curl_easy_init],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_escape],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_setopt],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_cleanup],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_free],,[AC_MSG_ERROR([libcurl required!])])

PKG_CHECK_MODULES([json], [json-c],,
    [PKG_CHECK_MODULES([json], [json],,
        [PKG_CHECK_MODULES([json], [json0],,
            [AC_MSG_ERROR([No pkg-config for json-c (aka libjson0, libjson)])])])])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([calloc strchr strcpy strncat sprintf])

AM_PROG_AR
LT_INIT

AC_OUTPUT( Makefile src/Makefile )
