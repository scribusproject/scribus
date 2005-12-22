pluginsdir = $(libdir)/scribus/plugins

# Build scripter2 code into a convenience lib that gets linked
# into libscriptplugin.so if enabled.
noinst_LTLIBRARIES = libscripter2.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
libscripter2_la_METASOURCES = AUTO

KDE_OPTIONS = qtonly

libscripter2_la_SOURCES = \
	scripter2_base.cpp \
	scripter2_qttype_qstring.cpp \
	scripter2_qtclass_qobject.cpp \
	scripter2_qtclass_qapplication.cpp \
	scripter2_qtclass_qwidget.cpp \
	scripter2_qtclass_qframe.cpp \
	scripter2_qtclass_qscrollview.cpp \
	scripter2_qtclass_qmainwindow.cpp \
	scripter2_scribusstructs.cpp \
	scripter2_scribus_scribusmainwin.cpp \
	scripter2_scribus_scribusqapp.cpp \
	scripter2_scribus_pageitem.cpp \
	scripter2_scribus_scribuswin.cpp \
	scripter2_scribus_scribusdoc.cpp \
	scripter2_scribus_scribusview.cpp \
	scripter2_styles.cpp

EXTRA_DIST = \
	scripter2_base.cpp \
	scripter2_qtclass_qapplication.cpp \
	scripter2_qtclass_qframe.cpp \
	scripter2_qtclass_qmainwindow.cpp \
	scripter2_qtclass_qobject.cpp \
	scripter2_qtclass_qscrollview.cpp \
	scripter2_qtclass_qwidget.cpp \
	scripter2_qttype_qstring.cpp \
	scripter2_scribus_pageitem.cpp \
	Scripter2_scribus_scribusdoc.cpp \
	scripter2_scribus_scribusmainwin.cpp \
	scripter2_scribus_scribusqapp.cpp \
	scripter2_scribus_scribusview.cpp \
	scripter2_scribus_scribuswin.cpp \
	scripter2_scribusstructs.cpp \
	scripter2_styles.cpp

AM_LDFLAGS = $(PYTHONLIB) $(LIBPYTHON) $(MODULE_LINK_FLAGS) $(BOOST_PYTHON_LDFLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus $(PYTHONINC) \
    -I$(top_builddir)/scribus/plugins/scriptplugin \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    $(GCC4_VISIBILITY_FLAGS)

KDE_CXXFLAGS = \
    $(BOOST_PYTHON_CXXFLAGS)
