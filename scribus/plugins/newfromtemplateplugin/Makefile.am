pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libnewfromtemplateplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libnewfromtemplateplugin_la_LDFLAGS = -version-info 0:0:0
libnewfromtemplateplugin_la_METASOURCES = AUTO
libnewfromtemplateplugin_la_SOURCES = nftemplate.cpp nftdialog.cpp nftrcreader.cpp nftsettings.cpp nfttemplate.cpp

EXTRA_DIST = nftemplate.cpp nftemplate.h nftdialog.cpp nftdialog.h nftrcreader.cpp nftrcreader.h nftsettings.cpp nftsettings.h nfttemplate.cpp nfttemplate.h

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)

