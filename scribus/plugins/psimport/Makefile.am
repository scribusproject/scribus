pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =

plugins_LTLIBRARIES = libimportps.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libimportps_la_LDFLAGS = -version-info 0:0:0
libimportps_la_METASOURCES = AUTO
libimportps_la_SOURCES = importps.cpp

EXTRA_DIST = import.prolog importps.cpp importps.h

install-data-local:
	$(mkinstalldirs) $(libdir)/scribus/
	$(INSTALL_DATA) $(srcdir)/import.prolog $(libdir)/scribus/import.prolog

uninstall-local:
	-rm -f $(libdir)/scribus/import.prolog

KDE_OPTIONS = qtonly

if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)
