if HAVE_SCRIPTER2
  scripter2dir = scripter2
  scripter2lib = -L$(top_builddir)/scribus/plugins/scriptplugin/scripter2 -lscripter2
endif
SUBDIRS = samples scripts $(scripter2dir)
pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libscriptplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
libscriptplugin_la_LDFLAGS = -version-info 0:0:0 $(scripter2lib)
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
	scriptercore.h \
	scripterprefsgui.h \
	scriptplugin.h \
	svgimport.h \
	valuedialog.h

KDE_OPTIONS = qtonly
AM_LDFLAGS = $(PYTHONLIB) $(LIBPYTHON) $(MODULE_LINK_FLAGS) $(BOOST_PYTHON_LDFLAGS)

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

# even though we don't use Boost::Python in the plain scripter, we need to
# enable support for it if we've found it, since cmdvar pulls in the boost
# header (which requires exceptions) instead of Python.h .
KDE_CXXFLAGS = \
    $(BOOST_PYTHON_CXXFLAGS)
