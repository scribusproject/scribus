pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libbarcodegenerator.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libbarcodegenerator_la_LDFLAGS = -version-info 0:0:0
libbarcodegenerator_la_METASOURCES = AUTO
libbarcodegenerator_la_SOURCES = barcode.cpp barcodegeneratorbase.cpp barcodegenerator.cpp

EXTRA_DIST = barcode.h barcodegeneratorbase.h barcodegenerator.h barcodegeneratorbase.ui barcode.ps

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)

install-data-local:
	$(mkinstalldirs) $(datadir)/scribus/plugins/
	$(INSTALL_DATA) $(srcdir)/barcode.ps $(datadir)/scribus/plugins/barcode.ps

uninstall-local:
	-rm -f $(datadir)/scribus/plugins/barcode.ps