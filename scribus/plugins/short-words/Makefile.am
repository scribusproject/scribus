pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libscribusshortwords.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribusshortwords_la_LDFLAGS = -version-info 0:0:0
libscribusshortwords_la_METASOURCES = AUTO
libscribusshortwords_la_SOURCES = vlnadialog.cpp parse.cpp shortwords.cpp configuration.cpp version.h

EXTRA_DIST = parse.h vlnadialog.h shortwords.h version.h configuration.h scribus-short-words.rc

KDE_OPTIONS = qtonly

if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS = $(linkflag) $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    -DPLUGINDATA=\"$(datadir)/scribus/plugins/\"

install-data-local:
	$(mkinstalldirs) $(datadir)/scribus/plugins/
	$(INSTALL_DATA) $(srcdir)/scribus-short-words.rc $(datadir)/scribus/plugins/scribus-short-words.rc

uninstall-local:
	-rm -f $(datadir)/scribus/plugins/scribus-short-words.rc
