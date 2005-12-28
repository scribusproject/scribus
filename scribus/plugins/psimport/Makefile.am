pluginsdir = $(libdir)/scribus/plugins

SUBDIRS =

plugins_LTLIBRARIES = libimportps.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libimportps_la_LDFLAGS = -version-info 0:0:0
libimportps_la_METASOURCES = AUTO
libimportps_la_SOURCES = importps.cpp importpsplugin.cpp

EXTRA_DIST = import.prolog importps.h importpsplugin.h

install-data-local:
	$(mkinstalldirs) $(libdir)/scribus/
	$(INSTALL_DATA) $(srcdir)/import.prolog $(libdir)/scribus/import.prolog

uninstall-local:
	-rm -f $(libdir)/scribus/import.prolog

KDE_OPTIONS = qtonly
AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS) 

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
