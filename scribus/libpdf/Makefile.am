pluginsdir = $(libdir)/scribus/libs
plugins_LTLIBRARIES = libpdf.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)


libpdf_la_LDFLAGS = -version-info 0:0:0

libpdf_la_METASOURCES = AUTO

libpdf_la_SOURCES = pdflib.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = pdflib.cpp pdflib.h 

install-data-local:
	$(mkinstalldirs) $(prefix)/include/scribus/libpdf/
	$(INSTALL_DATA) $(srcdir)/pdflib.h $(prefix)/include/scribus/libpdf/pdflib.h

uninstall-local:
	-rm -f $(prefix)/include/scribus/libpdf/pdflib.h

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
