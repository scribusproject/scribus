IF(WIN32)
  ADD_DEFINITIONS(-DQT_DLL)
ENDIF(WIN32)

ADD_SUBDIRECTORY(dicts)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(dtd)
ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(keysets)
ADD_SUBDIRECTORY(loremipsum)
ADD_SUBDIRECTORY(manpages)
ADD_SUBDIRECTORY(profiles)
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
  aboutplugins.ui
  aligndistribute.ui
  charselect.ui
  cmsprefsbase.ui
  docitemattrprefs.ui
  docsections.ui
  gtfiledialog.ui
  guidemanager.ui
  hysettingsBase.ui
  insertaframe.ui
  latexeditor.ui
  measurementsbase.ui
  multipleduplicate.ui
  multiprogressdialog.ui
  pageitemattributes.ui
  patterndialog.ui
  picsearch.ui
  picsearchoptions.ui
  picstatus.ui
  pluginmanagerprefsgui.ui
  printdialogbase.ui
  replacecolors.ui
  replaceonecolor.ui
  selectobjects.ui
  shortcutwidget.ui
  smcstylewidget.ui
  smlinestylewidget.ui
  smpstylewidget.ui
  smreplacedia.ui
  smstyleimport.ui
  stylemanager.ui
  tabdisplay.ui
  tabexternaltoolswidget.ui
  tabgeneral.ui
  tabkeyboardshortcutswidget.ui
  tabmiscellaneous.ui
  tabprinter.ui
  tabscrapbook.ui
  tocindexprefs.ui
  ui/copypagetomasterpage.ui
  ui/createrange.ui
  ui/helpbrowser.ui
  unicodesearch.ui
  useprintermarginsdialog.ui
)

SET(SCRIBUS_MOC_CLASSES
  about.h
  aboutplugins.h
  actionmanager.h
  adjustcmsdialog.h
  aligndistribute.h
  alignselect.h
  annot.h
  annota.h
  applytemplatedialog.h
  arrowchooser.h
  autoform.h
  autoformbuttongroup.h
  basepointwidget.h
  bookmarkpalette.h
  bookmwin.h
  buttonicon.h
  canvas.h
  canvasmode_measurements.h
  canvasmode_legacy.h
  charselect.h
  chartablemodel.h
  chartableview.h
  charzoom.h
  checkDocument.h
  cmsprefs.h
  cmykfw.h
  collect4output.h
  colorblind.h
  colorchart.h
  colorcombo.h
  colorlistbox.h
  colorm.h
  commonstrings.h
  contextmenu.h
  cpalette.h
  cupsoptions.h
  curvewidget.h
  customfdialog.h
  dasheditor.h
  dcolor.h
  deferredtask.h
  delpages.h
  docinfo.h
  docitemattrprefs.h
  docsections.h
  editor.h
  effectsdialog.h
  extimageprops.h
  fileloader.h
  filesearch.h
  filewatcher.h
  fontcombo.h
  fontlistmodel.h
  fontlistview.h
  fontprefs.h
  fontreplacedialog.h
  gradienteditor.h
  gtdialogs.h
  gtfiledialog.h
  guidemanager.h
  guidesdelegate.h
  guidesmodel.h
  helpbrowser.h
  hruler.h
  hyask.h
  hyphenator.h
  hysettings.h
  imageinfodialog.h
  insertTable.h
  insertaframe.h
  inspage.h
  javadocs.h
  latexeditor.h
  latexhelpers.h
  layers.h
  loadsaveplugin.h
  loremipsum.h
  marginwidget.h
  margindialog.h
  masterpagepalette.h
  measurements.h
  menumanager.h
  mergedoc.h
  missing.h
  movepage.h
  multipleduplicate.h
  multiprogressdialog.h
  navigator.h
  newfile.h
  newtemp.h
  nodeeditpalette.h
  observable.h
  oneclick.h
  outlinepalette.h
  pageitem.h
  pageitem_imageframe.h
  pageitem_latexframe.h
  pageitem_line.h
  pageitem_pathtext.h
  pageitem_polygon.h
  pageitem_polyline.h
  pageitem_textframe.h
  pageitemattributes.h
  pagelayout.h
  pagepalette.h
  pageselector.h
  patterndialog.h
  pdflib.h
  pdflib_core.h
  pdfopts.h
  picsearch.h
  picsearchoptions.h
  picstatus.h
  pluginmanager.h
  pluginmanagerprefsgui.h
  polygonwidget.h
  polyprops.h
  prefs.h
  prefsdialogbase.h
  prefsmanager.h
  prefspanel.h
  preview.h
  printdialog.h
  propertiespalette.h
  pslib.h
  query.h
  reformdoc.h
  replacecolors.h
  replaceonecolor.h
  rulermover.h
  sampleitem.h
  scfilewidget.h
  scgtplugin.h
  schelptreemodel.h
  scinputdialog.h
  scmenu.h
  scmessagebox.h
  scplugin.h
  scprintengine.h
  scraction.h
  scrapbookpalette.h
  scribus.h
  scribusapp.h
  scribuscore.h
  scribusdoc.h
  scribusview.h
  scribuswin.h
  scrpalettebase.h
  scrspinbox.h
  sctablewidget.h
  sctextbrowser.h
  sctoolbar.h
  sctreewidget.h
  search.h
  selection.h
  selectobjects.h
  selfield.h
  shadebutton.h
  shortcutwidget.h
  smalignselect.h
  smcolorcombo.h
  smcstylewidget.h
  smfontcomboh.h
  smlinestyle.h
  smlinestylewidget.h
  smpstylewidget.h
  smreplacedia.h
  smsccombobox.h
  smscrspinbox.h
  smshadebutton.h
  smspinbox.h
  smstyleimport.h
  smstyleselect.h
  smtabruler.h
  smtextstyles.h
  spalette.h
  story.h
  styleitem.h
  stylemanager.h
  styleselect.h
  styleview.h
  tabcheckdoc.h
  tabdisplay.h
  tabdocument.h
  tabexternaltoolswidget.h
  tabgeneral.h
  tabguides.h
  tabkeyboardshortcutswidget.h
  tabmanager.h
  tabmiscellaneous.h
  tabpdfoptions.h
  tabprinter.h
  tabruler.h
  tabscrapbook.h
  tabtools.h
  tabtypography.h
  tocgenerator.h
  tocindexprefs.h
  tt/simpletreemodel/treemodel.h
  ui/copypagetomasterpagedialog.h
  ui/createrange.h
  ui/edittoolbar.h
  ui/filetoolbar.h  
  ui/modetoolbar.h
  ui/pdftoolbar.h
  undogui.h
  undomanager.h
  unicodesearch.h
  upgradechecker.h
  urllauncher.h
  useprintermarginsdialog.h
  vruler.h
)

SET(SCRIBUS_SOURCES
  about.cpp
  aboutplugins.cpp
  actionmanager.cpp
  adjustcmsdialog.cpp
  aligndistribute.cpp
  alignselect.cpp
  annot.cpp
  annota.cpp
  annotation.cpp
  applytemplatedialog.cpp
  arrowchooser.cpp
  autoform.cpp
  autoformbuttongroup.cpp
  basepointwidget.cpp
  bookmarkpalette.cpp
  bookmwin.cpp
  buttonicon.cpp
  canvas.cpp
  canvasgesture_linemove.cpp
  canvasgesture_rectselect.cpp
  canvasgesture_resize.cpp
  canvasgesture_rulermove.cpp
  canvasmode.cpp
  canvasmode_create.cpp
  canvasmode_drawbezier.cpp
  canvasmode_drawfreehand.cpp
  canvasmode_legacy.cpp
  canvasmode_measurements.cpp
  canvasmode_nodeedit.cpp
  charselect.cpp
  chartablemodel.cpp
  chartableview.cpp
  charzoom.cpp
  checkDocument.cpp
  cmserrorhandling.cpp
  cmsettings.cpp
  cmsprefs.cpp
  cmykfw.cpp
  collect4output.cpp
  colorblind.cpp
  colorchart.cpp
  colorcombo.cpp
  colorlistbox.cpp
  colorm.cpp
  colorsetmanager.cpp
  commonstrings.cpp
  contextmenu.cpp
  cpalette.cpp
  cupsoptions.cpp
  curvewidget.cpp
  customfdialog.cpp
  dasheditor.cpp
  dcolor.cpp
  deferredtask.cpp
  delpages.cpp
  docinfo.cpp
  docitemattrprefs.cpp
  docsections.cpp
  documentchecker.cpp
  documentinformation.cpp
  editor.cpp
  effectsdialog.cpp
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
  fontreplacedialog.cpp
  fparser.cpp
  fpoint.cpp
  fpointarray.cpp
  fpoptimizer.cpp
  gradienteditor.cpp
  gtaction.cpp
  gtdialogs.cpp
  gtfiledialog.cpp
  gtfont.cpp
  gtframestyle.cpp
  gtgettext.cpp
  gtmeasure.cpp
  gtparagraphstyle.cpp
  gtstyle.cpp
  gtwriter.cpp
  guidemanager.cpp
  guidemanagercore.cpp
  guidesdelegate.cpp
  guidesmodel.cpp
  helpbrowser.cpp
  hnjalloc.c
  hruler.cpp
  hyask.cpp
  hyphen.c
  hyphenator.cpp
  hysettings.cpp
  imageinfodialog.cpp
  insertTable.cpp
  insertaframe.cpp
  inspage.cpp
  ioapi.c
  javadocs.cpp
  KarbonCurveFit.cpp
  langmgr.cpp
  latexeditor.cpp
  latexhelpers.cpp
  layers.cpp
  linecombo.cpp
  linkbutton.cpp
  loadsaveplugin.cpp
  loremipsum.cpp
  marginwidget.cpp
  margindialog.cpp
  masterpagepalette.cpp
  measurements.cpp
  menumanager.cpp
  mergedoc.cpp
  missing.cpp
  movepage.cpp
  multipleduplicate.cpp
  multiprogressdialog.cpp
  navigator.cpp
  newfile.cpp
  newtemp.cpp
  nodeeditpalette.cpp
  observable.cpp
  oneclick.cpp
  outlinepalette.cpp
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
  pageitemattributes.cpp
  pagelayout.cpp
  pagepalette.cpp
  pageselector.cpp
  pagesize.cpp
  patterndialog.cpp
  pdflib.cpp
  pdflib_core.cpp
  pdfoptions.cpp
  pdfoptionsio.cpp
  pdfopts.cpp
  picsearch.cpp
  picsearchoptions.cpp
  picstatus.cpp
  pluginmanager.cpp
  pluginmanagerprefsgui.cpp
  polygonwidget.cpp
  polyprops.cpp
  prefs.cpp
  prefscontext.cpp
  prefsdialogbase.cpp
  prefsfile.cpp
  prefsmanager.cpp
  prefspanel.cpp
  prefsreader.cpp
  prefstable.cpp
  preview.cpp
  printdialog.cpp
  propertiespalette.cpp
  pslib.cpp
  query.cpp
  rawimage.cpp
  rc4.c
  reformdoc.cpp
  replacecolors.cpp
  replaceonecolor.cpp
  rulermover.cpp
  sampleitem.cpp
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
  scinputdialog.cpp
  sclayer.cpp
  scmenu.cpp
  scmessagebox.cpp
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
  scrapbookpalette.cpp
  scribus.cpp
  scribusXml.cpp
  scribusapp.cpp
  scribuscore.cpp
  scribusdoc.cpp
  scribusview.cpp
  scribuswin.cpp
  scrpalettebase.cpp
  scrspinbox.cpp
  scslainforeader.cpp
  scstreamfilter.cpp
  scstreamfilter_ascii85.cpp
  scstreamfilter_flate.cpp
  scstreamfilter_rc4.cpp
  sctablewidget.cpp
  sctextbrowser.cpp
  sctextstruct.cpp
  sctoolbar.cpp
  sctreewidget.cpp
  search.cpp
  selection.cpp
  selectobjects.cpp
  selfield.cpp
  serializer.cpp
  shadebutton.cpp
  shortcutwidget.cpp
  smalignselect.cpp
  smcolorcombo.cpp
  smcstylewidget.cpp
  smfontcomboh.cpp
  smlinestyle.cpp
  smlinestylewidget.cpp
  smpstylewidget.cpp
  smreplacedia.cpp
  smsccombobox.cpp
  smscrspinbox.cpp
  smshadebutton.cpp
  smspinbox.cpp
  smstyleimport.cpp
  smstyleselect.cpp
  smtabruler.cpp
  smtextstyles.cpp
  spalette.cpp
  splash.cpp
  stencilreader.cpp
  story.cpp
  styleitem.cpp
  stylemanager.cpp
  styleselect.cpp
  styleview.cpp
  tabcheckdoc.cpp
  tabdisplay.cpp
  tabdocument.cpp
  tabexternaltoolswidget.cpp
  tabgeneral.cpp
  tabguides.cpp
  tabkeyboardshortcutswidget.cpp
  tabmanager.cpp
  tabmiscellaneous.cpp
  tabpdfoptions.cpp
  tabprinter.cpp
  tabruler.cpp
  tabscrapbook.cpp
  tabtools.cpp
  tabtypography.cpp
  tocgenerator.cpp
  tocindexprefs.cpp
  transaction.cpp
  tt/simpletreemodel/treeitem.cpp
  tt/simpletreemodel/treemodel.cpp
  ui/copypagetomasterpagedialog.cpp
  ui/createrange.cpp
  ui/edittoolbar.cpp
  ui/filetoolbar.cpp
  ui/modetoolbar.cpp
  ui/pdftoolbar.cpp
  undogui.cpp
  undomanager.cpp
  undoobject.cpp
  undostack.cpp
  undostate.cpp
  unicodesearch.cpp
  units.cpp
  unzip.c
  updatemanager.cpp
  upgradechecker.cpp
  urllauncher.cpp
  useprintermarginsdialog.cpp
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
  vruler.cpp
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
    ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
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
ENDIF(NOT WANT_NOHEADERINSTALL)
