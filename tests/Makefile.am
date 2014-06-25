LDADD = $(top_builddir)/src/libinfluxdb.la

TESTS = server database cluster_admin database_user

check_PROGRAMS = server database cluster_admin database_user

server_SOURCES = server.c tests.c
database_SOURCES = database.c tests.c
cluster_admin_SOURCES = cluster_admin.c tests.c
database_user_SOURCES = database_user.c tests.c
