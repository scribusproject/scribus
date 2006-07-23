noinst_LTLIBRARIES=libfonts.la

libfonts_la_SOURCES=scface.cpp ftface.cpp scface_ttf.cpp scface_ps.cpp scfontmetrics.cpp 

EXTRA_DIST = scface.h ftface.h scface_ttf.h scface_ps.h scfontmetrics.h 


INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/doc/scribus-$(VERSION)/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    -DPLUGINDATA=\"$(datadir)/scribus/plugins/\" \
    $(GCC4_VISIBILITY_FLAGS)

