SUBDIRS = samples scripts
pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libscriptplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
libscriptplugin_la_LDFLAGS = -version-info 0:0:0
libscriptplugin_la_METASOURCES = AUTO
libscriptplugin_la_SOURCES = scriptercore.cpp valuedialog.cpp cmdutil.cpp cmddialog.cpp cmddoc.cpp cmdpage.cpp cmdobj.cpp cmdgetprop.cpp cmdsetprop.cpp cmdtext.cpp cmdmani.cpp cmdcolor.cpp cmdmisc.cpp objprinter.cpp objpdffile.cpp guiapp.cpp conswin.cpp pconsole.cpp scriptplugin.cpp objimageexport.cpp cmdgetsetprop.cpp
EXTRA_DIST = valuedialog.h cmdvar.h cmdutil.cpp cmdutil.h cmddialog.cpp cmddialog.h cmddoc.cpp cmddoc.h cmdpage.cpp cmdpage.h cmdobj.cpp cmdobj.h cmdgetprop.cpp cmdgetprop.h cmdsetprop.cpp cmdsetprop.h cmdtext.cpp cmdtext.h cmdmani.cpp cmdmani.h cmdcolor.cpp cmdcolor.h cmdmisc.cpp cmdmisc.h objprinter.cpp objprinter.h objpdffile.cpp objpdffile.h guiapp.cpp guiapp.h scriptplugin.cpp scriptplugin.h pconsole.cpp pconsole.h conswin.cpp conswin.h objimageexport.h cmdgetsetprop.h cmdgetsetprop.cpp scriptercore.h
KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) -Xlinker $(PYTHONLIB) $(LIBPYTHON)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus $(PYTHONINC) \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
