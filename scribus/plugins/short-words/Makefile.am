pluginsdir = $(prefix)/lib/scribus/plugins
AM_CPPFLAGS = -I$(prefix)/include/scribus \
	-s $(LIBFREETYPE_LIBS) \
	-I$(top_builddir)/scribus \
	-DDOCDIR=\"$(datadir)/scribus/doc/\" \
	-DICONDIR=\"$(datadir)/scribus/icons/\" \
	-DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
	-DLIBDIR=\"$(libdir)/scribus/\" \
	-DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

SUBDIRS =

plugins_LTLIBRARIES = libscribusshortwords.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribusshortwords_la_LDFLAGS = -version-info 0:0:0
libscribusshortwords_la_METASOURCES = AUTO
libscribusshortwords_la_SOURCES = vlnadialog.cpp parse.cpp shortwords.cpp configuration.cpp version.h

EXTRA_DIST = parse.h vlnadialog.h shortwords.h version.h configuration config.h scribus-short-words.rc

KDE_OPTIONS = qtonly
AM_LDFLAGS =	-s $(LIBFREETYPE_LIBS)


install-data-local:
	$(mkinstalldirs) $(prefix)/lib/scribus/plugins/
	$(INSTALL_DATA) $(srcdir)/scribus-short-words.rc $(prefix)/lib/scribus/plugins/scribus-short-words.rc

uninstall-local:
	-rm -f $(prefix)/lib/scribus/plugins/scribus-short-words.rc
