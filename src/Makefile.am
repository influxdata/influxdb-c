lib_LTLIBRARIES = libinfluxdb.la

include_HEADERS = client.h cluster_admin.h config.h database.h database_user.h influxdb.h query.h series.h server.h shard.h utils.h

MAINTAINERCLEANFILES = Makefile.in

libinfluxdb_la_SOURCES = client.c cluster_admin.c database.c database_user.c query.c series.c server.c shard.c utils.c

LIBS = $(CURL_LIBS) $(JSON_LIBS)
AM_CPPFLAGS = $(CURL_CFLAGS) $(JSON_CFLAGS)

includedir = $(prefix)/include/influxdb
