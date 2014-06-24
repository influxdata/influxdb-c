ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src tests

MAINTAINERCLEANFILES = aclocal.m4 Makefile.in configure depcomp install-sh missing

CLEANFILES = *~ \#* *.swp

test: check

doxygen.config: Makefile.in
	echo "PROJECT_NAME          = libinfluxdb" > doxygen.config
	echo "INPUT                 =  src/" >> doxygen.config
	echo "FILE_PATTERNS         =  *.h *.c" >> doxygen.config
	echo "OUTPUT_DIRECTORY      = docs/" >> doxygen.config
	echo "OPTIMIZE_OUTPUT_FOR_C = YES" >> doxygen.config
	echo "EXTRACT_ALL           = YES" >> doxygen.config

docs: doxygen.config
	doxygen doxygen.config
