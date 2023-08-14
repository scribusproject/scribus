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


include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${HARFBUZZ_INCLUDE_DIRS}
	${HARFBUZZ_SUBSET_INCLUDE_DIRS}
)

include(api/CMakeLists.txt)
include(colormgmt/CMakeLists.txt)
include(desaxe/CMakeLists.txt)
include(fonts/CMakeLists.txt)
include(styles/CMakeLists.txt)
include(text/CMakeLists.txt)

add_subdirectory(dtd)
add_subdirectory(ui/qml)
if(WITH_TESTS)
	add_subdirectory(tests)
endif()
if (WANT_DESIGNER)
	add_subdirectory(designer)
endif ()
add_subdirectory(third_party)

if(HAVE_OSG)
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

set(SCRIBUS_SOURCES
	actionmanager.cpp
	actionsearch.cpp
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
	pdfversion.cpp
	pdfwriter.cpp
	pluginmanager.cpp
	pp_proxy.cpp
	prefscontext.cpp
	prefsfile.cpp
	prefsmanager.cpp
	prefsreader.cpp
	prefstable.cpp
	printpreviewcreator.cpp
	printpreviewcreator_pdf.cpp
	printpreviewcreator_ps.cpp
	printpreviewcreatorfactory.cpp
	pslib.cpp
	qtiocompressor.cpp
	rawimage.cpp
	rc4.c
	sampleitem.cpp
	scclocale.cpp
	sccolor.cpp
	sccolorengine.cpp
	sccolorshade.cpp
	sccolorstructs.cpp
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
	scprintengine.cpp
	scprintengine_pdf.cpp
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
	storyloader.cpp
	styleitem.cpp
	tableborder.cpp
	tablecell.cpp
	tableutils.cpp
	textnote.cpp
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
	imagedataloaders/scimgdataloader_png.cpp
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
	ui/actionsearchdialog.cpp
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
	ui/cmykfw.cpp
	ui/collectforoutput_ui.cpp
	ui/colorchart.cpp
	ui/colorcombo.cpp
	ui/colorlistbox.cpp
	ui/colorlistmodel.cpp
	ui/colorpalette.cpp
	ui/colorsandfills.cpp
	ui/contentpalette.cpp
	ui/contentpalette_default.cpp
	ui/contentpalette_page.cpp
	ui/contextmenu.cpp
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
	ui/markinsert.cpp
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
	ui/newdocdialog.cpp
	ui/newtemp.cpp
	ui/nftdialog.cpp
	ui/nftwidget.cpp
	ui/nodeeditpalette.cpp
	ui/notesstyleseditor.cpp
	ui/numformatcombo.cpp
	ui/outlinepalette.cpp
	ui/outputpreview_pdf.cpp
	ui/outputpreview_ps.cpp
	ui/outputpreviewbase.cpp
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
	ui/polygonwidget.cpp
	ui/polyprops.cpp
	ui/preferencesdialog.cpp
	ui/prefs_colormanagement.cpp
	ui/prefs_display.cpp
	ui/prefs_documentinformation.cpp
	ui/prefs_documentsections.cpp
	ui/prefs_documentsetup.cpp
	ui/prefs_experimental.cpp
	ui/prefs_externaltools.cpp
	ui/prefs_fonts.cpp
	ui/prefs_guides.cpp
	ui/prefs_hyphenator.cpp
	ui/prefs_imagecache.cpp
	ui/prefs_keyboardshortcuts.cpp
	ui/prefs_miscellaneous.cpp
	ui/prefs_pagesizes.cpp
	ui/prefs_pane.cpp
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
	ui/printdialog.cpp
	ui/printlanguagecombo.cpp
	ui/printpreview.cpp
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
	ui/scdialog.cpp
	ui/scdockpalette.cpp
	ui/scescapecatcher.cpp
	ui/scfilewidget.cpp
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
	ui/smnumformatcombo.cpp
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
	ui/spiralvectordialog.cpp
	ui/spiralwidget.cpp
	ui/splash.cpp
	ui/storyeditor.cpp
	ui/stylecombos.cpp
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
	set(SCRIBUS_WIN32_ONLY_SOURCES
		printpreviewcreator_gdi.cpp
		scpainterex_cairo.cpp
		scprintengine_gdi.cpp
		)
	set(SCRIBUS_MAIN_CPP main_win32.cpp)
else()
	set(SCRIBUS_WIN32_ONLY_SOURCES)
	set(SCRIBUS_MAIN_CPP main.cpp)
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

set_property(SOURCE third_party/fparser/file.hh PROPERTY SKIP_AUTOGEN ON)
set_property(SOURCE third_party/fparser/fparser.hh PROPERTY SKIP_AUTOGEN ON)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}/third_party/wpg
	${CMAKE_CURRENT_BINARY_DIR}/third_party/pgf
	${CMAKE_CURRENT_BINARY_DIR}/third_party/rtf-qt
	${CMAKE_CURRENT_BINARY_DIR}/third_party/zip
	${HARFBUZZ_LIBRARY_DIRS}
	${HARFBUZZ_SUBSET_LIBRARY_DIRS}
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

if(WITH_TESTS)
	set(SCRIBUS_TESTS_LIB "scribus_tests_lib")
else()
	set(SCRIBUS_TESTS_LIB "")
endif()
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
	target_link_libraries(${EXE_NAME} PRIVATE
	${HYPHEN_LIBRARY}
	)
endif()


if(GESTURE_FRAME_PREVIEW)
	 message(STATUS "Enable gesture frame preview [experimental]")
	 set(GESTURE_FRAME_PREVIEW_SOURCES	pageitempreview.cpp)
	 add_definitions(-DGESTURE_FRAME_PREVIEW)
endif()

if(APPLEBUNDLE)
	set(EXE_NAME "Scribus${TAG_VERSION}" CACHE STRING "Scribus version")
else()
	set(EXE_NAME "scribus${TAG_VERSION}" CACHE STRING "Scribus version")
endif()

#if(COMMAND cmake_policy)
#	cmake_policy(SET CMP0003 OLD)
#endif()


if (NOT DEFINED CMAKE_GLOBAL_AUTOGEN_TARGET_NAME)
	set(SCRIBUS_AUTOGEN_INCLUDE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}_autogen/include")
else()
	set(SCRIBUS_AUTOGEN_INCLUDE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}_${CMAKE_GLOBAL_AUTOGEN_TARGET_NAME}/include")
endif()
message(STATUS "Autogen Include Path: ${SCRIBUS_AUTOGEN_INCLUDE_PATH}")
#set(CMAKE_AUTOUIC_SEARCH_PATHS ${SCRIBUS_AUTOGEN_INCLUDE_PATH})

if(HAVE_OSG)
	add_executable(${EXE_NAME}
		${SCRIBUS_MAIN_CPP}
		${SCRIBUS_SOURCES}
		${SCRIBUS_API_SOURCES}
		${SCRIBUS_COLORMGMT_SOURCES}
		${SCRIBUS_DESAXE_SOURCES}
		${SCRIBUS_FONTS_SOURCES}
		${SCRIBUS_STYLES_SOURCES}
		${SCRIBUS_TEXT_SOURCES}
		${SCRIBUS_WIN32_ONLY_SOURCES}
		${GESTURE_FRAME_PREVIEW_SOURCES}
		${SCRIBUS_PRC_SOURCES}
	)
else()
	add_executable(${EXE_NAME}
		${SCRIBUS_MAIN_CPP}
		${SCRIBUS_SOURCES}
		${SCRIBUS_API_SOURCES}
		${SCRIBUS_COLORMGMT_SOURCES}
		${SCRIBUS_DESAXE_SOURCES}
		${SCRIBUS_FONTS_SOURCES}
		${SCRIBUS_STYLES_SOURCES}
		${SCRIBUS_TEXT_SOURCES}
		${SCRIBUS_WIN32_ONLY_SOURCES}
		${GESTURE_FRAME_PREVIEW_SOURCES}
	)
endif ()

if(WANT_PCH)
	target_precompile_headers(${EXE_NAME} PRIVATE scribus_pch.h)
endif()

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

##


#Set up libraries to link with all found packages
include_directories(${cairo_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} PRIVATE ${cairo_LIBRARIES})
link_directories(${PREFIX_cairo_LIB})
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${PREFIX_cairo_LIB}")

message("RPATH: ${CMAKE_INSTALL_RPATH}")

target_link_libraries(${EXE_NAME}
	PUBLIC
	${Qt5Core_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${Qt5Gui_LIBRARIES}
	${Qt5Xml_LIBRARIES}
	${Qt5Network_LIBRARIES}
	${Qt5OpenGL_LIBRARIES}
	PRIVATE
	${Qt5PrintSupport_LIBRARIES}
)

target_link_libraries(${EXE_NAME}
	PUBLIC
	${ZLIB_LIBRARIES}
	${LIBXML2_LIBRARIES}
	PRIVATE
	${CUPS_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${HARFBUZZ_LIBRARIES}
	${HARFBUZZ_SUBSET_LIBRARIES}
	${ICU_LIBRARIES}
	${TIFF_LIBRARIES}
	${PNG_LIBRARIES}
	${JPEG_LIBRARIES}
	${WIN32_ONLY_LIBS}
	${GMAGICK_LIBRARIES}
)


if (TARGET Fontconfig::Fontconfig)
	target_link_libraries(${EXE_NAME} PRIVATE
		Fontconfig::Fontconfig
	)
endif()



if (NOT WIN32 AND NOT HAIKU)
	target_link_libraries(${EXE_NAME} PRIVATE
		${CUPS_LIBRARIES}
	)
endif ()

# required to resolve symbols from libdl when ld is called with --no-add-needed
# #9310 for the OpenBSD addition
if(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND NOT HAIKU)
  target_link_libraries(${EXE_NAME} PUBLIC ${CMAKE_DL_LIBS})
endif()

# #9311 for the OpenBSD addition
if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
  target_link_libraries(${EXE_NAME} PRIVATE util execinfo)
endif()

if(HAIKU)
  target_link_libraries(${EXE_NAME} PRIVATE execinfo)
endif(HAIKU)

if(WIN32)
  target_link_libraries(${EXE_NAME} PRIVATE ${QT_QTMAIN_LIBRARY})
endif()

if(APPLE)
	target_link_libraries(${EXE_NAME}
		PUBLIC
		"-undefined dynamic_lookup"
		PRIVATE
		"-framework CoreFoundation"
		)
	target_link_options(${EXE_NAME}
		PUBLIC
		"-Wl,-no_fixup_chains"
	    )
endif()

if (WANT_SVNVERSION)
	add_dependencies(${EXE_NAME} svnheader)
endif ()

target_link_libraries(${EXE_NAME} PRIVATE
	${SCRIBUS_WPG_LIB}
	${SCRIBUS_PGF_LIB}
	${SCRIBUS_ZIP_LIB}
	${SCRIBUS_RTF_LIB}
	${LCMS2_LIBRARIES}
)

if(WITH_TESTS)
	target_link_libraries(${EXE_NAME} PRIVATE
		${QT_QTTEST_LIBRARY}
		${SCRIBUS_TESTS_LIB}
	)
endif()

if(HAVE_PODOFO)
	target_link_libraries(${EXE_NAME} PRIVATE
		${LIBPODOFO_LIBRARY}
	)
endif()

if(HAVE_OSG)
	target_link_libraries(${EXE_NAME} PRIVATE
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
	target_link_libraries(${EXE_NAME} PRIVATE
		${SCRIBUS_2GEOM_LIB}
	)
endif()

# Now build plugins

# IF your plugin needs additional libraries, please link them privately
# in the CMakeLists.txt for your plugin.

add_custom_target(Plugins ALL)
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
