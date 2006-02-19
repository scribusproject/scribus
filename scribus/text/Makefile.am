noinst_LTLIBRARIES=libtext.la

libtext_la_SOURCES=storytext.cpp 

EXTRA_DIST = storytext.h


INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

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
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    -DPLUGINDATA=\"$(datadir)/scribus/plugins/\" \
    $(GCC4_VISIBILITY_FLAGS)

