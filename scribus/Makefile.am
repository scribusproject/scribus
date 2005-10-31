bin_PROGRAMS = scribus

if COMPILE_CAIRO
# We don't need to build libart/ or pixbuf/ since we're using Cairo
renderer_subdirs =
renderer_libs =
else
if LINK_PIXBUF
# Use a convenience library for the gdk-pixbuf sources so we can avoid
# compiling and linking them if they're not needed. We do this because using a
# variable subsitution in scribus_SOURCES appears to break the automatic
# moc'ing and uic'ing done by the build system.
renderer_subdirs = libart pixbuf
renderer_libs = libart/libart.la pixbuf/libpixbuf.la
else
# Use a convenience library to build our libart renderer, but omit
# libgdk-pixbuf. This is used for build paths that use libart, but don't have
# X11, such as Qt/Mac .
renderer_subdirs = libart
renderer_libs = libart/libart.la
endif
endif

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
	docsectionsbase.ui	\
	docsections.cpp	\
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
	fpoint.cpp	\
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
	tocindexprefsbase.ui	\
	tocindexprefs.cpp	\
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

#     NOTE: `EXTRA_DIST' is *NOT* required. The build system includes all
#     loose files by default. You only need to use `EXTRA_DIST' when using
#     `KDE_OPTIONS = noautodist'
# EXTRA_DIST =


about.o: about_builddate.inc

about_builddate.inc:
	python -c 'f = open("about_builddate.inc", "w") ; import sys; args = sys.argv + [""]; import time; f.write(time.strftime("BUILD_YEAR = \"%Y\";\nBUILD_DAY = \"%d\";\nBUILD_MONTH = \"%B\";\nBUILD_TIME=\"%H:%M\";\nBUILD_TZ = \"%Z\";\n")); f.write("BUILD_NAME = \"%s\";\n" % args[1]); f.close()' ${BUILD_NAME}

install-data-local:
	$(mkinstalldirs) $(libdir)/scribus/
	$(INSTALL_DATA) $(srcdir)/rgbsvg.txt $(libdir)/scribus/rgbsvg.txt
	$(mkinstalldirs) $(libdir)/scribus/
	$(INSTALL_DATA) $(srcdir)/rgbscribusgreys.txt $(libdir)/scribus/rgbscribusgreys.txt
	$(mkinstalldirs) $(libdir)/scribus/
	$(INSTALL_DATA) $(srcdir)/rgbscribusgnome.txt $(libdir)/scribus/rgbscribusgnome.txt
	$(mkinstalldirs) $(libdir)/scribus/
	$(INSTALL_DATA) $(srcdir)/rgbscribus.txt                $(libdir)/scribus/rgbscribus.txt
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/rgbscribusopenoffice.txt      $(libdir)/scribus/rgbscribusopenoffice.txt
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/about.h                       $(prefix)/include/scribus/about.h
	$(INSTALL_DATA) $(srcdir)/actionmanager.h               $(prefix)/include/scribus/actionmanager.h
	$(INSTALL_DATA) $(srcdir)/aligndistribute.h             $(prefix)/include/scribus/aligndistribute.h
	$(INSTALL_DATA) $(srcdir)/alignselect.h                 $(prefix)/include/scribus/alignselect.h
	$(INSTALL_DATA) $(srcdir)/annot.h                       $(prefix)/include/scribus/annot.h
	$(INSTALL_DATA) $(srcdir)/annota.h                      $(prefix)/include/scribus/annota.h
	$(INSTALL_DATA) $(srcdir)/applytemplatedialog.h         $(prefix)/include/scribus/applytemplatedialog.h
	$(INSTALL_DATA) $(srcdir)/arrowchooser.h                $(prefix)/include/scribus/arrowchooser.h
	$(INSTALL_DATA) $(srcdir)/autoform.h                    $(prefix)/include/scribus/autoform.h
	$(INSTALL_DATA) $(srcdir)/autoformbuttongroup.h         $(prefix)/include/scribus/autoformbuttongroup.h
	$(INSTALL_DATA) $(srcdir)/bookmwin.h                    $(prefix)/include/scribus/bookmwin.h
	$(INSTALL_DATA) $(srcdir)/bookpalette.h                 $(prefix)/include/scribus/bookpalette.h
	$(INSTALL_DATA) $(srcdir)/buttonicon.h                  $(prefix)/include/scribus/buttonicon.h
	$(INSTALL_DATA) $(srcdir)/charselect.h                  $(prefix)/include/scribus/charselect.h
	$(INSTALL_DATA) $(srcdir)/checkDocument.h               $(prefix)/include/scribus/checkDocument.h
	$(INSTALL_DATA) $(srcdir)/cmsprefs.h                    $(prefix)/include/scribus/cmsprefs.h
	$(INSTALL_DATA) $(srcdir)/cmykfw.h                      $(prefix)/include/scribus/cmykfw.h
	$(INSTALL_DATA) $(srcdir)/collect4output.h              $(prefix)/include/scribus/collect4output.h
	$(INSTALL_DATA) $(srcdir)/color.h                       $(prefix)/include/scribus/color.h
	$(INSTALL_DATA) $(srcdir)/colorchart.h                  $(prefix)/include/scribus/colorchart.h
	$(INSTALL_DATA) $(srcdir)/colorm.h                      $(prefix)/include/scribus/colorm.h
	$(INSTALL_DATA) $(srcdir)/cpalette.h                    $(prefix)/include/scribus/cpalette.h
	$(INSTALL_DATA) $(srcdir)/crypt.h                       $(prefix)/include/scribus/crypt.h
	$(INSTALL_DATA) $(srcdir)/cupsoptions.h                 $(prefix)/include/scribus/cupsoptions.h
	$(INSTALL_DATA) $(srcdir)/customfdialog.h               $(prefix)/include/scribus/customfdialog.h
	$(INSTALL_DATA) $(srcdir)/dcolor.h                      $(prefix)/include/scribus/dcolor.h
	$(INSTALL_DATA) $(srcdir)/delpages.h                    $(prefix)/include/scribus/delpages.h
	$(INSTALL_DATA) $(srcdir)/docinfo.h                     $(prefix)/include/scribus/docinfo.h
	$(INSTALL_DATA) $(srcdir)/docitemattrprefs.h            $(prefix)/include/scribus/docitemattrprefs.h
	$(INSTALL_DATA) $(srcdir)/documentchecker.h             $(prefix)/include/scribus/documentchecker.h
	$(INSTALL_DATA) $(srcdir)/documentinformation.h         $(prefix)/include/scribus/documentinformation.h
	$(INSTALL_DATA) $(srcdir)/druck.h                       $(prefix)/include/scribus/druck.h
	$(INSTALL_DATA) $(srcdir)/dynamictip.h                  $(prefix)/include/scribus/dynamictip.h
	$(INSTALL_DATA) $(srcdir)/edit1format.h                 $(prefix)/include/scribus/edit1format.h
	$(INSTALL_DATA) $(srcdir)/editformats.h                 $(prefix)/include/scribus/editformats.h
	$(INSTALL_DATA) $(srcdir)/editor.h                      $(prefix)/include/scribus/editor.h
	$(INSTALL_DATA) $(srcdir)/extimageprops.h               $(prefix)/include/scribus/extimageprops.h
	$(INSTALL_DATA) $(srcdir)/fileloader.h                  $(prefix)/include/scribus/fileloader.h
	$(INSTALL_DATA) $(srcdir)/fileunzip.h                   $(prefix)/include/scribus/fileunzip.h
	$(INSTALL_DATA) $(srcdir)/filewatcher.h                 $(prefix)/include/scribus/filewatcher.h
	$(INSTALL_DATA) $(srcdir)/fmitem.h                      $(prefix)/include/scribus/fmitem.h
	$(INSTALL_DATA) $(srcdir)/fontcombo.h                   $(prefix)/include/scribus/fontcombo.h
	$(INSTALL_DATA) $(srcdir)/fontprefs.h                   $(prefix)/include/scribus/fontprefs.h
	$(INSTALL_DATA) $(srcdir)/fontreplacedialog.h           $(prefix)/include/scribus/fontreplacedialog.h
	$(INSTALL_DATA) $(srcdir)/fparser.h                     $(prefix)/include/scribus/fparser.h
	$(INSTALL_DATA) $(srcdir)/fpoint.h                      $(prefix)/include/scribus/fpoint.h
	$(INSTALL_DATA) $(srcdir)/fpointarray.h                 $(prefix)/include/scribus/fpointarray.h
	$(INSTALL_DATA) $(srcdir)/frameedit.h                   $(prefix)/include/scribus/frameedit.h
	$(INSTALL_DATA) $(srcdir)/gradienteditor.h              $(prefix)/include/scribus/gradienteditor.h
	$(INSTALL_DATA) $(srcdir)/gtaction.h                    $(prefix)/include/scribus/gtaction.h
	$(INSTALL_DATA) $(srcdir)/gtdialogs.h                   $(prefix)/include/scribus/gtdialogs.h
	$(INSTALL_DATA) $(srcdir)/gtfont.h                      $(prefix)/include/scribus/gtfont.h
	$(INSTALL_DATA) $(srcdir)/gtframestyle.h                $(prefix)/include/scribus/gtframestyle.h
	$(INSTALL_DATA) $(srcdir)/gtgettext.h                   $(prefix)/include/scribus/gtgettext.h
	$(INSTALL_DATA) $(srcdir)/gtmeasure.h                   $(prefix)/include/scribus/gtmeasure.h
	$(INSTALL_DATA) $(srcdir)/gtparagraphstyle.h            $(prefix)/include/scribus/gtparagraphstyle.h
	$(INSTALL_DATA) $(srcdir)/gtstyle.h                     $(prefix)/include/scribus/gtstyle.h
	$(INSTALL_DATA) $(srcdir)/gtwriter.h                    $(prefix)/include/scribus/gtwriter.h
	$(INSTALL_DATA) $(srcdir)/guidemanager.h                $(prefix)/include/scribus/guidemanager.h
	$(INSTALL_DATA) $(srcdir)/helpbrowser.h                 $(prefix)/include/scribus/helpbrowser.h
	$(INSTALL_DATA) $(srcdir)/hnjalloc.h                    $(prefix)/include/scribus/hnjalloc.h
	$(INSTALL_DATA) $(srcdir)/hruler.h                      $(prefix)/include/scribus/hruler.h
	$(INSTALL_DATA) $(srcdir)/hyask.h                       $(prefix)/include/scribus/hyask.h
	$(INSTALL_DATA) $(srcdir)/hyphen.h                      $(prefix)/include/scribus/hyphen.h
	$(INSTALL_DATA) $(srcdir)/hyphenator.h                  $(prefix)/include/scribus/hyphenator.h
	$(INSTALL_DATA) $(srcdir)/hysettings.h                  $(prefix)/include/scribus/hysettings.h
	$(INSTALL_DATA) $(srcdir)/icons5.h                      $(prefix)/include/scribus/icons5.h
	$(INSTALL_DATA) $(srcdir)/icons6.h                      $(prefix)/include/scribus/icons6.h
	$(INSTALL_DATA) $(srcdir)/insertTable.h                 $(prefix)/include/scribus/insertTable.h
	$(INSTALL_DATA) $(srcdir)/inspage.h                     $(prefix)/include/scribus/inspage.h
	$(INSTALL_DATA) $(srcdir)/ioapi.h                       $(prefix)/include/scribus/ioapi.h
	$(INSTALL_DATA) $(srcdir)/javadocs.h                    $(prefix)/include/scribus/javadocs.h
	$(INSTALL_DATA) $(srcdir)/keymanager.h                  $(prefix)/include/scribus/keymanager.h
	$(INSTALL_DATA) $(srcdir)/langmgr.h                     $(prefix)/include/scribus/langmgr.h
	$(INSTALL_DATA) $(srcdir)/layers.h                      $(prefix)/include/scribus/layers.h
	$(INSTALL_DATA) $(srcdir)/linecombo.h                   $(prefix)/include/scribus/linecombo.h
	$(INSTALL_DATA) $(srcdir)/lineformats.h                 $(prefix)/include/scribus/lineformats.h
	$(INSTALL_DATA) $(srcdir)/linkbutton.h                  $(prefix)/include/scribus/linkbutton.h
	$(INSTALL_DATA) $(srcdir)/loremipsum.h                  $(prefix)/include/scribus/loremipsum.h
	$(INSTALL_DATA) $(srcdir)/md5.h                         $(prefix)/include/scribus/md5.h
	$(INSTALL_DATA) $(srcdir)/mdup.h                        $(prefix)/include/scribus/mdup.h
	$(INSTALL_DATA) $(srcdir)/margindialog.h                $(prefix)/include/scribus/margindialog.h
	$(INSTALL_DATA) $(srcdir)/marginWidget.h                $(prefix)/include/scribus/marginWidget.h
	$(INSTALL_DATA) $(srcdir)/measurements.h                $(prefix)/include/scribus/measurements.h
	$(INSTALL_DATA) $(srcdir)/menumanager.h                 $(prefix)/include/scribus/menumanager.h
	$(INSTALL_DATA) $(srcdir)/mergedoc.h                    $(prefix)/include/scribus/mergedoc.h
	$(INSTALL_DATA) $(srcdir)/missing.h                     $(prefix)/include/scribus/missing.h
	$(INSTALL_DATA) $(srcdir)/movepage.h                    $(prefix)/include/scribus/movepage.h
	$(INSTALL_DATA) $(srcdir)/mpalette.h                    $(prefix)/include/scribus/mpalette.h
	$(INSTALL_DATA) $(srcdir)/mspinbox.h                    $(prefix)/include/scribus/mspinbox.h
	$(INSTALL_DATA) $(srcdir)/multiline.h                   $(prefix)/include/scribus/multiline.h
	$(INSTALL_DATA) $(srcdir)/muster.h                      $(prefix)/include/scribus/muster.h
	$(INSTALL_DATA) $(srcdir)/navigator.h                   $(prefix)/include/scribus/navigator.h
	$(INSTALL_DATA) $(srcdir)/newfile.h                     $(prefix)/include/scribus/newfile.h
	$(INSTALL_DATA) $(srcdir)/newtemp.h                     $(prefix)/include/scribus/newtemp.h
	$(INSTALL_DATA) $(srcdir)/page.h                        $(prefix)/include/scribus/page.h
	$(INSTALL_DATA) $(srcdir)/pageitem.h                    $(prefix)/include/scribus/pageitem.h
	$(INSTALL_DATA) $(srcdir)/pageitemattributes.h          $(prefix)/include/scribus/pageitemattributes.h
	$(INSTALL_DATA) $(srcdir)/pagelayout.h                  $(prefix)/include/scribus/pagelayout.h
	$(INSTALL_DATA) $(srcdir)/pageselector.h                $(prefix)/include/scribus/pageselector.h
	$(INSTALL_DATA) $(srcdir)/pagesize.h                    $(prefix)/include/scribus/pagesize.h
	$(INSTALL_DATA) $(srcdir)/pagestructs.h                 $(prefix)/include/scribus/pagestructs.h
	$(INSTALL_DATA) $(srcdir)/pdfopts.h                     $(prefix)/include/scribus/pdfopts.h
	$(INSTALL_DATA) $(srcdir)/picsearch.h                   $(prefix)/include/scribus/picsearch.h
	$(INSTALL_DATA) $(srcdir)/picstatus.h                   $(prefix)/include/scribus/picstatus.h
	$(INSTALL_DATA) $(srcdir)/pluginmanager.h               $(prefix)/include/scribus/pluginmanager.h
	$(INSTALL_DATA) $(srcdir)/polygonwidget.h               $(prefix)/include/scribus/polygonwidget.h
	$(INSTALL_DATA) $(srcdir)/polyprops.h                   $(prefix)/include/scribus/polyprops.h
	$(INSTALL_DATA) $(srcdir)/prefs.h                       $(prefix)/include/scribus/prefs.h
	$(INSTALL_DATA) $(srcdir)/prefscontext.h                $(prefix)/include/scribus/prefscontext.h
	$(INSTALL_DATA) $(srcdir)/prefsdialogbase.h             $(prefix)/include/scribus/prefsdialogbase.h
	$(INSTALL_DATA) $(srcdir)/prefsfile.h                   $(prefix)/include/scribus/prefsfile.h
	$(INSTALL_DATA) $(srcdir)/prefsmanager.h                $(prefix)/include/scribus/prefsmanager.h
	$(INSTALL_DATA) $(srcdir)/prefspanel.h                  $(prefix)/include/scribus/prefspanel.h
	$(INSTALL_DATA) $(srcdir)/prefsreader.h                 $(prefix)/include/scribus/prefsreader.h
	$(INSTALL_DATA) $(srcdir)/prefstable.h                  $(prefix)/include/scribus/prefstable.h
	$(INSTALL_DATA) $(srcdir)/preview.h                     $(prefix)/include/scribus/preview.h
	$(INSTALL_DATA) $(srcdir)/query.h                       $(prefix)/include/scribus/query.h
	$(INSTALL_DATA) $(srcdir)/rc4.h                         $(prefix)/include/scribus/rc4.h
	$(INSTALL_DATA) $(srcdir)/rulermover.h                  $(prefix)/include/scribus/rulermover.h
	$(INSTALL_DATA) $(srcdir)/reformdoc.h                   $(prefix)/include/scribus/reformdoc.h
	$(INSTALL_DATA) $(srcdir)/sccolor.h                     $(prefix)/include/scribus/sccolor.h
	$(INSTALL_DATA) $(srcdir)/scfonts.h                     $(prefix)/include/scribus/scfonts.h
	$(INSTALL_DATA) $(srcdir)/scfonts_ttf.h                 $(prefix)/include/scribus/scfonts_ttf.h
	$(INSTALL_DATA) $(srcdir)/scfontmetrics.h               $(prefix)/include/scribus/scfontmetrics.h
	$(INSTALL_DATA) $(srcdir)/scmenu.h                      $(prefix)/include/scribus/scmenu.h
	$(INSTALL_DATA) $(srcdir)/scmessagebox.h                $(prefix)/include/scribus/scmessagebox.h
	$(INSTALL_DATA) $(srcdir)/scpainter.h                   $(prefix)/include/scribus/scpainter.h
	$(INSTALL_DATA) $(srcdir)/scpaths.h                     $(prefix)/include/scribus/scpaths.h
	$(INSTALL_DATA) $(srcdir)/scplugin.h                    $(prefix)/include/scribus/scplugin.h
	$(INSTALL_DATA) $(srcdir)/scpreview.h                   $(prefix)/include/scribus/scpreview.h
	$(INSTALL_DATA) $(srcdir)/scraction.h                   $(prefix)/include/scribus/scraction.h
	$(INSTALL_DATA) $(srcdir)/scrap.h                       $(prefix)/include/scribus/scrap.h
	$(INSTALL_DATA) $(srcdir)/scribus.h                     $(prefix)/include/scribus/scribus.h
	$(INSTALL_DATA) $(srcdir)/scribusapp.h                  $(prefix)/include/scribus/scribusapp.h
	$(INSTALL_DATA) $(srcdir)/scribusdoc.h                  $(prefix)/include/scribus/scribusdoc.h
	$(INSTALL_DATA) $(srcdir)/scribusstructs.h              $(prefix)/include/scribus/scribusstructs.h
	$(INSTALL_DATA) $(srcdir)/scribusview.h                 $(prefix)/include/scribus/scribusview.h
	$(INSTALL_DATA) $(srcdir)/scribuswin.h                  $(prefix)/include/scribus/scribuswin.h
	$(INSTALL_DATA) $(srcdir)/scribusXml.h                  $(prefix)/include/scribus/scribusXml.h
	$(INSTALL_DATA) $(srcdir)/scrpalettebase.h              $(prefix)/include/scribus/scrpalettebase.h
	$(INSTALL_DATA) $(srcdir)/search.h                      $(prefix)/include/scribus/search.h
	$(INSTALL_DATA) $(srcdir)/seiten.h                      $(prefix)/include/scribus/seiten.h
	$(INSTALL_DATA) $(srcdir)/selfield.h                    $(prefix)/include/scribus/selfield.h
	$(INSTALL_DATA) $(srcdir)/serializer.h                  $(prefix)/include/scribus/serializer.h
	$(INSTALL_DATA) $(srcdir)/shadebutton.h                 $(prefix)/include/scribus/shadebutton.h
	$(INSTALL_DATA) $(srcdir)/spalette.h                    $(prefix)/include/scribus/spalette.h
	$(INSTALL_DATA) $(srcdir)/splash.h                      $(prefix)/include/scribus/splash.h
	$(INSTALL_DATA) $(srcdir)/story.h                       $(prefix)/include/scribus/story.h
	$(INSTALL_DATA) $(srcdir)/styleselect.h                 $(prefix)/include/scribus/styleselect.h
	$(INSTALL_DATA) $(srcdir)/tabcheckdoc.h                 $(prefix)/include/scribus/tabcheckdoc.h
	$(INSTALL_DATA) $(srcdir)/tabguides.h                   $(prefix)/include/scribus/tabguides.h
	$(INSTALL_DATA) $(srcdir)/tabmanager.h                  $(prefix)/include/scribus/tabmanager.h
	$(INSTALL_DATA) $(srcdir)/tabpdfoptions.h               $(prefix)/include/scribus/tabpdfoptions.h
	$(INSTALL_DATA) $(srcdir)/tabruler.h                    $(prefix)/include/scribus/tabruler.h
	$(INSTALL_DATA) $(srcdir)/tabtools.h                    $(prefix)/include/scribus/tabtools.h
	$(INSTALL_DATA) $(srcdir)/tabtypography.h               $(prefix)/include/scribus/tabtypography.h
	$(INSTALL_DATA) $(srcdir)/tocgenerator.h                $(prefix)/include/scribus/tocgenerator.h
	$(INSTALL_DATA) $(srcdir)/tocindexprefs.h               $(prefix)/include/scribus/tocindexprefs.h
	$(INSTALL_DATA) $(srcdir)/tree.h                        $(prefix)/include/scribus/tree.h
	$(INSTALL_DATA) $(srcdir)/undogui.h                     $(prefix)/include/scribus/undogui.h
	$(INSTALL_DATA) $(srcdir)/undomanager.h                 $(prefix)/include/scribus/undomanager.h
	$(INSTALL_DATA) $(srcdir)/undoobject.h                  $(prefix)/include/scribus/undoobject.h
	$(INSTALL_DATA) $(srcdir)/undostate.h                   $(prefix)/include/scribus/undostate.h
	$(INSTALL_DATA) $(srcdir)/units.h                       $(prefix)/include/scribus/units.h
	$(INSTALL_DATA) $(srcdir)/unzip.h                       $(prefix)/include/scribus/unzip.h
	$(INSTALL_DATA) $(srcdir)/vgradient.h                   $(prefix)/include/scribus/vgradient.h
	$(INSTALL_DATA) $(srcdir)/vruler.h                      $(prefix)/include/scribus/vruler.h
	$(INSTALL_DATA) $(srcdir)/werktoolb.h                   $(prefix)/include/scribus/werktoolb.h

uninstall-local:
	-rm -f $(libdir)/scribus/rgbscribus.txt
	-rm -f $(libdir)/scribus/rgbscribusgnome.txt
	-rm -f $(libdir)/scribus/rgbscribusgreys.txt
	-rm -f $(libdir)/scribus/rgbscribusopenoffice.txt
	-rm -f $(libdir)/scribus/rgbsvg.txt
	-rm -f $(prefix)/include/scribus/about.h
	-rm -f $(prefix)/include/scribus/actionmanager.h
	-rm -f $(prefix)/include/scribus/aligndistribute.h
	-rm -f $(prefix)/include/scribus/alignselect.h
	-rm -f $(prefix)/include/scribus/annot.h
	-rm -f $(prefix)/include/scribus/annota.h
	-rm -f $(prefix)/include/scribus/applytemplatedialog.h
	-rm -f $(prefix)/include/scribus/arrowchooser.h
	-rm -f $(prefix)/include/scribus/art_kmisc.h
	-rm -f $(prefix)/include/scribus/art_render_misc.h
	-rm -f $(prefix)/include/scribus/art_render_pattern.h
	-rm -f $(prefix)/include/scribus/art_rgb.h
	-rm -f $(prefix)/include/scribus/art_rgba_affine.h
	-rm -f $(prefix)/include/scribus/art_rgb_affine_private.h
	-rm -f $(prefix)/include/scribus/art_rgb_svp.h
	-rm -f $(prefix)/include/scribus/autoform.h
	-rm -f $(prefix)/include/scribus/autoformbuttongroup.h
	-rm -f $(prefix)/include/scribus/bookmwin.h
	-rm -f $(prefix)/include/scribus/bookpalette.h
	-rm -f $(prefix)/include/scribus/buttonicon.h
	-rm -f $(prefix)/include/scribus/charselect.h
	-rm -f $(prefix)/include/scribus/checkDocument.h
	-rm -f $(prefix)/include/scribus/cmsprefs.h
	-rm -f $(prefix)/include/scribus/cmykfw.h
	-rm -f $(prefix)/include/scribus/collect4output.h
	-rm -f $(prefix)/include/scribus/color.h
	-rm -f $(prefix)/include/scribus/colorchart.h
	-rm -f $(prefix)/include/scribus/colorm.h
	-rm -f $(prefix)/include/scribus/cpalette.h
	-rm -f $(prefix)/include/scribus/crypt.h
	-rm -f $(prefix)/include/scribus/cupsoptions.h
	-rm -f $(prefix)/include/scribus/customfdialog.h
	-rm -f $(prefix)/include/scribus/dcolor.h
	-rm -f $(prefix)/include/scribus/delpages.h
	-rm -f $(prefix)/include/scribus/docinfo.h
	-rm -f $(prefix)/include/scribus/docitemattrprefs.h
	-rm -f $(prefix)/include/scribus/documentchecker.h
	-rm -f $(prefix)/include/scribus/documentinformation.h
	-rm -f $(prefix)/include/scribus/druck.h
	-rm -f $(prefix)/include/scribus/dynamictip.h
	-rm -f $(prefix)/include/scribus/edit1format.h
	-rm -f $(prefix)/include/scribus/editformats.h
	-rm -f $(prefix)/include/scribus/editor.h
	-rm -f $(prefix)/include/scribus/extimageprops.h
	-rm -f $(prefix)/include/scribus/fileloader.h
	-rm -f $(prefix)/include/scribus/fileunzip.h
	-rm -f $(prefix)/include/scribus/filewatcher.h
	-rm -f $(prefix)/include/scribus/fmitem.h
	-rm -f $(prefix)/include/scribus/fontcombo.h
	-rm -f $(prefix)/include/scribus/fontcombo.h
	-rm -f $(prefix)/include/scribus/fontprefs.h
	-rm -f $(prefix)/include/scribus/fontreplacedialog.h
	-rm -f $(prefix)/include/scribus/fparser.h
	-rm -f $(prefix)/include/scribus/fpoint.h
	-rm -f $(prefix)/include/scribus/fpointarray.h
	-rm -f $(prefix)/include/scribus/frameedit.h
	-rm -f $(prefix)/include/scribus/gradienteditor.h
	-rm -f $(prefix)/include/scribus/gradienteditor.h
	-rm -f $(prefix)/include/scribus/gtaction.h
	-rm -f $(prefix)/include/scribus/gtdialogs.h
	-rm -f $(prefix)/include/scribus/gtfont.h
	-rm -f $(prefix)/include/scribus/gtframestyle.h
	-rm -f $(prefix)/include/scribus/gtgettext.h
	-rm -f $(prefix)/include/scribus/gtmeasure.h
	-rm -f $(prefix)/include/scribus/gtparagraphstyle.h
	-rm -f $(prefix)/include/scribus/gtstyle.h
	-rm -f $(prefix)/include/scribus/gtwriter.h
	-rm -f $(prefix)/include/scribus/guidemanager.h
	-rm -f $(prefix)/include/scribus/helpbrowser.h
	-rm -f $(prefix)/include/scribus/hnjalloc.h
	-rm -f $(prefix)/include/scribus/hruler.h
	-rm -f $(prefix)/include/scribus/hyask.h
	-rm -f $(prefix)/include/scribus/hyphen.h
	-rm -f $(prefix)/include/scribus/hyphenator.h
	-rm -f $(prefix)/include/scribus/hysettings.h
	-rm -f $(prefix)/include/scribus/icons5.h
	-rm -f $(prefix)/include/scribus/icons6.h
	-rm -f $(prefix)/include/scribus/insertTable.h
	-rm -f $(prefix)/include/scribus/inspage.h
	-rm -f $(prefix)/include/scribus/ioapi.h
	-rm -f $(prefix)/include/scribus/javadocs.h
	-rm -f $(prefix)/include/scribus/keymanager.h
	-rm -f $(prefix)/include/scribus/langmgr.h
	-rm -f $(prefix)/include/scribus/layers.h
	-rm -f $(prefix)/include/scribus/linecombo.h
	-rm -f $(prefix)/include/scribus/lineformats.h
	-rm -f $(prefix)/include/scribus/linkbutton.h
	-rm -f $(prefix)/include/scribus/loremipsum.h
	-rm -f $(prefix)/include/scribus/md5.h
	-rm -f $(prefix)/include/scribus/mdup.h
	-rm -f $(prefix)/include/scribus/measurements.h
	-rm -f $(prefix)/include/scribus/menumanager.h
	-rm -f $(prefix)/include/scribus/mergedoc.h
	-rm -f $(prefix)/include/scribus/missing.h
	-rm -f $(prefix)/include/scribus/movepage.h
	-rm -f $(prefix)/include/scribus/mpalette.h
	-rm -f $(prefix)/include/scribus/mspinbox.h
	-rm -f $(prefix)/include/scribus/multiline.h
	-rm -f $(prefix)/include/scribus/muster.h
	-rm -f $(prefix)/include/scribus/navigator.h
	-rm -f $(prefix)/include/scribus/newfile.h
	-rm -f $(prefix)/include/scribus/newtemp.h
	-rm -f $(prefix)/include/scribus/page.h
	-rm -f $(prefix)/include/scribus/pageitem.h
	-rm -f $(prefix)/include/scribus/pageitemattributes.h
	-rm -f $(prefix)/include/scribus/pagelayout.h
	-rm -f $(prefix)/include/scribus/pageselector.h
	-rm -f $(prefix)/include/scribus/pagesize.h
	-rm -f $(prefix)/include/scribus/pagestructs.h
	-rm -f $(prefix)/include/scribus/pdfopts.h
	-rm -f $(prefix)/include/scribus/picsearch.h
	-rm -f $(prefix)/include/scribus/picstatus.h
	-rm -f $(prefix)/include/scribus/pluginmanager.h
	-rm -f $(prefix)/include/scribus/polygonwidget.h
	-rm -f $(prefix)/include/scribus/polyprops.h
	-rm -f $(prefix)/include/scribus/prefs.h
	-rm -f $(prefix)/include/scribus/prefscontext.h
	-rm -f $(prefix)/include/scribus/prefsdialogbase.h
	-rm -f $(prefix)/include/scribus/prefsfile.h
	-rm -f $(prefix)/include/scribus/prefsmanager.h
	-rm -f $(prefix)/include/scribus/prefsreader.h
	-rm -f $(prefix)/include/scribus/prefstable.h
	-rm -f $(prefix)/include/scribus/preview.h
	-rm -f $(prefix)/include/scribus/query.h
	-rm -f $(prefix)/include/scribus/rc4.h
	-rm -f $(prefix)/include/scribus/rulermover.h
	-rm -f $(prefix)/include/scribus/reformdoc.h
	-rm -f $(prefix)/include/scribus/sccolor.h
	-rm -f $(prefix)/include/scribus/scfonts.h
	-rm -f $(prefix)/include/scribus/scfonts_ttf.h
	-rm -f $(prefix)/include/scribus/scmenu.h
	-rm -f $(prefix)/include/scribus/scmessagebox.h
	-rm -f $(prefix)/include/scribus/scpainter.h
	-rm -f $(prefix)/include/scribus/scpaths.h
	-rm -f $(prefix)/include/scribus/scpreview.h
	-rm -f $(prefix)/include/scribus/scraction.h
	-rm -f $(prefix)/include/scribus/scrap.h
	-rm -f $(prefix)/include/scribus/scribus.h
	-rm -f $(prefix)/include/scribus/scribusapp.h
	-rm -f $(prefix)/include/scribus/scribusdoc.h
	-rm -f $(prefix)/include/scribus/scribusstructs.h
	-rm -f $(prefix)/include/scribus/scribusview.h
	-rm -f $(prefix)/include/scribus/scribuswin.h
	-rm -f $(prefix)/include/scribus/scribusXml.h
	-rm -f $(prefix)/include/scribus/scrpalettebase.h
	-rm -f $(prefix)/include/scribus/search.h
	-rm -f $(prefix)/include/scribus/seiten.h
	-rm -f $(prefix)/include/scribus/selfield.h
	-rm -f $(prefix)/include/scribus/serializer.h
	-rm -f $(prefix)/include/scribus/shadebutton.h
	-rm -f $(prefix)/include/scribus/spalette.h
	-rm -f $(prefix)/include/scribus/splash.h
	-rm -f $(prefix)/include/scribus/story.h
	-rm -f $(prefix)/include/scribus/styleselect.h
	-rm -f $(prefix)/include/scribus/tabcheckdoc.h
	-rm -f $(prefix)/include/scribus/tabguides.h
	-rm -f $(prefix)/include/scribus/tabmanager.h
	-rm -f $(prefix)/include/scribus/tabpdfoptions.h
	-rm -f $(prefix)/include/scribus/tabruler.h
	-rm -f $(prefix)/include/scribus/tabtools.h
	-rm -f $(prefix)/include/scribus/tabtypography.h
	-rm -f $(prefix)/include/scribus/tocindexprefs.h
	-rm -f $(prefix)/include/scribus/tocgenerator.h
	-rm -f $(prefix)/include/scribus/tree.h
	-rm -f $(prefix)/include/scribus/undogui.h
	-rm -f $(prefix)/include/scribus/undomanager.h
	-rm -f $(prefix)/include/scribus/undoobject.h
	-rm -f $(prefix)/include/scribus/undostate.h
	-rm -f $(prefix)/include/scribus/units.h
	-rm -f $(prefix)/include/scribus/unzip.h
	-rm -f $(prefix)/include/scribus/vgradient.h
	-rm -f $(prefix)/include/scribus/vruler.h
	-rm -f $(prefix)/include/scribus/werktoolb.h

scribus_LDADD  = $(LIBCAIRO_LIBS) $(LIBJPEG) $(LIBART_LIBS) $(LIBFREETYPE_LIBS) $(LIBFONTCONFIG_LIBS) $(LIBCUPS) $(LIBLCMS) $(LIBDL) $(LIBDL2) $(LIBTIFF) -lm $(LIB_QT) $(LIB_X11) $(LIBSOCKET) $(renderer_libs) $(COREFOUNDATION_LIBS)

AM_LDFLAGS = -export-dynamic $(LIBFREETYPE_LIBS)
# set the include path for X, qt and KDE
INCLUDES= $(LIBCAIRO_CFLAGS) $(LIBART_CFLAGS) $(LIBFREETYPE_CFLAGS) $(LIBFONTCONFIG_CFLAGS) $(all_includes)

scribus_METASOURCES = AUTO

# the library search path.
scribus_LDFLAGS = $(all_libraries)

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
    $(GCC4_VISIBILITY_FLAGS)

noinst_HEADERS = marginWidget.h
