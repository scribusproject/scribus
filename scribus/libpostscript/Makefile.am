libdir = $(prefix)/lib/scribus/libs
AM_CPPFLAGS = -I$(top_builddir)/scribus
lib_LTLIBRARIES = libpostscript.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpostscript_la_LDFLAGS = -version-info 0:0:0

libpostscript_la_METASOURCES = AUTO

libpostscript_la_SOURCES = pslib.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = pslib.cpp pslib.h 

install-data-local:
	$(mkinstalldirs) $(prefix)/include/scribus/libpostscript/
	$(INSTALL_DATA) $(srcdir)/pslib.h $(prefix)/include/scribus/libpostscript/pslib.h

uninstall-local:
	-rm -f $(prefix)/include/scribus/libpostscript/pslib.h

AM_LDFLAGS =  -s $(LIBFREETYPE_LIBS) 
