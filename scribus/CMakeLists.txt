#For those that want it, show the SVN version in the About dialog
if (WANT_SVNVERSION)
	add_definitions(-DHAVE_SVNVERSION)
	message(STATUS "SVN Version requested in About dialog")
	# a custom target that is always built

	if (NOT CMAKE_VERSION VERSION_LESS "3.2.0")
	add_custom_target(svnheader ALL COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_MODULE_PATH}/getsvn.cmake BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h)

	else ()
	add_custom_target(svnheader ALL DEPENDS svn_header ) # svn_header is nothing more than a unique string

	# creates svnheader.h using cmake script
	add_custom_command(OUTPUT svn_header ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
		COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_MODULE_PATH}/getsvn.cmake)

	set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
		PROPERTIES GENERATED TRUE
		HEADER_FILE_ONLY TRUE)
	endif ()
endif ()

if(WIN32)
	add_definitions(-DQT_DLL)
endif()

add_subdirectory(dtd)
add_subdirectory(colormgmt)
add_subdirectory(desaxe)
add_subdirectory(fonts)
add_subdirectory(styles)
add_subdirectory(text)
add_subdirectory(ui/qml)
if(WITH_TESTS)
	add_subdirectory(tests)
endif()
add_subdirectory(unicodemap)
if (WANT_DESIGNER)
	add_subdirectory(designer)
endif ()
add_subdirectory(third_party)

if(HAVE_OSG)
	set(SCRIBUS_OSG_UI_SRC
		ui/osgeditor.ui
	)
	set(SCRIBUS_OSG_CLASS
		ui/AdapterWidget.h
		ui/osgeditor.h
		pageitem_osgframe.h
	)
	set(SCRIBUS_OSG_SRC
		ui/AdapterWidget.cpp
		ui/osgeditor.cpp
		pageitem_osgframe.cpp
	)
endif ()

if (GMAGICK_FOUND)
	set(SCRIBUS_GMAGICK_SRC imagedataloaders/scimgdataloader_gmagick.cpp)
else()
	set(SCRIBUS_GMAGICK_SRC)
endif()

# We setup something like a group to gather setter related stuff
# because it could expand into a huge amount of files at some point.
set(SCRIBUS_ITEMSETTERS_UI_SRC
	ui/positionsetter.ui
	ui/rotationsetter.ui
)

set(SCRIBUS_ITEMSETTERS_CLASS
	openpalettemanager.h
	openpalettemodel.h
	pageitemsettersmanager.h
	ui/openpalette.h
	ui/openpaletteview.h
	ui/pageitempositionsetter.h
	ui/pageitemrotationsetter.h
)

set(SCRIBUS_ITEMSETTERS_SRC
	openpalettemanager.cpp
	openpalettemodel.cpp
	pageitemsettersmanager.cpp
	ui/openpalette.cpp
	ui/openpaletteview.cpp
	ui/pageitemsetterbase.cpp
	ui/pageitempositionsetter.cpp
	ui/pageitemrotationsetter.cpp
)

set(SCRIBUS_UI_SRC
	ui/aboutplugins.ui
	ui/aligndistribute.ui
	ui/annot.ui
	ui/arcvectorbase.ui
	ui/arcwidgetbase.ui
	ui/charselect.ui
	ui/charselectenhanced.ui
	ui/cmsprefsbase.ui
	ui/cmykfwbase.ui
	ui/colorpalette.ui
	ui/colorsandfillsbase.ui
	ui/cxfimportdialogbase.ui
	ui/downloadspalettebase.ui
	ui/gradientaddedit.ui
	ui/gradienteditor.ui
	ui/gradientvectorbase.ui
	ui/gtfiledialog.ui
	ui/guidemanager.ui
	ui/insertaframe.ui
	ui/inserttablecolumnsdialog.ui
	ui/inserttablerowsdialog.ui
	ui/latexeditor.ui
	ui/newmarginwidgetbase.ui
	ui/mark2item.ui
	ui/mark2mark.ui
	ui/markanchor.ui
	ui/marknote.ui
	ui/marksmanager.ui
	ui/markvariabletext.ui
	ui/measurementsbase.ui
	ui/multipleduplicate.ui
	ui/multiprogressdialog.ui
	ui/nftdialog.ui
	ui/nftwidget.ui
	ui/notesstyleseditor.ui
	ui/pageitemattributes.ui
	ui/pagepalette_pagesbase.ui
	ui/pagepalette_masterpagesbase.ui
	ui/picsearch.ui
	ui/picsearchoptions.ui
	ui/picstatus.ui
	ui/pluginmanagerprefsgui.ui
	ui/polygonwidgetbase.ui
	ui/preferencesdialogbase.ui
	ui/prefs_colormanagementbase.ui
	ui/prefs_displaybase.ui
	ui/prefs_documentinformationbase.ui
	ui/prefs_documentsectionsbase.ui
	ui/prefs_documentsetupbase.ui
	ui/prefs_externaltoolsbase.ui
	ui/prefs_fontsbase.ui
	ui/prefs_guidesbase.ui
	ui/prefs_hyphenatorbase.ui
	ui/prefs_imagecachebase.ui
	ui/prefs_keyboardshortcutsbase.ui
	ui/prefs_miscellaneousbase.ui
	ui/prefs_pathsbase.ui
	ui/prefs_pagesizesbase.ui
	ui/prefs_pluginsbase.ui
	ui/prefs_preflightverifierbase.ui
	ui/prefs_printerbase.ui
	ui/prefs_scrapbookbase.ui
	ui/prefs_spellingbase.ui
	ui/prefs_typographybase.ui
	ui/prefs_userinterfacebase.ui
	ui/prefs_itemtoolsbase.ui
	ui/prefs_operatortoolsbase.ui
	ui/prefs_pdfexportbase.ui
	ui/prefs_tableofcontentsbase.ui
	ui/prefs_documentitemattributesbase.ui
	ui/printdialogbase.ui
	ui/propertiespalette_groupbase.ui
	ui/propertiespalette_imagebase.ui
	ui/propertiespalette_linebase.ui
	ui/propertiespalette_shapebase.ui
	ui/propertiespalette_tablebase.ui
	ui/propertiespalette_textbase.ui
	ui/propertiespalette_xyzbase.ui
	ui/propertywidget_advancedbase.ui
	ui/propertywidget_distancebase.ui
	ui/propertywidget_pareffectbase.ui
	ui/propertywidget_flopbase.ui
	ui/propertywidget_fontfeaturesbase.ui
	ui/propertywidget_hyphenationbase.ui
	ui/propertywidget_optmarginsbase.ui
	ui/propertywidget_orphansbase.ui
	ui/propertywidget_pathtextbase.ui
	ui/propertywidget_textcolorbase.ui
	ui/recoverdialog.ui
	ui/replacecolors.ui
	ui/replaceonecolor.ui
	ui/resourcemanagerbase.ui
	ui/resourcemanagerlicensebase.ui
	ui/selectobjects.ui
	ui/shortcutwidget.ui
	ui/smcellstylewidget.ui
	ui/smcstylewidget.ui
	ui/smfontfeaturesbase.ui
	ui/smlinestylewidget.ui
	ui/smtablestylewidget.ui
	ui/smpstylewidget.ui
	ui/smreplacedia.ui
	ui/smstyleimport.ui
	ui/spiralvectorbase.ui
	ui/spiralwidgetbase.ui
	ui/stylemanager.ui
	ui/tablecolumnwidthsdialog.ui
	ui/tablerowheightsdialog.ui
	ui/tabpdfoptions.ui
	ui/transformdialogbase.ui
	ui/transparencypalette.ui
	ui/copypagetomasterpage.ui
	ui/createrange.ui
	ui/helpbrowser.ui
	ui/helpbrowsernavigation.ui
	ui/unicodesearch.ui
	ui/useprintermarginsdialog.ui
	ui/weldeditbase.ui
#	#{SCRIBUS_ITEMSETTERS_UI_SRC}
	${SCRIBUS_OSG_UI_SRC}
)

set(SCRIBUS_MOC_CLASSES
	actionmanager.h
	appmodehelper.h
	canvas.h
	canvasgesture.h
	canvasgesture_cellselect.h
	canvasgesture_columnresize.h
	canvasgesture_rowresize.h
	canvasgesture_rulermove.h
	canvasgesture_table.h
	canvasgesture_tableresize.h
	canvasmode.h
	canvasmode_create.h
	canvasmode_edit.h
	canvasmode_editarc.h
	canvasmode_editpolygon.h
	canvasmode_editspiral.h
	canvasmode_edittable.h
	canvasmode_editweldpoint.h
	canvasmode_eyedropper.h
	canvasmode_framelinks.h
	canvasmode_imageimport.h
	canvasmode_measurements.h
	canvasmode_normal.h
	canvasmode_objimport.h
	chartablemodel.h
	chartableview.h
	collect4output.h
	colorblind.h
	commonstrings.h
	deferredtask.h
	docinfo.h
	downloadmanager/scdlmgr.h
	downloadmanager/scdlthread.h
	fileloader.h
	filesearch.h
	filewatcher.h
	fontlistmodel.h
	guidesdelegate.h
	guidesmodel.h
	guidesview.h
	hyphenator.h
	iconmanager.h
	latexhelpers.h
	loadsaveplugin.h
	menumanager.h
	navigator.h
	notesstyles.h
	observable.h
	pageitem.h
	pageitem_arc.h
	pageitem_group.h
	pageitem_imageframe.h
	pageitem_latexframe.h
	pageitem_line.h
	pageitem_pathtext.h
	pageitem_polygon.h
	pageitem_polyline.h
	pageitem_regularpolygon.h
	pageitem_spiral.h
	pageitem_symbol.h
	pageitem_table.h
	pageitem_textframe.h
	pageitem_noteframe.h
	pageitempointer.h
	pdf_analyzer.h
	pdflib.h
	pdflib_core.h
	pluginmanager.h
	pp_proxy.h
	prefsmanager.h
	pslib.h
	qtiocompressor.h
	sampleitem.h
	scgtplugin.h
	schelptreemodel.h
	scimagecachedir.h
	scimagecachefile.h
	scimagecachemanager.h
	scmimedata.h
	scplugin.h
	scprintengine.h
	scraction.h
	scribus.h
	scribusapp.h
	scribuscore.h
	scribusdoc.h
	scribusview.h
	scribuswin.h
	selection.h
	selectionrubberband.h
	styleitem.h
	tocgenerator.h
	undogui.h
	undomanager.h
	upgradechecker.h
	urllauncher.h
	ui/about.h
	ui/aboutplugins.h
	ui/adjustcmsdialog.h
	ui/aligndistribute.h
	ui/alignselect.h
	ui/directionselect.h
	ui/annot.h
	ui/annota.h
	ui/applytemplatedialog.h
	ui/arcwidget.h
	ui/arcvectordialog.h
	ui/arrowchooser.h
	ui/autoform.h
	ui/autoformbuttongroup.h
	ui/basepointwidget.h
	ui/bookmarkpalette.h
	ui/bookmwin.h
	ui/buttonicon.h
	ui/charselect.h
	ui/charselectenhanced.h
	ui/charzoom.h
	ui/checkDocument.h
	ui/clockwidget.h
	ui/cmsprefs.h
	ui/cmykfw.h
	ui/collectforoutput_ui.h
	ui/colorchart.h
	ui/colorcombo.h
	ui/colorlistbox.h
	ui/colorlistmodel.h
	ui/colorsandfills.h
	ui/contextmenu.h
	ui/cpalette.h
	ui/cupsoptions.h
	ui/curvewidget.h
	ui/customfdialog.h
	ui/cxfimportdialog.h
	ui/dasheditor.h
	ui/dcolor.h
	ui/delpages.h
	ui/downloadspalette.h
	ui/editor.h
	ui/effectsdialog.h
	ui/extimageprops.h
	ui/filedialogeventcatcher.h
	ui/fontcombo.h
	ui/fontembeddingcombo.h
	ui/fontembeddingmodel.h
	ui/fontlistview.h
	ui/fontreplacedialog.h
	ui/gradientaddedit.h
	ui/gradienteditor.h
	ui/gradientvectordialog.h
	ui/gradientpreview.h
	ui/gtdialogs.h
	ui/gtfiledialog.h
	ui/guidemanager.h
	ui/helpbrowser.h
	ui/helpnavigation.h
	ui/hruler.h
	ui/hyask.h
	ui/imageinfodialog.h
	ui/inlinepalette.h
	ui/insertTable.h
	ui/insertaframe.h
	ui/inserttablecolumnsdialog.h
	ui/inserttablerowsdialog.h
	ui/inspage.h
	ui/javadocs.h
	ui/latexeditor.h
	ui/layers.h
	ui/linecombo.h
	ui/linkbutton.h
	ui/loremipsum.h
	ui/markinsert.h
	ui/mark2item.h
	ui/mark2mark.h
	ui/markanchor.h
	ui/marknote.h
	ui/marksmanager.h
	ui/markvariabletext.h
	ui/marginpresetlayout.h
	ui/marginwidget.h
	ui/newmarginwidget.h
	ui/measurements.h
	ui/mergedoc.h
	ui/missing.h
	ui/movepage.h
	ui/multipleduplicate.h
	ui/multiprogressdialog.h
	ui/newfile.h
	ui/newtemp.h
	ui/nftdialog.h
	ui/nftwidget.h
	ui/nodeeditpalette.h
	ui/notesstyleseditor.h
	ui/oneclick.h
	ui/outlinepalette.h
	ui/pageitemattributes.h
	ui/pagelayout.h
	ui/pagepalette.h
	ui/pagepalette_pages.h
	ui/pagepalette_masterpages.h
	ui/pagepalette_widgets.h
	ui/pagepropertiesdialog.h
	ui/pageselector.h
	ui/patternpropsdialog.h
	ui/pdfexportdialog.h
	ui/pdfversioncombo.h
	ui/pdfversionmodel.h
	ui/picsearch.h
	ui/picsearchoptions.h
	ui/picstatus.h
	ui/pluginmanagerprefsgui.h
	ui/polygonwidget.h
	ui/polyprops.h
	ui/preferencesdialog.h
	ui/prefs_colormanagement.h
	ui/prefs_display.h
	ui/prefs_documentinformation.h
	ui/prefs_documentsections.h
	ui/prefs_documentsetup.h
	ui/prefs_externaltools.h
	ui/prefs_fonts.h
	ui/prefs_guides.h
	ui/prefs_hyphenator.h
	ui/prefs_imagecache.h
	ui/prefs_keyboardshortcuts.h
	ui/prefs_miscellaneous.h
	ui/prefs_pagesizes.h
	ui/prefs_paths.h
	ui/prefs_pane.h
	ui/prefs_plugins.h
	ui/prefs_preflightverifier.h
	ui/prefs_printer.h
	ui/prefs_scrapbook.h
	ui/prefs_spelling.h
	ui/prefs_typography.h
	ui/prefs_userinterface.h
	ui/prefs_itemtools.h
	ui/prefs_operatortools.h
	ui/prefs_pdfexport.h
	ui/prefs_tableofcontents.h
	ui/prefs_documentitemattributes.h
	ui/prefspanel.h
	ui/preview.h
	ui/printdialog.h
	ui/propertiespalette.h
	ui/propertiespalette_group.h
	ui/propertiespalette_image.h
	ui/propertiespalette_line.h
	ui/propertiespalette_shadow.h
	ui/propertiespalette_shape.h
	ui/propertiespalette_table.h
	ui/propertiespalette_text.h
	ui/propertiespalette_utils.h
	ui/propertiespalette_xyz.h
	ui/propertywidget_advanced.h
	ui/propertywidget_distance.h
	ui/propertywidget_pareffect.h
	ui/propertywidget_flop.h
	ui/propertywidget_hyphenation.h
	ui/propertywidget_optmargins.h
	ui/propertywidget_orphans.h
	ui/propertywidget_pathtext.h
	ui/propertywidget_textcolor.h
	ui/propertywidget_fontfeatures.h
	ui/proptree.h
	ui/query.h
	ui/recoverdialog.h
	ui/replacecolors.h
	ui/replaceonecolor.h
	ui/resourcemanager.h
	ui/resourcemanagerlicense.h
	ui/rulermover.h
	ui/sccolorslider.h
	ui/sccombobox.h
	ui/scdockpalette.h
	ui/scescapecatcher.h
	ui/scfilewidget.h
	ui/scinputdialog.h
	ui/sclistwidgetdelegate.h
	ui/scmenu.h
	ui/scmessagebox.h
	ui/scmwmenumanager.h
	ui/scrapbookpalette.h
	ui/scrpalettebase.h
	ui/scrspinbox.h
	ui/sctablewidget.h
	ui/sctextbrowser.h
	ui/sctoolbar.h
	ui/sctreewidget.h
	ui/search.h
	ui/selectobjects.h
	ui/selfield.h
	ui/shadebutton.h
	ui/shortcutwidget.h
	ui/smalignselect.h
	ui/smcellstylewidget.h
	ui/smcellstyle.h
	ui/smcheckbox.h
	ui/smcolorcombo.h
	ui/smcstylewidget.h
	ui/smdirectionselect.h
	ui/smfontfeatures.h
	ui/smtablestylewidget.h
	ui/smfontcomboh.h
	ui/smlinestyle.h
	ui/smlinestylewidget.h
	ui/smpstylewidget.h
	ui/smradiobutton.h
	ui/smreplacedia.h
	ui/smsccombobox.h
	ui/smscrspinbox.h
	ui/smshadebutton.h
	ui/smspinbox.h
	ui/smlineedit.h
	ui/smstyleimport.h
	ui/smstyleselect.h
	ui/smtablestyle.h
	ui/smtabruler.h
	ui/smtextstyles.h
	ui/spalette.h
	ui/spiralvectordialog.h
	ui/spiralwidget.h
	ui/splash.h
	ui/storyeditor.h
	ui/stylemanager.h
	ui/styleselect.h
	ui/styleview.h
	ui/swatchcombo.h
	ui/symbolpalette.h
	ui/tablecolumnwidthsdialog.h
	ui/tablesideselector.h
	ui/tablerowheightsdialog.h
	ui/tabmanager.h
	ui/tabpdfoptions.h
	ui/tabruler.h
	ui/textpalette.h
	ui/transformdialog.h
	ui/transparencypalette.h
	tt/simpletreemodel/treemodel.h
	ui/copypagetomasterpagedialog.h
	ui/createrange.h
	ui/edittoolbar.h
	ui/filetoolbar.h
	ui/modetoolbar.h
	ui/pdftoolbar.h
	ui/viewtoolbar.h
	ui/unicodesearch.h
	ui/useprintermarginsdialog.h
	ui/vruler.h
	ui/weldedit.h
#	#{SCRIBUS_ITEMSETTERS_CLASS}
	${SCRIBUS_OSG_CLASS}
)

set(SCRIBUS_SOURCES
	actionmanager.cpp
	appmodehelper.cpp
	canvas.cpp
	canvasgesture_cellselect.cpp
	canvasgesture_columnresize.cpp
	canvasgesture_linemove.cpp
	canvasgesture_pan.cpp
	canvasgesture_rectselect.cpp
	canvasgesture_resize.cpp
	canvasgesture_rowresize.cpp
	canvasgesture_rulermove.cpp
	canvasgesture_table.cpp
	canvasgesture_tableresize.cpp
	canvasmode.cpp
	canvasmode_copyproperties.cpp
	canvasmode_create.cpp
	canvasmode_drawbezier.cpp
	canvasmode_drawcalligraphic.cpp
	canvasmode_drawfreehand.cpp
	canvasmode_eyedropper.cpp
	canvasmode_framelinks.cpp
	canvasmode_imageimport.cpp
	canvasmode_edit.cpp
	canvasmode_editarc.cpp
	canvasmode_editgradient.cpp
	canvasmode_editmeshgradient.cpp
	canvasmode_editmeshpatch.cpp
	canvasmode_editpolygon.cpp
	canvasmode_editspiral.cpp
	canvasmode_edittable.cpp
	canvasmode_editweldpoint.cpp
	canvasmode_magnifier.cpp
	canvasmode_measurements.cpp
	canvasmode_nodeedit.cpp
	canvasmode_normal.cpp
	canvasmode_objimport.cpp
	canvasmode_panning.cpp
	canvasmode_rotate.cpp
	cellarea.cpp
	chartablemodel.cpp
	chartableview.cpp
	cmserrorhandling.cpp
	cmsettings.cpp
	collapsedtablepainter.cpp
	collapsedtablepainterex.cpp
	collect4output.cpp
	colorblind.cpp
	colorsetmanager.cpp
	commonstrings.cpp
	deferredtask.cpp
	docinfo.cpp
	documentchecker.cpp
	documentinformation.cpp
	exif.cpp
	fileloader.cpp
	filesearch.cpp
	filewatcher.cpp
	fontlistmodel.cpp
	fpoint.cpp
	fpointarray.cpp
	gtaction.cpp
	gtfont.cpp
	gtframestyle.cpp
	gtgettext.cpp
	gtmeasure.cpp
	gtparagraphstyle.cpp
	gtstyle.cpp
	gtwriter.cpp
	guidemanagercore.cpp
	guidesdelegate.cpp
	guidesmodel.cpp
	guidesview.cpp
	hyphenator.cpp
	iconmanager.cpp
	ioapi.c
	KarbonCurveFit.cpp
	langdef.cpp
	langmgr.cpp
	latexhelpers.cpp
	loadsaveplugin.cpp
	localemgr.cpp
	marks.cpp
	menumanager.cpp
	mesh.cpp
	navigator.cpp
	numeration.cpp
	observable.cpp
	nftrcreader.cpp
	nftsettings.cpp
	nfttemplate.cpp
	nodeeditcontext.cpp
	notesstyles.cpp
	pageitem.cpp
	pageitem_arc.cpp
	pageitem_group.cpp
	pageitem_imageframe.cpp
	pageitem_latexframe.cpp
	pageitem_line.cpp
	pageitem_pathtext.cpp
	pageitem_polygon.cpp
	pageitem_polyline.cpp
	pageitem_regularpolygon.cpp
	pageitem_spiral.cpp
	pageitem_symbol.cpp
	pageitem_table.cpp
	pageitem_textframe.cpp
	pageitem_noteframe.cpp
	pageitemiterator.cpp
	pageitempointer.cpp
	pagesize.cpp
	pdf_analyzer.cpp
	pdflib.cpp
	pdflib_core.cpp
	pdfoptions.cpp
	pdfoptionsio.cpp
	pdfwriter.cpp
	pluginmanager.cpp
	pp_proxy.cpp
	prefscontext.cpp
	prefsfile.cpp
	prefsmanager.cpp
	prefsreader.cpp
	prefstable.cpp
	pslib.cpp
	qtiocompressor.cpp
	rawimage.cpp
	rc4.c
	sampleitem.cpp
	scclocale.cpp
	sccolor.cpp
	sccolorengine.cpp
	sccolorshade.cpp
	scdocoutput.cpp
	scdocoutput_ps2.cpp
	scdomelement.cpp
	scfonts.cpp
	scgtplugin.cpp
	schelptreemodel.cpp
	scimage.cpp
	scimagecacheproxy.cpp
	scimagecachedir.cpp
	scimagecachefile.cpp
	scimagecachemanager.cpp
	scimagecachewriteaction.cpp
	scimagestructs.cpp
	sclayer.cpp
	sclockedfile.cpp
	scmimedata.cpp
	scpage.cpp
	scpageoutput.cpp
	scpageoutput_ps2.cpp
	scpainter.cpp
	scpainterex_ps2.cpp
	scpainterexbase.cpp
	scpaths.cpp
	scpattern.cpp
	scplugin.cpp
	scpreview.cpp
	scprintengine_ps.cpp
	scraction.cpp
	scribus.cpp
	scribusXml.cpp
	scribusapp.cpp
	scribuscore.cpp
	scribusdoc.cpp
	scribusview.cpp
	scribuswin.cpp
	scslainforeader.cpp
	scstreamfilter.cpp
	scstreamfilter_ascii85.cpp
	scstreamfilter_flate.cpp
	scstreamfilter_jpeg.cpp
	scstreamfilter_rc4.cpp
	sctextstream.cpp
	sctextstruct.cpp
	scxmlstreamreader.cpp
	selection.cpp
	selectionrubberband.cpp
	serializer.cpp
	styleitem.cpp
	tableborder.cpp
	tablecell.cpp
	tableutils.cpp
	textwriter.cpp
	tocgenerator.cpp
	transaction.cpp
	undogui.cpp
	undomanager.cpp
	undoobject.cpp
	undostack.cpp
	undostate.cpp
	undotransaction.cpp
	units.cpp
	updatemanager.cpp
	upgradechecker.cpp
	urllauncher.cpp
	util.cpp
	util_color.cpp
	util_debug.cpp
	util_file.cpp
	util_formats.cpp
	util_ghostscript.cpp
	util_layer.cpp
	util_math.cpp
	util_os.cpp
	util_printer.cpp
	util_text.cpp
	vgradient.cpp
	vgradientex.cpp
	downloadmanager/scdlmgr.cpp
	downloadmanager/scdlthread.cpp
	imagedataloaders/scimgdataloader.cpp
	imagedataloaders/scimgdataloader_gimp.cpp
	imagedataloaders/scimgdataloader_jpeg.cpp
	imagedataloaders/scimgdataloader_kra.cpp
	imagedataloaders/scimgdataloader_ora.cpp
	imagedataloaders/scimgdataloader_pdf.cpp
	imagedataloaders/scimgdataloader_pgf.cpp
	imagedataloaders/scimgdataloader_pict.cpp
	imagedataloaders/scimgdataloader_ps.cpp
	imagedataloaders/scimgdataloader_psd.cpp
	imagedataloaders/scimgdataloader_qt.cpp
	imagedataloaders/scimgdataloader_tiff.cpp
	imagedataloaders/scimgdataloader_wpg.cpp
	palettes/cxfcolor.cpp
	palettes/cxfcolorspecification.cpp
	palettes/cxfdocument.cpp
	palettes/cxfobject.cpp
	palettes/cxfmeasurementspec.cpp
	palettes/cxftristimulusspec.cpp
	palettes/cxfutils.cpp
	palettes/paletteloader.cpp
	palettes/paletteloader_adobe_acb.cpp
	palettes/paletteloader_adobe_aco.cpp
	palettes/paletteloader_adobe_ase.cpp
	palettes/paletteloader_autocad_acb.cpp
	palettes/paletteloader_cxf.cpp
	palettes/paletteloader_ps.cpp
	palettes/paletteloader_sk1.cpp
	palettes/paletteloader_swatchbook.cpp
	third_party/fparser/fpoptimizer.cc
	third_party/fparser/fparser.cc
	ui/about.cpp
	ui/aboutplugins.cpp
	ui/adjustcmsdialog.cpp
	ui/aligndistribute.cpp
	ui/alignselect.cpp
	ui/directionselect.cpp
	ui/annot.cpp
	ui/annota.cpp
	ui/applytemplatedialog.cpp
	ui/arcwidget.cpp
	ui/arcvectordialog.cpp
	ui/arrowchooser.cpp
	ui/autoform.cpp
	ui/autoformbuttongroup.cpp
	ui/basepointwidget.cpp
	ui/bookmarkpalette.cpp
	ui/bookmwin.cpp
	ui/buttonicon.cpp
	ui/charselect.cpp
	ui/charselectenhanced.cpp
	ui/charzoom.cpp
	ui/checkDocument.cpp
	ui/clockwidget.cpp
	ui/cmsprefs.cpp
	ui/cmykfw.cpp
	ui/collectforoutput_ui.cpp
	ui/colorchart.cpp
	ui/colorcombo.cpp
	ui/colorlistbox.cpp
	ui/colorlistmodel.cpp
	ui/colorsandfills.cpp
	ui/contextmenu.cpp
	ui/cpalette.cpp
	ui/cupsoptions.cpp
	ui/curvewidget.cpp
	ui/customfdialog.cpp
	ui/cxfimportdialog.cpp
	ui/dasheditor.cpp
	ui/dcolor.cpp
	ui/delpages.cpp
	ui/downloadspalette.cpp
	ui/editor.cpp
	ui/effectsdialog.cpp
	ui/extimageprops.cpp
	ui/filedialogeventcatcher.cpp
	ui/fontcombo.cpp
	ui/fontembeddingcombo.cpp
	ui/fontembeddingmodel.cpp
	ui/fontlistview.cpp
	ui/fontreplacedialog.cpp
	ui/gradientaddedit.cpp
	ui/gradienteditor.cpp
	ui/gradientvectordialog.cpp
	ui/gradientpreview.cpp
	ui/gtdialogs.cpp
	ui/gtfiledialog.cpp
	ui/guidemanager.cpp
	ui/helpbrowser.cpp
	ui/hruler.cpp
	ui/hyask.cpp
	ui/imageinfodialog.cpp
	ui/inlinepalette.cpp
	ui/insertTable.cpp
	ui/insertaframe.cpp
	ui/inserttablecolumnsdialog.cpp
	ui/inserttablerowsdialog.cpp
	ui/inspage.cpp
	ui/javadocs.cpp
	ui/latexeditor.cpp
	ui/layers.cpp
	ui/linecombo.cpp
	ui/linkbutton.cpp
	ui/loremipsum.cpp
	ui/mark2item.cpp
	ui/mark2mark.cpp
	ui/markanchor.cpp
	ui/marknote.cpp
	ui/marksmanager.cpp
	ui/markvariabletext.cpp
	ui/marginpresetlayout.cpp
	ui/marginwidget.cpp
	ui/newmarginwidget.cpp
	ui/mergedoc.cpp
	ui/missing.cpp
	ui/movepage.cpp
	ui/multipleduplicate.cpp
	ui/multiprogressdialog.cpp
	ui/measurements.cpp
	ui/newfile.cpp
	ui/newtemp.cpp
	ui/nftdialog.cpp
	ui/nftwidget.cpp
	ui/nodeeditpalette.cpp
	ui/notesstyleseditor.cpp
	ui/oneclick.cpp
	ui/outlinepalette.cpp
	ui/pageitemattributes.cpp
	ui/pagelayout.cpp
	ui/pagepalette.cpp
	ui/pagepalette_pages.cpp
	ui/pagepalette_masterpages.cpp
	ui/pagepalette_widgets.cpp
	ui/pagepropertiesdialog.cpp
	ui/pageselector.cpp
	ui/patternpropsdialog.cpp
	ui/pdfexportdialog.cpp
	ui/pdfversioncombo.cpp
	ui/pdfversionmodel.cpp
	ui/picsearch.cpp
	ui/picsearchoptions.cpp
	ui/picstatus.cpp
	ui/pluginmanagerprefsgui.cpp
	ui/polygonwidget.cpp
	ui/polyprops.cpp
	ui/preferencesdialog.cpp
	ui/prefs_colormanagement.cpp
	ui/prefs_display.cpp
	ui/prefs_documentinformation.cpp
	ui/prefs_documentsections.cpp
	ui/prefs_documentsetup.cpp
	ui/prefs_externaltools.cpp
	ui/prefs_fonts.cpp
	ui/prefs_guides.cpp
	ui/prefs_hyphenator.cpp
	ui/prefs_imagecache.cpp
	ui/prefs_keyboardshortcuts.cpp
	ui/prefs_miscellaneous.cpp
	ui/prefs_pagesizes.cpp
	ui/prefs_paths.cpp
	ui/prefs_plugins.cpp
	ui/prefs_preflightverifier.cpp
	ui/prefs_printer.cpp
	ui/prefs_scrapbook.cpp
	ui/prefs_spelling.cpp
	ui/prefs_typography.cpp
	ui/prefs_userinterface.cpp
	ui/prefs_itemtools.cpp
	ui/prefs_operatortools.cpp
	ui/prefs_pdfexport.cpp
	ui/prefs_tableofcontents.cpp
	ui/prefs_documentitemattributes.cpp
	ui/prefspanel.cpp
	ui/preview.cpp
	ui/printdialog.cpp
	ui/propertiespalette.cpp
	ui/propertiespalette_image.cpp
	ui/propertiespalette_group.cpp
	ui/propertiespalette_line.cpp
	ui/propertiespalette_shadow.cpp
	ui/propertiespalette_shape.cpp
	ui/propertiespalette_table.cpp
	ui/propertiespalette_text.cpp
	ui/propertiespalette_utils.cpp
	ui/propertiespalette_xyz.cpp
	ui/propertywidget_advanced.cpp
	ui/propertywidget_distance.cpp
	ui/propertywidget_pareffect.cpp
	ui/propertywidget_flop.cpp
	ui/propertywidget_hyphenation.cpp
	ui/propertywidget_optmargins.cpp
	ui/propertywidget_orphans.cpp
	ui/propertywidget_pathtext.cpp
	ui/propertywidget_textcolor.cpp
	ui/propertywidget_fontfeatures.cpp
	ui/propertywidgetbase.cpp
	ui/proptree.cpp
	ui/query.cpp
	ui/recoverdialog.cpp
	ui/replacecolors.cpp
	ui/replaceonecolor.cpp
	ui/resourcemanager.cpp
	ui/resourcemanagerlicense.cpp
	ui/rulermover.cpp
	ui/sccolorslider.cpp
	ui/sccombobox.cpp
	ui/scdockpalette.cpp
	ui/scescapecatcher.cpp
	ui/scfilewidget.cpp
	ui/scinputdialog.cpp
	ui/sclistwidgetdelegate.cpp
	ui/scmenu.cpp
	ui/scmessagebox.cpp
	ui/scmwmenumanager.cpp
	ui/scrapbookpalette.cpp
	ui/scresizecursor.cpp
	ui/scrpalettebase.cpp
	ui/scrspinbox.cpp
	ui/sctablewidget.cpp
	ui/sctextbrowser.cpp
	ui/sctoolbar.cpp
	ui/sctreewidget.cpp
	ui/search.cpp
	ui/selectobjects.cpp
	ui/selfield.cpp
	ui/shadebutton.cpp
	ui/shortcutwidget.cpp
	ui/smalignselect.cpp
	ui/smcellstylewidget.cpp
	ui/smcellstyle.cpp
	ui/smcheckbox.cpp
	ui/smcolorcombo.cpp
	ui/smcstylewidget.cpp
	ui/smdirectionselect.cpp
	ui/smfontfeatures.cpp
	ui/smfontcomboh.cpp
	ui/smlinestyle.cpp
	ui/smlinestylewidget.cpp
	ui/smpstylewidget.cpp
	ui/smradiobutton.cpp
	ui/smreplacedia.cpp
	ui/smsccombobox.cpp
	ui/smscrspinbox.cpp
	ui/smshadebutton.cpp
	ui/smspinbox.cpp
	ui/smlineedit.cpp
	ui/smstyleimport.cpp
	ui/smstyleselect.cpp
	ui/smtablestylewidget.cpp
	ui/smtablestyle.cpp
	ui/smtabruler.cpp
	ui/smtextstyles.cpp
	ui/spalette.cpp
	ui/spiralvectordialog.cpp
	ui/spiralwidget.cpp
	ui/splash.cpp
	ui/storyeditor.cpp
	ui/stylemanager.cpp
	ui/styleselect.cpp
	ui/styleview.cpp
	ui/swatchcombo.cpp
	ui/symbolpalette.cpp
	ui/tablecolumnwidthsdialog.cpp
	ui/tablerowheightsdialog.cpp
	ui/tablesideselector.cpp
	ui/tabmanager.cpp
	ui/tabpdfoptions.cpp
	ui/tabruler.cpp
	ui/textpalette.cpp
	ui/transformdialog.cpp
	ui/transparencypalette.cpp
	tt/simpletreemodel/treeitem.cpp
	tt/simpletreemodel/treemodel.cpp
	ui/copypagetomasterpagedialog.cpp
	ui/createrange.cpp
	ui/edittoolbar.cpp
	ui/filetoolbar.cpp
	ui/helpnavigation.cpp
	ui/modetoolbar.cpp
	ui/pdftoolbar.cpp
	ui/viewtoolbar.cpp
	ui/unicodesearch.cpp
	ui/vruler.cpp
	ui/useprintermarginsdialog.cpp
	ui/weldedit.cpp
#	#{SCRIBUS_ITEMSETTERS_SRC}
	${SCRIBUS_OSG_SRC}
	${SCRIBUS_GMAGICK_SRC}
)

if(NOT HAVE_HYPHEN)
	include_directories(third_party/hyphen)
	set(SCRIBUS_SOURCES
	${SCRIBUS_SOURCES}
	third_party/hyphen/hnjalloc.c
	third_party/hyphen/hyphen.c
	)
endif()



if(WIN32)
	set(SCRIBUS_MOC_WIN32_ONLY_CLASSES scprintengine_gdi.h)
	set(SCRIBUS_WIN32_ONLY_SOURCES
		scpainterex_cairo.cpp
		scprintengine_gdi.cpp
		)
	set(SCRIBUS_MAIN_CPP main_win32.cpp)
else()
	set(SCRIBUS_MOC_WIN32_ONLY_CLASSES)
	set(SCRIBUS_WIN32_ONLY_SOURCES)
	set(SCRIBUS_MAIN_CPP main.cpp)
endif()

QT5_WRAP_UI(SCRIBUS_UI_SOURCES ${SCRIBUS_UI_SRC} )

QT5_WRAP_CPP(SCRIBUS_MOC_SOURCES
	${SCRIBUS_MOC_CLASSES}
	${SCRIBUS_MOC_WIN32_ONLY_CLASSES}
	)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}/colormgmt
	${CMAKE_CURRENT_BINARY_DIR}/desaxe
	${CMAKE_CURRENT_BINARY_DIR}/fonts
	${CMAKE_CURRENT_BINARY_DIR}/styles
	${CMAKE_CURRENT_BINARY_DIR}/text
	${CMAKE_CURRENT_BINARY_DIR}/third_party/wpg
	${CMAKE_CURRENT_BINARY_DIR}/third_party/pgf
	${CMAKE_CURRENT_BINARY_DIR}/third_party/rtf-qt
	${CMAKE_CURRENT_BINARY_DIR}/third_party/zip
	${HARFBUZZ_LIBRARY_DIRS}
	${ICU_LIBRARY_DIRS}
)

if(WITH_TESTS)
	link_directories( ${CMAKE_CURRENT_BINARY_DIR}/tests )
endif()

if(HAVE_OSG)
	link_directories( ${CMAKE_CURRENT_BINARY_DIR}/third_party/prc )
endif ()

if(HAVE_BOOST)
	link_directories( ${CMAKE_CURRENT_BINARY_DIR}/third_party/lib2geom )
endif ()

set(SCRIBUS_COLORMGMT_LIB "scribus_colormgmt_lib")
set(SCRIBUS_TEXT_LIB "scribus_text_lib")
if(WITH_TESTS)
	set(SCRIBUS_TESTS_LIB "scribus_tests_lib")
else()
	set(SCRIBUS_TESTS_LIB "")
endif()
set(SCRIBUS_STYLES_LIB "scribus_styles_lib")
set(SCRIBUS_FONTS_LIB "scribus_fonts_lib")
set(SCRIBUS_DESAXE_LIB "scribus_desaxe_lib")
if(HAVE_OSG)
	set(SCRIBUS_PRC_LIB "scribus_prc_lib")
endif ()
if(HAVE_BOOST)
	set(SCRIBUS_2GEOM_LIB "scribus_2geom_lib")
endif ()
set(SCRIBUS_WPG_LIB "scribus_wpg_lib")
set(SCRIBUS_PGF_LIB "scribus_pgf_lib")
set(SCRIBUS_ZIP_LIB "scribus_zip_lib")
set(SCRIBUS_RTF_LIB "scribus_rtf_lib")

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${HARFBUZZ_INCLUDE_DIRS}
)

if (GMAGICK_FOUND)
	message(STATUS "GMAGICK: Libs: ${GMAGICK_LDFLAGS}, Inc: ${GMAGICK_INCLUDE_DIRS}, Ldflags: ${GMAGICK_LDFLAGS}")
	include_directories(
		BEFORE ${GMAGICK_INCLUDE_DIRS}
	)
	link_directories(
		${GMAGICK_LIBRARY}
	)
endif()

if(HAVE_PODOFO)
	if(OPENSSL_FOUND)
		message(STATUS "Including OpenSSL files for PoDoFo >= 0.9.5")
		include_directories(
			${OPENSSL_INCLUDE_DIR}
		)
	else()
#		message(STATUS "Not including OpenSSL files because this system's PoDoFo does not require them")
	endif()
	include_directories(
		${LIBPODOFO_INCLUDE_DIR}
	)

endif()

if(HAVE_HYPHEN)
	target_link_libraries(${EXE_NAME}
	${HYPHEN_LIBRARY}
	)
endif()


if(GESTURE_FRAME_PREVIEW)
	 message(STATUS "Enable gesture frame preview [experimental]")
	 set(GESTURE_FRAME_PREVIEW_SOURCES	pageitempreview.cpp)
	 add_definitions(-DGESTURE_FRAME_PREVIEW)
endif()

if(APPLEBUNDLE)
	set(EXE_NAME "Scribus${TAG_VERSION}" CACHE TYPE STRING)
else()
	set(EXE_NAME "scribus${TAG_VERSION}" CACHE TYPE STRING)
endif()

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 OLD)
endif()

if(HAVE_OSG)
	add_executable(${EXE_NAME}
		${SCRIBUS_MOC_SOURCES}
		${SCRIBUS_SOURCES}
		${SCRIBUS_MAIN_CPP}
		${SCRIBUS_UI_SOURCES}
		${SCRIBUS_TEXT_SOURCES}
		${SCRIBUS_STYLES_SOURCES}
		${SCRIBUS_COLORMGMT_SOURCES}
		${SCRIBUS_FONTS_SOURCES}
		${SCRIBUS_DESAXE_SOURCES}
		${SCRIBUS_WIN32_ONLY_SOURCES}
		${GESTURE_FRAME_PREVIEW_SOURCES}
		${SCRIBUS_PRC_SOURCES}
	)
else()
	add_executable(${EXE_NAME}
		${SCRIBUS_MOC_SOURCES}
		${SCRIBUS_SOURCES}
		${SCRIBUS_MAIN_CPP}
		${SCRIBUS_UI_SOURCES}
		${SCRIBUS_TEXT_SOURCES}
		${SCRIBUS_STYLES_SOURCES}
		${SCRIBUS_COLORMGMT_SOURCES}
		${SCRIBUS_FONTS_SOURCES}
		${SCRIBUS_DESAXE_SOURCES}
		${SCRIBUS_WIN32_ONLY_SOURCES}
		${GESTURE_FRAME_PREVIEW_SOURCES}
	)
endif ()

if(WIN32)
	set_target_properties(${EXE_NAME}
		PROPERTIES
		COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
		ENABLE_EXPORTS TRUE
	)
	set(WIN32_ONLY_LIBS mscms.lib)
else()
	set(WIN32_ONLY_LIBS)
endif()

##TEST FIX FOR 6897
if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
# Use the -E / --export-dynamic flag to ensure that symbols in the Scribus
# binary are in the public symbol table for plugins. The symbols must also
# be set as SCRIBUS_API if we're building with -fvisibility=hidden .
# Should fix bug 6897 .
	set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS -Wl,-E)
endif ()
if (GCC AND NOT WIN32)
# Use the -E / --export-dynamic flag to ensure that symbols in the Scribus
# binary are in the public symbol table for plugins. The symbols must also
# be set as SCRIBUS_API if we're building with -fvisibility=hidden .
# Should fix bug 6897 .
	set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS -Wl,-E)
endif ()
##


#Set up libraries to link with all found packages
include_directories(${CAIRO_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} ${CAIRO_LIBRARIES})
link_directories(${PREFIX_CAIRO_LIB})
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${PREFIX_CAIRO_LIB}")

message("RPATH: ${CMAKE_INSTALL_RPATH}")

target_link_libraries(${EXE_NAME}
	${Qt5Core_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${Qt5Gui_LIBRARIES}
	${Qt5Xml_LIBRARIES}
	${Qt5Network_LIBRARIES}
	${Qt5OpenGL_LIBRARIES}
	${Qt5PrintSupport_LIBRARIES}
#	${Qt5Quick_LIBRARIES}
	${CUPS_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${HARFBUZZ_LIBRARIES}
	${ICU_LIBRARIES}
	${FONTCONFIG_LIBRARIES}
	${LIBXML2_LIBRARIES}
	${PYTHON_LIBRARIES}
	${TIFF_LIBRARIES}
	${JPEG_LIBRARIES}
	${ZLIB_LIBRARIES}
	${WIN32_ONLY_LIBS}
	${GMAGICK_LIBRARIES}
)

if (NOT WIN32 AND NOT HAIKU)
	target_link_libraries(${EXE_NAME}
		${CUPS_LIBRARIES}
	)
endif ()

# required to resolve symbols from libdl when ld is called with --no-add-needed
# #9310 for the OpenBSD addition
if(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND NOT HAIKU)
	target_link_libraries(${EXE_NAME} dl)
endif()

# #9311 for the OpenBSD addition
if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	target_link_libraries(${EXE_NAME} util execinfo)
endif()

if(HAIKU)
	target_link_libraries(${EXE_NAME} execinfo)
endif(HAIKU)

if(WIN32)
	target_link_libraries(${EXE_NAME} ${QT_QTMAIN_LIBRARY})
endif()

if(APPLE)
	target_link_libraries(${EXE_NAME}
	"-undefined dynamic_lookup"
	"-framework CoreFoundation" )
endif()

if (WANT_SVNVERSION)
	add_dependencies(${EXE_NAME} svnheader)
endif ()

target_link_libraries(${EXE_NAME}
	${SCRIBUS_COLORMGMT_LIB}
	${SCRIBUS_DESAXE_LIB}
	${SCRIBUS_TEXT_LIB}
	${SCRIBUS_STYLES_LIB}
	${SCRIBUS_FONTS_LIB}
	${SCRIBUS_WPG_LIB}
	${SCRIBUS_PGF_LIB}
	${SCRIBUS_ZIP_LIB}
	${SCRIBUS_RTF_LIB}
	${LCMS2_LIBRARIES}
)

if(WITH_TESTS)
	target_link_libraries(${EXE_NAME}
		${QT_QTTEST_LIBRARY}
		${SCRIBUS_TESTS_LIB}
	)
endif()

if(HAVE_PODOFO)
	target_link_libraries(${EXE_NAME}
		${LIBPODOFO_LIBRARY}
	)
endif()

if(HAVE_OSG)
	target_link_libraries(${EXE_NAME}
		${OPENTHREADS_LIBRARY}
		${OSG_LIBRARY}
		${OSGGA_LIBRARY}
		${OSGUTIL_LIBRARY}
		${OSGDB_LIBRARY}
		${OSGVIEWER_LIBRARY}
		${OSGTEXT_LIBRARY}
		${SCRIBUS_PRC_LIB}
	)
endif()

if (HAVE_BOOST)
	target_link_libraries(${EXE_NAME}
		${SCRIBUS_2GEOM_LIB}
	)
endif()

# Now build plugins

set(PLUGIN_LIBRARIES
	${Qt5Core_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${Qt5Gui_LIBRARIES}
	${Qt5Xml_LIBRARIES}
	${Qt5Network_LIBRARIES}
	${Qt5OpenGL_LIBRARIES}
	${LIBXML2_LIBRARIES}
	${ZLIB_LIBRARIES}
)

if(WIN32)
	# IF your plugin needs more than this, please link them privately
	# in the CMakeLists.txt for your plugin.
	set(PLUGIN_LIBRARIES
#		${QT_LIBRARIES} part of #7210
		${PLUGIN_LIBRARIES}
		${EXE_NAME}
	)
endif()
if(APPLE)
	set(PLUGIN_LIBRARIES
		${PLUGIN_LIBRARIES}
		"-undefined dynamic_lookup"
	)
endif()
message(STATUS ${PLUGIN_LIBRARIES})

add_subdirectory(plugins)

#do we want a lib suffix on osx? doubt it
if(APPLEBUNDLE)
	install(DIRECTORY DESTINATION MacOS)
	install(TARGETS ${EXE_NAME}
	RUNTIME DESTINATION MacOS
	LIBRARY DESTINATION lib
	)
#	add_custom_command(TARGET ${EXE_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} ARGS -Dinput_file=${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.app/Contents/MacOS/${EXE_NAME} -P ${CMAKE_MODULE_PATH}/ScribusIngestOSXBundleLibraries.cmake
#	)
#message(STATUS "Installing apple bundle")

#	set(APPS "${APPLE_APP_DIR}")
#	set(DIRS ${QT_LIBRARY_DIRS})
#	install(CODE "
#		file(GLOB_RECURSE QTPLUGINS
#		  \"\${CMAKE_INSTALL_PREFIX}/${LIBDIR}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#		include(BundleUtilities)
#		fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#		" COMPONENT Runtime)


else()
	if(WIN32)
		install(TARGETS ${EXE_NAME}
			RUNTIME DESTINATION .
			LIBRARY DESTINATION ${LIB_DIR_NAME}
		)
	else()
		install(TARGETS ${EXE_NAME}
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION ${LIB_DIR_NAME}
		)
	endif()
endif()

#Install our header files, selected from all existing dirs, excluding unwanted directories and svn files
if(WANT_HEADERINSTALL)
	message(STATUS "Source header files will be installed")
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${INCLUDEDIR}
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "dicts" EXCLUDE
		PATTERN "doc" EXCLUDE
		PATTERN "dtd" EXCLUDE
		PATTERN "editorconfig" EXCLUDE
		PATTERN "icon*" EXCLUDE
		PATTERN "manpages" EXCLUDE
		PATTERN "old" EXCLUDE
		PATTERN ".svn" EXCLUDE
	)
else()
	message(STATUS "No source header files will be installed")
endif()

#Install our stylesheet
install(FILES
	scribus.css
	DESTINATION ${LIBDIR}
)
