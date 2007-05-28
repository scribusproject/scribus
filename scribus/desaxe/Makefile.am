noinst_LTLIBRARIES=libdesaxe.la
METASOURCES = AUTO
libdesaxe_la_SOURCES=digester.cpp digester_parse.cpp saxXML.cpp saxiohelper.cpp saxfilter.cpp uniqueid.cpp

EXTRA_DIST = actions.h automata.h base_actions.h control_actions.h desaxe_conf.h digester.h prog_actions.h saxXML.h saxhandler.h saxio.h simple_actions.h saxiohelper.h saxfilter.h uniqueid.h


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

