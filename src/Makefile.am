lib_LTLIBRARIES = libinfluxdb.la

include_HEADERS = client.h cluster_admin.h database.h database_user.h influxdb.h query.h series.h server.h utils.h

MAINTAINERCLEANFILES = Makefile.in

libinfluxdb_la_SOURCES = client.c cluster_admin.c database.c database_user.c query.c series.c server.c utils.c
libinfluxdb_la_LDFLAGS = -lcurl -ljson-c

includedir = $(prefix)/include/influxdb
