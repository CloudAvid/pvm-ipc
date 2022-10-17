AM_CPPFLAGS=\
	$(PPARAM_CFLAGS) \
	$(PLOGGER_CFLAGS)\
	$(GNUTLS_CFLAGS)\
	-I../include

ipcincludedir = $(includedir)/pvm/ipc
ipcinclude_HEADERS=\
		../include/macros.h \
		../include/plogger.hpp \
		../include/socket-struct.hpp \
		../include/socket-tools.hpp \
		../include/socket.hpp \
		../include/socket-client.hpp \
		../include/socket-server.hpp \
		../include/socket-server-tls.hpp\
		../include/socket-client-tls.hpp


lib_LTLIBRARIES= libpipc.la
libpipc_la_SOURCES=\
		plogger.cpp \
		socket-struct.cpp\
		socket-tools.cpp\
		socket.cpp\
		socket-server.cpp\
		socket-client.cpp\
		socket-server-tls.cpp\
		socket-client-tls.cpp

libpipc_la_LDFLAGS= -version-info $(LIBPIPC_SO_VERSION)
libpipc_la_LIBADD=\
		$(PPARAM_LIBS) \
		$(PLOGGER_LIBS) \
		$(GNUTLS_LIBS)
