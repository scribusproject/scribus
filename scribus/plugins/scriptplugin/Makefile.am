SUBDIRS = samples scripts
pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libscriptplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
libscriptplugin_la_LDFLAGS = -version-info 0:0:0
libscriptplugin_la_METASOURCES = AUTO

libscriptplugin_la_SOURCES = \
	cmdcolor.cpp \
	cmddialog.cpp \
	cmddoc.cpp \
	cmdgetprop.cpp \
	cmdgetsetprop.cpp \
	cmdmani.cpp \
	cmdmisc.cpp \
	cmdobj.cpp \
	cmdpage.cpp \
	cmdsetprop.cpp \
	cmdtext.cpp \
	cmdutil.cpp \
	guiapp.cpp \
	objimageexport.cpp \
	objpdffile.cpp \
	objprinter.cpp \
	pconsole.cpp \
	runscriptdialog.cpp \
	scripter2_base.cpp \
	scripter2_qttype_qstring.cpp \
	scripter2_scribusstructs.cpp \
	scripter2_styles.cpp \
	scriptercore.cpp \
	scripterprefsgui.cpp \
	scriptplugin.cpp \
	svgimport.cpp \
	valuedialog.cpp

EXTRA_DIST = \
	cmdcolor.h \
	cmddialog.h \
	cmddoc.h \
	cmdgetprop.h \
	cmdgetsetprop.h \
	cmdmani.h \
	cmdmisc.h \
	cmdobj.h \
	cmdpage.h \
	cmdsetprop.h \
	cmdtext.h \
	cmdutil.h \
	cmdvar.h \
	guiapp.h \
	objimageexport.h \
	objpdffile.h \
	objprinter.h \
	pconsole.h \
	runscriptdialog.h \
	scripter2_base.h \
	scripter2_qttypes.h \
	scripter2_scribusstructs.h \
	scriptercore.h \
	scripterprefsgui.h \
	scriptplugin.h \
	svgimport.h \
	valuedialog.h

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
