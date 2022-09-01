AM_CPPFLAGS=\
	$(PPARAM_CFLAGS) \
	$(PLOGGER_CFLAGS)\
	-I../include

ipcincludedir = $(includedir)/pvm/ipc
ipcinclude_HEADERS=\
		../include/plogger.hpp \
		../include/socket-types.hpp \
		../include/socket-tools.hpp \
		../include/socket.hpp \
		../include/socket-client.hpp \
		../include/socket-server.hpp

lib_LTLIBRARIES= libpipc.la
libpipc_la_SOURCES=\
		plogger.cpp \
		socket-types.cpp\
		socket-tools.cpp\
		socket.cpp\
		socket-server.cpp\
		socket-client.cpp

libpipc_la_LDFLAGS= -version-info $(LIBPIPC_SO_VERSION)
libpipc_la_LIBADD=\
		$(PPARAM_LIBS) \
		$(PLOGGER_LIBS)
