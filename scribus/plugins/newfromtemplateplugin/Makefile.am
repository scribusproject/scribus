pluginsdir = $(libdir)/scribus/plugins

SUBDIRS =  

plugins_LTLIBRARIES = libnewfromtemplateplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libnewfromtemplateplugin_la_LDFLAGS = -version-info 0:0:0
libnewfromtemplateplugin_la_METASOURCES = AUTO
libnewfromtemplateplugin_la_SOURCES = nftemplate.cpp nftdialog.cpp nftrcreader.cpp nftsettings.cpp nfttemplate.cpp

EXTRA_DIST = nftemplate.cpp nftemplate.h nftdialog.cpp nftdialog.h nftrcreader.cpp nftrcreader.h nftsettings.cpp nftsettings.h nfttemplate.cpp nfttemplate.h

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
    $(GCC4_VISIBILITY_FLAGS)
