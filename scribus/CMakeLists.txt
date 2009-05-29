IF(WIN32)
  ADD_DEFINITIONS(-DQT_DLL)
ENDIF(WIN32)

ADD_SUBDIRECTORY(dicts)
ADD_SUBDIRECTORY(dtd)
ADD_SUBDIRECTORY(keysets)
ADD_SUBDIRECTORY(loremipsum)
ADD_SUBDIRECTORY(manpages)
ADD_SUBDIRECTORY(styles)
ADD_SUBDIRECTORY(swatches)
ADD_SUBDIRECTORY(templates)
ADD_SUBDIRECTORY(text)
ADD_SUBDIRECTORY(fonts)
ADD_SUBDIRECTORY(desaxe)
ADD_SUBDIRECTORY(unicodemap)
ADD_SUBDIRECTORY(editorconfig)
IF (WANT_DESIGNER)
  ADD_SUBDIRECTORY(designer)
ENDIF (WANT_DESIGNER)

SET(SCRIBUS_UI_SRC
  ui/aboutplugins.ui
  ui/aligndistribute.ui
  ui/charselect.ui
  ui/charselectenhanced.ui
  ui/cmsprefsbase.ui
  ui/docitemattrprefs.ui
  ui/docsections.ui
  ui/gtfiledialog.ui
  ui/guidemanager.ui
  ui/hysettingsBase.ui
  ui/insertaframe.ui
  ui/latexeditor.ui
  ui/measurementsbase.ui
  ui/multipleduplicate.ui
  ui/multiprogressdialog.ui
  ui/nftdialog.ui
  ui/nftwidget.ui
  ui/pageitemattributes.ui
  ui/patterndialog.ui
  ui/picsearch.ui
  ui/picsearchoptions.ui
  ui/picstatus.ui
  ui/pluginmanagerprefsgui.ui
  ui/polygonwidgetbase.ui
  ui/printdialogbase.ui
  ui/replacecolors.ui
  ui/replaceonecolor.ui
  ui/selectobjects.ui
  ui/shortcutwidget.ui
  ui/smcstylewidget.ui
  ui/smlinestylewidget.ui
  ui/smpstylewidget.ui
  ui/smreplacedia.ui
  ui/smstyleimport.ui
  ui/stylemanager.ui
  ui/tabdisplay.ui
  ui/tabexternaltoolswidget.ui
  ui/tabgeneral.ui
  ui/tabkeyboardshortcutswidget.ui
  ui/tabmiscellaneous.ui
  ui/tabprinter.ui
  ui/tabscrapbook.ui
  ui/tocindexprefs.ui
  ui/copypagetomasterpage.ui
  ui/createrange.ui
  ui/helpbrowser.ui
  ui/helpbrowsernavigation.ui
  ui/unicodesearch.ui
  ui/useprintermarginsdialog.ui
)

SET(SCRIBUS_MOC_CLASSES
  ui/about.h
  ui/aboutplugins.h
  actionmanager.h
  ui/adjustcmsdialog.h
  ui/aligndistribute.h
  alignselect.h
  annot.h
  annota.h
  ui/applytemplatedialog.h
  ui/arrowchooser.h
  autoform.h
  autoformbuttongroup.h
  ui/basepointwidget.h
  ui/bookmarkpalette.h
  ui/bookmwin.h
  buttonicon.h
  canvas.h
  canvasgesture_rulermove.h
  canvasmode.h
  canvasmode_edit.h
  canvasmode_legacy.h
  canvasmode_measurements.h
  canvasmode_normal.h
  ui/charselect.h
  ui/charselectenhanced.h
  chartablemodel.h
  chartableview.h
  charzoom.h
  checkDocument.h
  ui/cmsprefs.h
  cmykfw.h
  collect4output.h
  colorblind.h
  colorchart.h
  colorcombo.h
  colorlistbox.h
  colorm.h
  commonstrings.h
  ui/contextmenu.h
  ui/cpalette.h
  cupsoptions.h
  ui/curvewidget.h
  ui/customfdialog.h
  dasheditor.h
  dcolor.h
  deferredtask.h
  delpages.h
  docinfo.h
  ui/docitemattrprefs.h
  ui/docsections.h
  editor.h
  ui/effectsdialog.h
  extimageprops.h
  fileloader.h
  filesearch.h
  filewatcher.h
  fontcombo.h
  fontlistmodel.h
  fontlistview.h
  fontprefs.h
  ui/fontreplacedialog.h
  gradienteditor.h
  ui/gtdialogs.h
  ui/gtfiledialog.h
  ui/guidemanager.h
  guidesdelegate.h
  guidesmodel.h
  guidesview.h
  ui/helpbrowser.h
  ui/hruler.h
  hyask.h
  hyphenator.h
  ui/hysettings.h
  ui/imageinfodialog.h
  insertTable.h
  ui/insertaframe.h
  inspage.h
  javadocs.h
  ui/latexeditor.h
  latexhelpers.h
  ui/layers.h
  loadsaveplugin.h
  loremipsum.h
  marginwidget.h
  ui/margindialog.h
  ui/masterpagepalette.h
  ui/measurements.h
  menumanager.h
  mergedoc.h
  missing.h
  movepage.h
  ui/multipleduplicate.h
  ui/multiprogressdialog.h
  navigator.h
  newfile.h
  newtemp.h
  ui/nftdialog.h
  ui/nftwidget.h
  ui/nodeeditpalette.h
  observable.h
  oneclick.h
  ui/outlinepalette.h
  pageitem.h
  pageitem_imageframe.h
  pageitem_latexframe.h
  pageitem_line.h
  pageitem_pathtext.h
  pageitem_polygon.h
  pageitem_polyline.h
  pageitem_textframe.h
  ui/pageitemattributes.h
  pageitempointer.h
  pagelayout.h
  ui/pagepalette.h
  ui/pageselector.h
  ui/patterndialog.h
  pdflib.h
  pdflib_core.h
  pdfopts.h
  ui/picsearch.h
  ui/picsearchoptions.h
  ui/picstatus.h
  pluginmanager.h
  ui/pluginmanagerprefsgui.h
  ui/polygonwidget.h
  polyprops.h
  ui/prefs.h
  ui/prefsdialogbase.h
  prefsmanager.h
  prefspanel.h
  preview.h
  ui/printdialog.h
  ui/propertiespalette.h
  pslib.h
  query.h
  ui/reformdoc.h
  ui/replacecolors.h
  ui/replaceonecolor.h
  rulermover.h
  sampleitem.h
  scfilewidget.h
  scgtplugin.h
  schelptreemodel.h
  ui/scinputdialog.h
  ui/scmenu.h
  ui/scmessagebox.h
  scplugin.h
  scprintengine.h
  scraction.h
  ui/scrapbookpalette.h
  scribus.h
  scribusapp.h
  scribuscore.h
  scribusdoc.h
  scribusview.h
  scribuswin.h
  ui/scrpalettebase.h
  scrspinbox.h
  sctablewidget.h
  ui/sctextbrowser.h
  ui/sctoolbar.h
  sctreewidget.h
  search.h
  selection.h
  ui/selectobjects.h
  selfield.h
  shadebutton.h
  ui/shortcutwidget.h
  ui/smalignselect.h
  ui/smcheckbox.h
  ui/smcolorcombo.h
  ui/smcstylewidget.h
  ui/smfontcomboh.h
  ui/smlinestyle.h
  ui/smlinestylewidget.h
  ui/smpstylewidget.h
  ui/smreplacedia.h
  ui/smsccombobox.h
  ui/smscrspinbox.h
  ui/smshadebutton.h
  ui/smspinbox.h
  ui/smstyleimport.h
  ui/smstyleselect.h
  ui/smtabruler.h
  ui/smtextstyles.h
  ui/spalette.h
  ui/storyeditor.h
  styleitem.h
  ui/stylemanager.h
  styleselect.h
  styleview.h
  ui/tabcheckdoc.h
  ui/tabdisplay.h
  ui/tabdocument.h
  ui/tabexternaltoolswidget.h
  ui/tabgeneral.h
  ui/tabguides.h
  ui/tabkeyboardshortcutswidget.h
  ui/tabmanager.h
  ui/tabmiscellaneous.h
  ui/tabpdfoptions.h
  ui/tabprinter.h
  ui/tabruler.h
  ui/tabscrapbook.h
  ui/tabtools.h
  ui/tabtypography.h
  tocgenerator.h
  ui/tocindexprefs.h
  tt/simpletreemodel/treemodel.h
  ui/copypagetomasterpagedialog.h
  ui/createrange.h
  ui/edittoolbar.h
  ui/filetoolbar.h  
  ui/modetoolbar.h
  ui/pdftoolbar.h
  undogui.h
  undomanager.h
  ui/unicodesearch.h
  upgradechecker.h
  urllauncher.h
  ui/useprintermarginsdialog.h
  ui/vruler.h
)

SET(SCRIBUS_SOURCES
  ui/about.cpp
  ui/aboutplugins.cpp
  actionmanager.cpp
  ui/adjustcmsdialog.cpp
  ui/aligndistribute.cpp
  alignselect.cpp
  annot.cpp
  annota.cpp
  annotation.cpp
  ui/applytemplatedialog.cpp
  ui/arrowchooser.cpp
  autoform.cpp
  autoformbuttongroup.cpp
  ui/basepointwidget.cpp
  ui/bookmarkpalette.cpp
  ui/bookmwin.cpp
  buttonicon.cpp
  canvas.cpp
  canvasgesture_linemove.cpp
  canvasgesture_pan.cpp
  canvasgesture_rectselect.cpp
  canvasgesture_resize.cpp
  canvasgesture_rulermove.cpp
  canvasmode.cpp
  canvasmode_copyproperties.cpp
  canvasmode_create.cpp
  canvasmode_drawbezier.cpp
  canvasmode_drawfreehand.cpp
  canvasmode_eyedropper.cpp
  canvasmode_framelinks.cpp
  canvasmode_edit.cpp
  canvasmode_editgradient.cpp
  canvasmode_legacy.cpp
  canvasmode_magnifier.cpp
  canvasmode_measurements.cpp
  canvasmode_nodeedit.cpp
  canvasmode_normal.cpp
  canvasmode_objimport.cpp
  canvasmode_panning.cpp
  canvasmode_rotate.cpp
  ui/charselect.cpp
  ui/charselectenhanced.cpp
  chartablemodel.cpp
  chartableview.cpp
  charzoom.cpp
  checkDocument.cpp
  cmserrorhandling.cpp
  cmsettings.cpp
  ui/cmsprefs.cpp
  cmykfw.cpp
  collect4output.cpp
  colorblind.cpp
  colorchart.cpp
  colorcombo.cpp
  colorlistbox.cpp
  colorm.cpp
  colorsetmanager.cpp
  commonstrings.cpp
  ui/contextmenu.cpp
  ui/cpalette.cpp
  cupsoptions.cpp
  ui/curvewidget.cpp
  ui/customfdialog.cpp
  dasheditor.cpp
  dcolor.cpp
  deferredtask.cpp
  delpages.cpp
  docinfo.cpp
  ui/docitemattrprefs.cpp
  ui/docsections.cpp
  documentchecker.cpp
  documentinformation.cpp
  editor.cpp
  ui/effectsdialog.cpp
  exif.cpp
  extimageprops.cpp
  fileloader.cpp
  filesearch.cpp
  fileunzip.cpp
  filewatcher.cpp
  fontcombo.cpp
  fontlistmodel.cpp
  fontlistview.cpp
  fontprefs.cpp
  ui/fontreplacedialog.cpp
  fparser.cpp
  fpoint.cpp
  fpointarray.cpp
  fpoptimizer.cpp
  gradienteditor.cpp
  gtaction.cpp
  ui/gtdialogs.cpp
  ui/gtfiledialog.cpp
  gtfont.cpp
  gtframestyle.cpp
  gtgettext.cpp
  gtmeasure.cpp
  gtparagraphstyle.cpp
  gtstyle.cpp
  gtwriter.cpp
  ui/guidemanager.cpp
  guidemanagercore.cpp
  guidesdelegate.cpp
  guidesmodel.cpp
  guidesview.cpp
  ui/helpbrowser.cpp
  hnjalloc.c
  ui/hruler.cpp
  hyask.cpp
  hyphen.c
  hyphenator.cpp
  ui/hysettings.cpp
  ui/imageinfodialog.cpp
  insertTable.cpp
  ui/insertaframe.cpp
  inspage.cpp
  ioapi.c
  javadocs.cpp
  KarbonCurveFit.cpp
  langmgr.cpp
  ui/latexeditor.cpp
  latexhelpers.cpp
  ui/layers.cpp
  linecombo.cpp
  linkbutton.cpp
  loadsaveplugin.cpp
  loremipsum.cpp
  marginwidget.cpp
  ui/margindialog.cpp
  ui/masterpagepalette.cpp
  ui/measurements.cpp
  menumanager.cpp
  mergedoc.cpp
  missing.cpp
  movepage.cpp
  ui/multipleduplicate.cpp
  ui/multiprogressdialog.cpp
  navigator.cpp
  newfile.cpp
  newtemp.cpp
  ui/nftdialog.cpp
  ui/nftwidget.cpp
  nftrcreader.cpp
  nftsettings.cpp
  nfttemplate.cpp
  ui/nodeeditpalette.cpp
  observable.cpp
  oneclick.cpp
  ui/outlinepalette.cpp
  page.cpp
  pageitem.cpp
  pageitem_desaxe.cpp
  pageitem_imageframe.cpp
  pageitem_latexframe.cpp
  pageitem_line.cpp
  pageitem_pathtext.cpp
  pageitem_polygon.cpp
  pageitem_polyline.cpp
  pageitem_textframe.cpp
  ui/pageitemattributes.cpp
  pageitempointer.cpp
  pagelayout.cpp
  ui/pagepalette.cpp
  ui/pageselector.cpp
  pagesize.cpp
  ui/patterndialog.cpp
  pdflib.cpp
  pdflib_core.cpp
  pdfoptions.cpp
  pdfoptionsio.cpp
  pdfopts.cpp
  ui/picsearch.cpp
  ui/picsearchoptions.cpp
  ui/picstatus.cpp
  pluginmanager.cpp
  ui/pluginmanagerprefsgui.cpp
  ui/polygonwidget.cpp
  polyprops.cpp
  ui/prefs.cpp
  prefscontext.cpp
  ui/prefsdialogbase.cpp
  prefsfile.cpp
  prefsmanager.cpp
  prefspanel.cpp
  prefsreader.cpp
  prefstable.cpp
  preview.cpp
  ui/printdialog.cpp
  ui/propertiespalette.cpp
  pslib.cpp
  query.cpp
  rawimage.cpp
  rc4.c
  ui/reformdoc.cpp
  ui/replacecolors.cpp
  ui/replaceonecolor.cpp
  rulermover.cpp
  sampleitem.cpp
  scclocale.cpp
  sccolor.cpp
  sccolorengine.cpp
  sccolorshade.cpp
  sccombobox.cpp
  scdocoutput.cpp
  scdocoutput_ps2.cpp
  scfilewidget.cpp
  scfonts.cpp
  scgtplugin.cpp
  scgzfile.cpp
  schelptreemodel.cpp
  scimage.cpp
  scimagestructs.cpp
  scimgdataloader.cpp
  scimgdataloader_gimp.cpp
  scimgdataloader_jpeg.cpp
  scimgdataloader_pdf.cpp
  scimgdataloader_ps.cpp
  scimgdataloader_psd.cpp
  scimgdataloader_qt.cpp
  scimgdataloader_tiff.cpp
  ui/scinputdialog.cpp
  sclayer.cpp
  ui/scmenu.cpp
  ui/scmessagebox.cpp
  scmimedata.cpp
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
  ui/scrapbookpalette.cpp
  scresizecursor.cpp
  scribus.cpp
  scribusXml.cpp
  scribusapp.cpp
  scribuscore.cpp
  scribusdoc.cpp
  scribusview.cpp
  scribuswin.cpp
  ui/scrpalettebase.cpp
  scrspinbox.cpp
  scslainforeader.cpp
  scstreamfilter.cpp
  scstreamfilter_ascii85.cpp
  scstreamfilter_flate.cpp
  scstreamfilter_rc4.cpp
  sctablewidget.cpp
  ui/sctextbrowser.cpp
  sctextstream.cpp
  sctextstruct.cpp
  ui/sctoolbar.cpp
  sctreewidget.cpp
  search.cpp
  selection.cpp
  ui/selectobjects.cpp
  selfield.cpp
  serializer.cpp
  shadebutton.cpp
  ui/shortcutwidget.cpp
  ui/smalignselect.cpp
  ui/smcheckbox.cpp
  ui/smcolorcombo.cpp
  ui/smcstylewidget.cpp
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
  ui/smstyleimport.cpp
  ui/smstyleselect.cpp
  ui/smtabruler.cpp
  ui/smtextstyles.cpp
  ui/spalette.cpp
  ui/splash.cpp
  stencilreader.cpp
  ui/storyeditor.cpp
  styleitem.cpp
  ui/stylemanager.cpp
  styleselect.cpp
  styleview.cpp
  ui/tabcheckdoc.cpp
  ui/tabdisplay.cpp
  ui/tabdocument.cpp
  ui/tabexternaltoolswidget.cpp
  ui/tabgeneral.cpp
  ui/tabguides.cpp
  ui/tabkeyboardshortcutswidget.cpp
  ui/tabmanager.cpp
  ui/tabmiscellaneous.cpp
  ui/tabpdfoptions.cpp
  ui/tabprinter.cpp
  ui/tabruler.cpp
  ui/tabscrapbook.cpp
  ui/tabtools.cpp
  ui/tabtypography.cpp
  tocgenerator.cpp
  ui/tocindexprefs.cpp
  transaction.cpp
  tt/simpletreemodel/treeitem.cpp
  tt/simpletreemodel/treemodel.cpp
  ui/copypagetomasterpagedialog.cpp
  ui/createrange.cpp
  ui/edittoolbar.cpp
  ui/filetoolbar.cpp
  ui/helpnavigation.cpp
  ui/modetoolbar.cpp
  ui/pdftoolbar.cpp
  undogui.cpp
  undomanager.cpp
  undoobject.cpp
  undostack.cpp
  undostate.cpp
  ui/unicodesearch.cpp
  units.cpp
  unzip.c
  updatemanager.cpp
  upgradechecker.cpp
  urllauncher.cpp
  ui/useprintermarginsdialog.cpp
  util.cpp
  util_cms.cpp
  util_color.cpp
  util_file.cpp
  util_formats.cpp
  util_ghostscript.cpp
  util_icon.cpp
  util_math.cpp
  util_printer.cpp
  util_text.cpp
  vgradient.cpp
  vgradientex.cpp
  ui/vruler.cpp
)

IF(WIN32)
  SET(SCRIBUS_MOC_WIN32_ONLY_CLASSES scprintengine_gdi.h)
  SET(SCRIBUS_WIN32_ONLY_SOURCES
      scpainterex_cairo.cpp
      scprintengine_gdi.cpp
      )
  SET(SCRIBUS_MAIN_CPP main_win32.cpp)
ELSE(WIN32)
  SET(SCRIBUS_MOC_WIN32_ONLY_CLASSES)
  SET(SCRIBUS_WIN32_ONLY_SOURCES)
  SET(SCRIBUS_MAIN_CPP main.cpp)
ENDIF(WIN32)

QT4_WRAP_UI(SCRIBUS_UI_SOURCES ${SCRIBUS_UI_SRC} )

QT4_WRAP_CPP(SCRIBUS_MOC_SOURCES
	${SCRIBUS_MOC_CLASSES}
	${SCRIBUS_MOC_WIN32_ONLY_CLASSES}
	)

LINK_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}/desaxe
  ${CMAKE_CURRENT_BINARY_DIR}/text
  ${CMAKE_CURRENT_BINARY_DIR}/styles
  ${CMAKE_CURRENT_BINARY_DIR}/fonts
)

SET(SCRIBUS_TEXT_LIB "scribus_text_lib")
SET(SCRIBUS_STYLES_LIB "scribus_styles_lib")
SET(SCRIBUS_FONTS_LIB "scribus_fonts_lib")
SET(SCRIBUS_DESAXE_LIB "scribus_desaxe_lib")

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
)

IF(HAVE_PODOFO)
  INCLUDE_DIRECTORIES(
    ${LIBPODOFO_INCLUDE_DIR}
    )
ENDIF(HAVE_PODOFO)

IF(GESTURE_FRAME_PREVIEW)
   MESSAGE(STATUS "Enable gesture frame preview [experimental]")
   SET(GESTURE_FRAME_PREVIEW_SOURCES  pageitempreview.cpp)
   ADD_DEFINITIONS(-DGESTURE_FRAME_PREVIEW)
ENDIF(GESTURE_FRAME_PREVIEW)

IF(APPLEBUNDLE)
  SET(EXE_NAME "Scribus${TAG_VERSION}" CACHE TYPE STRING)
ELSE(APPLEBUNDLE)
  SET(EXE_NAME "scribus${TAG_VERSION}" CACHE TYPE STRING)
ENDIF(APPLEBUNDLE)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
ENDIF(COMMAND cmake_policy)

ADD_EXECUTABLE(${EXE_NAME}
  ${SCRIBUS_MOC_SOURCES}
  ${SCRIBUS_SOURCES}
  ${SCRIBUS_MAIN_CPP}
  ${SCRIBUS_UI_SOURCES}
  ${SCRIBUS_TEXT_SOURCES}
  ${SCRIBUS_STYLES_SOURCES}
  ${SCRIBUS_FONTS_SOURCES}
  ${SCRIBUS_DESAXE_SOURCES}
  ${SCRIBUS_WIN32_ONLY_SOURCES}
  ${GESTURE_FRAME_PREVIEW_SOURCES}
)

IF(WIN32)
  SET_TARGET_PROPERTIES(${EXE_NAME}
	PROPERTIES
	COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
	ENABLE_EXPORTS TRUE
	)
  SET(WIN32_ONLY_LIBS mscms.lib)
ELSE(WIN32)
  SET(WIN32_ONLY_LIBS)
ENDIF(WIN32)

##TEST FIX FOR 6897
IF (GCC AND NOT WIN32)
# Use the -E / --export-dynamic flag to ensure that symbols in the Scribus
# binary are in the public symbol table for plugins. The symbols must also
# be set as SCRIBUS_API if we're building with -fvisibility=hidden .
# Should fix bug 6897 .
     SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES LINK_FLAGS -Wl,-E)
 ENDIF (GCC AND NOT WIN32)
##


#Set up libraries to link with all found packages

#Select Libart_LGPL or Cairo
IF(RENDER_LIB STREQUAL "CAIRO")
  INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(${EXE_NAME} ${CAIRO_LIBRARIES})
  LINK_DIRECTORIES(${PREFIX_CAIRO_LIB})
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${PREFIX_CAIRO_LIB}")
ENDIF(RENDER_LIB STREQUAL "CAIRO")

MESSAGE("RPATH: ${CMAKE_INSTALL_RPATH}")

TARGET_LINK_LIBRARIES(${EXE_NAME}
  ${QT_LIBRARIES}
  ${CUPS_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${LCMS_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  ${PYTHON_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${WIN32_ONLY_LIBS}
)

IF(WIN32)
  TARGET_LINK_LIBRARIES(${EXE_NAME} ${QT_QTMAIN_LIBRARY})
ENDIF(WIN32)

IF(APPLE)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    "-undefined dynamic_lookup"
    "-framework CoreFoundation" )
ENDIF(APPLE)

TARGET_LINK_LIBRARIES(${EXE_NAME}
  ${SCRIBUS_DESAXE_LIB}
  ${SCRIBUS_TEXT_LIB}
  ${SCRIBUS_STYLES_LIB}
  ${SCRIBUS_FONTS_LIB}
)

IF(HAVE_PODOFO)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    ${LIBPODOFO_LIBRARY}
  )
ENDIF(HAVE_PODOFO)

# Now build plugins
SET(PLUGIN_LIBRARIES)
IF(WIN32)
  # IF your plugin needs more than this, please link them privately
  # in the CMakeLists.txt for your plugin.
  SET(PLUGIN_LIBRARIES
#    ${QT_LIBRARIES} part of #7210
    ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
    ${LIBXML2_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${EXE_NAME}
    )
ENDIF(WIN32)
IF(APPLE)
  SET(PLUGIN_LIBRARIES "-undefined dynamic_lookup")
ENDIF(APPLE)

ADD_SUBDIRECTORY(plugins)

#do we want a lib suffix on osx? doubt it
IF(APPLEBUNDLE)
  INSTALL(DIRECTORY DESTINATION MacOS)
  INSTALL(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION MacOS
    LIBRARY DESTINATION lib
    )
#  ADD_CUSTOM_COMMAND(TARGET ${EXE_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} ARGS -Dinput_file=${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.app/Contents/MacOS/${EXE_NAME} -P ${CMAKE_MODULE_PATH}/ScribusIngestOSXBundleLibraries.cmake
#  )

#  INSTALL(
#	CODE "SET(input_file ${CMAKE_INSTALL_PREFIX}/MacOS/${EXE_NAME})"  
#	SCRIPT ${CMAKE_MODULE_PATH}/ScribusIngestOSXBundleLibraries.cmake )
ELSE(APPLEBUNDLE)
  IF(WIN32)
    INSTALL(TARGETS ${EXE_NAME}
      RUNTIME DESTINATION .
      LIBRARY DESTINATION ${LIB_DIR_NAME}
      )
  ELSE(WIN32)
    INSTALL(TARGETS ${EXE_NAME}
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION ${LIB_DIR_NAME}
      )
  ENDIF(WIN32)
ENDIF(APPLEBUNDLE)

#Install our header files, selected from all existing dirs
IF(NOT WANT_NOHEADERINSTALL)
  FILE( GLOB_RECURSE SCRIBUS_HEADER_FILES *.h )
  INSTALL(FILES ${SCRIBUS_HEADER_FILES} DESTINATION ${INCLUDEDIR})
ELSE(NOT WANT_NOHEADERINSTALL)
  MESSAGE(STATUS "No source header files will be installed")
ENDIF(NOT WANT_NOHEADERINSTALL)
