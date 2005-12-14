SUBDIRS = samples scripts
pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libscriptplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
libscriptplugin_la_LDFLAGS = -version-info 0:0:0
libscriptplugin_la_METASOURCES = AUTO
libscriptplugin_la_SOURCES = scriptercore.cpp valuedialog.cpp cmdutil.cpp cmddialog.cpp cmddoc.cpp cmdpage.cpp cmdobj.cpp cmdgetprop.cpp cmdsetprop.cpp cmdtext.cpp cmdmani.cpp cmdcolor.cpp cmdmisc.cpp objprinter.cpp objpdffile.cpp guiapp.cpp svgimport.cpp pconsole.cpp scriptplugin.cpp objimageexport.cpp cmdgetsetprop.cpp runscriptdialog.cpp scripterprefsgui.cpp scripter2_base.cpp
EXTRA_DIST = valuedialog.h cmdvar.h cmdutil.cpp cmdutil.h cmddialog.cpp cmddialog.h cmddoc.cpp cmddoc.h cmdpage.cpp cmdpage.h cmdobj.cpp cmdobj.h cmdgetprop.cpp cmdgetprop.h cmdsetprop.cpp cmdsetprop.h cmdtext.cpp cmdtext.h cmdmani.cpp cmdmani.h cmdcolor.cpp cmdcolor.h cmdmisc.cpp cmdmisc.h objprinter.cpp objprinter.h objpdffile.cpp objpdffile.h guiapp.cpp guiapp.h svgimport.cpp svgimport.h scriptplugin.cpp scriptplugin.h pconsole.cpp pconsole.h objimageexport.h cmdgetsetprop.h cmdgetsetprop.cpp scriptercore.h runscriptdialog.h scripterprefsgui.h scripter2_base.h
KDE_OPTIONS = qtonly
AM_LDFLAGS = $(PYTHONLIB) $(LIBPYTHON) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus $(PYTHONINC) \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    $(GCC4_VISIBILITY_FLAGS)
