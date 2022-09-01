AM_CPPFLAGS= $(PPARAM_CFLAGS) $(PLOGGER_CFLAGS) -I../include

noinst_PROGRAMS= server client
server_SOURCES= server-example.cpp
client_SOURCES= client-example.cpp

examples_ldadd= $(PPARAM_LIBS) -L$(top_srcdir)/src/.libs -lpthread ../src/libpipc.la
examples_ldflags= -Wl,--rpath -Wl,$(top_srcdir)/src/.libs

server_LDADD= $(examples_ldadd)
server_LDFLAGS= $(examples_ldflags)
client_LDADD= $(examples_ldadd)
client_LDFLAGS= $(examples_ldflags)
