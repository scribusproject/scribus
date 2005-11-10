bin_PROGRAMS = scribus

renderer_subdirs = libart
renderer_libs = libart/libart.la

scribus_SOURCES = \
	about.cpp		\
	aboutpluginsbase.ui \
	aboutplugins.cpp	\
	actionmanager.cpp	\
	aligndistribute.cpp	\
	alignselect.cpp	\
	annota.cpp	\
	annot.cpp	\
	applytemplatedialog.cpp	\
	arrowchooser.cpp	\
	autoformbuttongroup.cpp	\
	autoform.cpp	\
	bookmwin.cpp	\
	bookpalette.cpp	\
	buttonicon.cpp	\
	charselect.cpp	\
	checkDocument.cpp	\
	cmsprefs.cpp	\
	cmykfw.cpp	\
	collect4output.cpp \
	colorchart.cpp	\
	colorm.cpp	\
	commonstrings.cpp \
	cpalette.cpp	\
	cupsoptions.cpp	\
	customfdialog.cpp	\
	dcolor.cpp	\
	deferredtask.cpp	\
	delpages.cpp	\
	docinfo.cpp	\
	docitemattrprefs.ui	\
	documentchecker.cpp	\
	documentinformation.cpp	\
	druck.cpp	\
	dynamictip.cpp	\
	edit1format.cpp	\
	editformats.cpp	\
	editor.cpp	\
	effectsdialog.cpp	\
	exif.cpp	\
	extimageprops.cpp	\
	fileloader.cpp	\
	filesearch.cpp	\
	fileunzip.cpp	\
	filewatcher.cpp	\
	fmitem.cpp	\
	fontcombo.cpp	\
	fontprefs.cpp	\
	fontreplacedialog.cpp	\
	fparser.cpp	\
	fpointarray.cpp	\
	frameedit.cpp	\
	gradienteditor.cpp	\
	gtaction.cpp	\
	gtdialogs.cpp	\
	gtfont.cpp	\
	gtframestyle.cpp	\
	gtgettext.cpp	\
	gtmeasure.cpp	\
	gtparagraphstyle.cpp	\
	gtstyle.cpp	\
	gtwriter.cpp	\
	guidemanager.cpp	\
	helpbrowser.cpp	\
	hnjalloc.c	\
	hruler.cpp	\
	hyask.cpp	\
	hyphenator.cpp	\
	hyphen.c	\
	hysettings.cpp	\
	imageinfodialog.cpp	\
	insertTable.cpp	\
	inspage.cpp	\
	ioapi.c	\
	javadocs.cpp	\
	keymanager.cpp	\
	langmgr.cpp	\
	layers.cpp	\
	linecombo.cpp	\
	lineformats.cpp	\
	linkbutton.cpp	\
	loadsaveplugin.cpp	\
	loremipsum.cpp	\
	margindialog.cpp	\
	marginWidget.cpp	\
	md5.c	\
	mdup.cpp	\
	measurements.cpp	\
	menumanager.cpp	\
	mergedoc.cpp	\
	missing.cpp	\
	movepage.cpp	\
	mpalette.cpp	\
	mspinbox.cpp	\
	multiline.cpp	\
	muster.cpp	\
	navigator.cpp	\
	newfile.cpp	\
	newtemp.cpp	\
	page.cpp	\
	pageitemattributes.ui	\
	pageitem.cpp	\
	pageitem_imageframe.cpp	\
	pageitem_line.cpp	\
	pageitem_pathtext.cpp	\
	pageitem_polygon.cpp	\
	pageitem_polyline.cpp	\
	pageitem_textframe.cpp	\
	pagelayout.cpp	\
	pageselector.cpp	\
	pagesize.cpp	\
	pdflib.cpp	\
	pdfoptions.cpp	\
	pdfoptionsio.cpp	\
	pdfopts.cpp	\
	picsearch.cpp	\
	picstatus.cpp	\
	pluginmanager.cpp	\
	pluginmanagerprefsgui.cpp	\
	polygonwidget.cpp	\
	polyprops.cpp	\
	prefscontext.cpp	\
	prefs.cpp	\
	prefsdialogbase.cpp	\
	prefsfile.cpp	\
	prefsmanager.cpp	\
	prefsreader.cpp	\
	prefstable.cpp	\
	preview.cpp	\
	pslib.cpp	\
	query.cpp	\
	rc4.c	\
	reformdoc.cpp	\
	rulermover.cpp	\
	sccolor.cpp	\
	sccombobox.cpp	\
	scfonts.cpp	\
	scfonts_ttf.cpp	\
	scfontmetrics.cpp	\
	scimage.cpp	\
	scmenu.cpp	\
	scmessagebox.cpp	\
	scpainter.cpp	\
	scpaths.cpp	\
	scplugin.cpp	\
	scpreview.cpp	\
	scraction.cpp	\
	scrap.cpp	\
	scribusdoc.cpp	\
	scribusview.cpp	\
	scribuswin.cpp	\
	scribusXml.cpp	\
	scrpalettebase.cpp	\
	search.cpp	\
	seiten.cpp	\
	selfield.cpp	\
	serializer.cpp	\
	shadebutton.cpp	\
	spalette.cpp	\
	splash.cpp	\
	story.cpp	\
	styleselect.cpp	\
	tabcheckdoc.cpp	\
	tabguides.cpp	\
	tabmanager.cpp	\
	tabpdfoptions.cpp	\
	tabruler.cpp	\
	tabtools.cpp	\
	tabtypography.cpp	\
	tocgenerator.cpp	\
	tocindexprefs.ui	\
	tree.cpp	\
	undogui.cpp	\
	undomanager.cpp	\
	undoobject.cpp	\
	undostate.cpp	\
	units.cpp	\
	unzip.c	\
	util.cpp	\
	vgradient.cpp	\
	vruler.cpp	\
	werktoolb.cpp	\
	scribus.cpp	\
	scribusapp.cpp	\
	main.cpp

INTERFACES =
TRANSLATIONS =

SUBDIRS = $(renderer_subdirs) dicts doc icons keysets manpages plugins po profiles qm templates dtd loremipsum

about.o: about_builddate.inc

about_builddate.inc:
	python -c 'f = open("about_builddate.inc", "w") ; import sys; args = sys.argv + [""]; import time; f.write(time.strftime("BUILD_YEAR = \"%Y\";\nBUILD_DAY = \"%d\";\nBUILD_MONTH = \"%B\";\nBUILD_TIME=\"%H:%M\";\nBUILD_TZ = \"%Z\";\n")); f.write("BUILD_NAME = \"%s\";\n" % args[1]); f.close()' ${BUILD_NAME}

scribus_LDADD  = $(LIBCAIRO_LIBS) $(LIBJPEG) $(LIBART_LIBS) $(LIBFREETYPE_LIBS) $(LIBFONTCONFIG_LIBS) $(LIBCUPS) $(LIBLCMS) $(LIBDL) $(LIBDL2) $(LIBTIFF) -lm $(LIB_QT) $(LIB_X11) $(LIBSOCKET) $(renderer_libs) $(COREFOUNDATION_LIBS) -lQt3Support_debug

AM_LDFLAGS = -export-dynamic $(LIBFREETYPE_LIBS)
# set the include path for X, qt and KDE
INCLUDES= $(LIBCAIRO_CFLAGS) $(LIBART_CFLAGS) $(LIBFREETYPE_CFLAGS) $(LIBFONTCONFIG_CFLAGS) $(all_includes)

scribus_METASOURCES = AUTO

# the library search path.
scribus_LDFLAGS =  -DQT3_SUPPORT $(all_libraries)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(scribus_METASOURCES)
# this is to enable Qt behavoir of am_edit
KDE_OPTIONS = qtonly

AM_CPPFLAGS = \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    -DSHAREDIR=\"$(datadir)/scribus/\" \
    -DQT3_SUPPORT \
    $(GCC4_VISIBILITY_FLAGS)

noinst_HEADERS = marginWidget.h
