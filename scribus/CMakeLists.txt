IF(WIN32)
  ADD_DEFINITIONS(-DQT_DLL)
ENDIF(WIN32)

ADD_SUBDIRECTORY(dtd)
ADD_SUBDIRECTORY(manpages)
ADD_SUBDIRECTORY(colormgmt)
ADD_SUBDIRECTORY(desaxe)
ADD_SUBDIRECTORY(fonts)
ADD_SUBDIRECTORY(styles)
ADD_SUBDIRECTORY(text)
ADD_SUBDIRECTORY(unicodemap)
IF (WANT_DESIGNER)
  ADD_SUBDIRECTORY(designer)
ENDIF (WANT_DESIGNER)
ADD_SUBDIRECTORY(third_party)


IF(HAVE_OSG)
  SET(SCRIBUS_OSG_UI_SRC
  ui/osgeditor.ui
  )
  SET(SCRIBUS_OSG_CLASS
  ui/AdapterWidget.h
  ui/osgeditor.h
  pageitem_osgframe.h
  )
  SET(SCRIBUS_OSG_SRC
  ui/AdapterWidget.cpp
  ui/osgeditor.cpp
  pageitem_osgframe.cpp
  )
ENDIF (HAVE_OSG)

IF (GMAGICK_FOUND)
    SET(SCRIBUS_GMAGICK_SRC scimgdataloader_gmagick.cpp)
ELSE(GMAGICK_FOUND)
    SET(SCRIBUS_GMAGICK_SRC)
ENDIF(GMAGICK_FOUND)

# We setup something like a group to gather setter related stuff
# because it could expand into a huge amount of file at some point.
SET(SCRIBUS_ITEMSETTERS_UI_SRC
ui/positionsetter.ui
ui/rotationsetter.ui
)
SET(SCRIBUS_ITEMSETTERS_CLASS
	openpalettemanager.h
	openpalettemodel.h
	pageitemsettersmanager.h
	ui/openpalette.h
	ui/openpaletteview.h
	ui/pageitempositionsetter.h
	ui/pageitemrotationsetter.h
)
SET(SCRIBUS_ITEMSETTERS_SRC
	openpalettemanager.cpp
	openpalettemodel.cpp
	pageitemsettersmanager.cpp
	ui/openpalette.cpp
	ui/openpaletteview.cpp
	ui/pageitemsetterbase.cpp
	ui/pageitempositionsetter.cpp
	ui/pageitemrotationsetter.cpp
)

SET(SCRIBUS_UI_SRC
  ui/aboutplugins.ui
  ui/aligndistribute.ui
  ui/arcvectorbase.ui
  ui/arcwidgetbase.ui
  ui/charselect.ui
  ui/charselectenhanced.ui
  ui/cmsprefsbase.ui
  ui/colorpalette.ui
  ui/gradientaddedit.ui
  ui/gradienteditor.ui
  ui/gradientvectorbase.ui
  ui/gtfiledialog.ui
  ui/guidemanager.ui
  ui/insertaframe.ui
  ui/latexeditor.ui
  ui/newmarginwidgetbase.ui
  ui/measurementsbase.ui
  ui/multipleduplicate.ui
  ui/multiprogressdialog.ui
  ui/nftdialog.ui
  ui/nftwidget.ui
  ui/pageitemattributes.ui
  ui/paintmanagerbase.ui
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
  ui/prefs_typographybase.ui
  ui/prefs_userinterfacebase.ui
  ui/prefs_itemtoolsbase.ui
  ui/prefs_operatortoolsbase.ui
  ui/prefs_pdfexportbase.ui
  ui/prefs_tableofcontentsbase.ui
  ui/prefs_documentitemattributesbase.ui
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
  ui/spiralvectorbase.ui
  ui/spiralwidgetbase.ui
  ui/stylemanager.ui
  ui/transformdialogbase.ui
  ui/transparencypalette.ui
  ui/copypagetomasterpage.ui
  ui/createrange.ui
  ui/helpbrowser.ui
  ui/helpbrowsernavigation.ui
  ui/unicodesearch.ui
  ui/useprintermarginsdialog.ui
#  ${SCRIBUS_ITEMSETTERS_UI_SRC}
  ${SCRIBUS_OSG_UI_SRC}
)

SET(SCRIBUS_MOC_CLASSES
  actionmanager.h
  canvas.h
  canvasgesture_rulermove.h
  canvasmode.h
  canvasmode_edit.h
  canvasmode_editarc.h
  canvasmode_editpolygon.h
  canvasmode_editspiral.h
  canvasmode_measurements.h
  canvasmode_normal.h
  chartablemodel.h
  chartableview.h
  collect4output.h
  colorblind.h
  commonstrings.h
  deferredtask.h
  docinfo.h
  fileloader.h
  filesearch.h
  filewatcher.h
  fontlistmodel.h
  guidesdelegate.h
  guidesmodel.h
  guidesview.h
  hyphenator.h
  latexhelpers.h
  loadsaveplugin.h
  menumanager.h
  navigator.h
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
  pageitem_textframe.h
  pageitempointer.h
  pdf_analyzer.h
  pdflib.h
  pdflib_core.h
  pluginmanager.h
  prefsmanager.h
  pslib.h
  sampleitem.h
  scgtplugin.h
  schelptreemodel.h
  scimagecachedir.h
  scimagecachefile.h
  scimagecachemanager.h
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
  ui/cmsprefs.h
  ui/cmykfw.h
  ui/collectforoutput_ui.h
  ui/colorchart.h
  ui/colorcombo.h
  ui/colorlistbox.h
  ui/contextmenu.h
  ui/cpalette.h
  ui/cupsoptions.h
  ui/curvewidget.h
  ui/customfdialog.h
  ui/dasheditor.h
  ui/dcolor.h
  ui/delpages.h
  ui/editor.h
  ui/effectsdialog.h
  ui/extimageprops.h
  ui/fontcombo.h
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
  ui/hruler.h
  ui/hyask.h
  ui/imageinfodialog.h
  ui/insertTable.h
  ui/insertaframe.h
  ui/inspage.h
  ui/javadocs.h
  ui/latexeditor.h
  ui/layers.h
  ui/loremipsum.h
  ui/margindialog.h
  ui/marginpresetlayout.h
  ui/marginwidget.h
  ui/newmarginwidget.h
  ui/masterpagepalette.h
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
  ui/oneclick.h
  ui/outlinepalette.h
  ui/pageitemattributes.h
  ui/pagelayout.h
  ui/pagepalette.h
  ui/pageselector.h
  ui/paintmanager.h
  ui/patternpropsdialog.h
  ui/pdfopts.h
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
  ui/query.h
  ui/replacecolors.h
  ui/replaceonecolor.h
  ui/rulermover.h
  ui/scfilewidget.h
  ui/scinputdialog.h
  ui/scmenu.h
  ui/scmessagebox.h
  ui/scrapbookpalette.h
  ui/scmwmenumanager.h
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
  ui/spiralvectordialog.h
  ui/spiralwidget.h
  ui/storyeditor.h
  ui/stylemanager.h
  ui/styleselect.h
  ui/styleview.h
  ui/swatchcombo.h
  ui/symbolpalette.h
  ui/tabmanager.h
  ui/tabpdfoptions.h
  ui/tabruler.h
  ui/transformdialog.h
  ui/transparencypalette.h
  tt/simpletreemodel/treemodel.h
  ui/copypagetomasterpagedialog.h
  ui/createrange.h
  ui/edittoolbar.h
  ui/filetoolbar.h  
  ui/modetoolbar.h
  ui/pdftoolbar.h
  ui/unicodesearch.h
  ui/useprintermarginsdialog.h
  ui/vruler.h
#  ${SCRIBUS_ITEMSETTERS_CLASS}
  ${SCRIBUS_OSG_CLASS}
)

SET(SCRIBUS_SOURCES
  actionmanager.cpp
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
  canvasmode_drawcalligraphic.cpp
  canvasmode_drawfreehand.cpp
  canvasmode_eyedropper.cpp
  canvasmode_framelinks.cpp
  canvasmode_edit.cpp
  canvasmode_editarc.cpp
  canvasmode_editgradient.cpp
  canvasmode_editmeshgradient.cpp
  canvasmode_editpolygon.cpp
  canvasmode_editspiral.cpp
  canvasmode_magnifier.cpp
  canvasmode_measurements.cpp
  canvasmode_nodeedit.cpp
  canvasmode_normal.cpp
  canvasmode_objimport.cpp
  canvasmode_panning.cpp
  canvasmode_rotate.cpp
  chartablemodel.cpp
  chartableview.cpp
  cmserrorhandling.cpp
  cmsettings.cpp
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
  fileunzip.cpp
  filewatcher.cpp
  ui/fontcombo.cpp
  fontlistmodel.cpp
  fparser.cpp
  fpoint.cpp
  fpointarray.cpp
  fpoptimizer.cpp
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
  ioapi.c
  KarbonCurveFit.cpp
  langmgr.cpp
  latexhelpers.cpp
  loadsaveplugin.cpp
  menumanager.cpp
  mesh.cpp
  navigator.cpp
  observable.cpp
  nftrcreader.cpp
  nftsettings.cpp
  nfttemplate.cpp
  page.cpp
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
  pageitem_textframe.cpp
  pageitempointer.cpp
  pagesize.cpp
  pdf_analyzer.cpp
  pdflib.cpp
  pdflib_core.cpp
  pdfoptions.cpp
  pdfoptionsio.cpp
  pluginmanager.cpp
  prefscontext.cpp
  prefsfile.cpp
  prefsmanager.cpp
  prefsreader.cpp
  prefstable.cpp
  pslib.cpp
  rawimage.cpp
  rc4.c
  sampleitem.cpp
  scclocale.cpp
  sccolor.cpp
  sccolorengine.cpp
  sccolorshade.cpp
  scdocoutput.cpp
  scdocoutput_ps2.cpp
  scfonts.cpp
  scgtplugin.cpp
  scgzfile.cpp
  schelptreemodel.cpp
  scimage.cpp
  scimagecacheproxy.cpp
  scimagecachedir.cpp
  scimagecachefile.cpp
  scimagecachemanager.cpp
  scimagecachewriteaction.cpp
  scimagestructs.cpp
  scimgdataloader.cpp
  scimgdataloader_gimp.cpp
  scimgdataloader_jpeg.cpp
  scimgdataloader_pdf.cpp
  scimgdataloader_pgf.cpp
  scimgdataloader_pict.cpp
  scimgdataloader_ps.cpp
  scimgdataloader_psd.cpp
  scimgdataloader_qt.cpp
  scimgdataloader_tiff.cpp
  scimgdataloader_wpg.cpp
  sclayer.cpp
  sclockedfile.cpp
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
  serializer.cpp
  styleitem.cpp
  textwriter.cpp
  tocgenerator.cpp
  transaction.cpp
  undogui.cpp
  undomanager.cpp
  undoobject.cpp
  undostack.cpp
  undostate.cpp
  units.cpp
  unzip.c
  updatemanager.cpp
  upgradechecker.cpp
  urllauncher.cpp
  util.cpp
  util_color.cpp
  util_file.cpp
  util_formats.cpp
  util_ghostscript.cpp
  util_icon.cpp
  util_layer.cpp
  util_math.cpp
  util_printer.cpp
  util_text.cpp
  vgradient.cpp
  vgradientex.cpp
  ui/about.cpp
  ui/aboutplugins.cpp
  ui/adjustcmsdialog.cpp
  ui/aligndistribute.cpp
  ui/alignselect.cpp
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
  ui/cmsprefs.cpp
  ui/cmykfw.cpp
  ui/collectforoutput_ui.cpp
  ui/colorchart.cpp
  ui/colorcombo.cpp
  ui/colorlistbox.cpp
  ui/contextmenu.cpp
  ui/cpalette.cpp
  ui/cupsoptions.cpp
  ui/curvewidget.cpp
  ui/customfdialog.cpp
  ui/dasheditor.cpp
  ui/dcolor.cpp
  ui/delpages.cpp
  ui/editor.cpp
  ui/effectsdialog.cpp
  ui/extimageprops.cpp
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
  ui/insertTable.cpp
  ui/insertaframe.cpp
  ui/inspage.cpp
  ui/javadocs.cpp
  ui/latexeditor.cpp
  ui/layers.cpp
  ui/linecombo.cpp
  ui/linkbutton.cpp
  ui/loremipsum.cpp
  ui/margindialog.cpp
  ui/marginpresetlayout.cpp
  ui/marginwidget.cpp
  ui/newmarginwidget.cpp
  ui/masterpagepalette.cpp
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
  ui/oneclick.cpp
  ui/outlinepalette.cpp
  ui/pageitemattributes.cpp
  ui/pagelayout.cpp
  ui/pagepalette.cpp
  ui/pageselector.cpp
  ui/paintmanager.cpp
  ui/patternpropsdialog.cpp
  ui/pdfopts.cpp
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
  ui/query.cpp
  ui/replacecolors.cpp
  ui/replaceonecolor.cpp
  ui/rulermover.cpp
  ui/sccombobox.cpp
  ui/scfilewidget.cpp
  ui/scinputdialog.cpp
  ui/scmenu.cpp
  ui/scmessagebox.cpp
  ui/scrapbookpalette.cpp
  ui/scresizecursor.cpp
  ui/scmwmenumanager.cpp
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
  ui/spiralvectordialog.cpp
  ui/spiralwidget.cpp
  ui/splash.cpp
  ui/storyeditor.cpp
  ui/stylemanager.cpp
  ui/styleselect.cpp
  ui/styleview.cpp
  ui/swatchcombo.cpp
  ui/symbolpalette.cpp
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
  ui/unicodesearch.cpp
  ui/vruler.cpp
  ui/useprintermarginsdialog.cpp
#  ${SCRIBUS_ITEMSETTERS_SRC}
  ${SCRIBUS_OSG_SRC}
  ${SCRIBUS_GMAGICK_SRC}
)

IF(NOT HAVE_HYPHEN)
  SET(SCRIBUS_SOURCES
    ${SCRIBUS_SOURCES}
    hnjalloc.c
    hyphen.c
  )
ENDIF(NOT HAVE_HYPHEN)



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
  ${CMAKE_CURRENT_BINARY_DIR}/colormgmt
  ${CMAKE_CURRENT_BINARY_DIR}/desaxe
  ${CMAKE_CURRENT_BINARY_DIR}/fonts
  ${CMAKE_CURRENT_BINARY_DIR}/styles
  ${CMAKE_CURRENT_BINARY_DIR}/text
  ${CMAKE_CURRENT_BINARY_DIR}/third_party/wpg
  ${CMAKE_CURRENT_BINARY_DIR}/third_party/pgf
)

IF(HAVE_OSG)
  LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/third_party/prc )
ENDIF (HAVE_OSG)

IF(HAVE_BOOST)
  LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/third_party/lib2geom )
ENDIF (HAVE_BOOST)

SET(SCRIBUS_COLORMGMT_LIB "scribus_colormgmt_lib")
SET(SCRIBUS_TEXT_LIB "scribus_text_lib")
SET(SCRIBUS_STYLES_LIB "scribus_styles_lib")
SET(SCRIBUS_FONTS_LIB "scribus_fonts_lib")
SET(SCRIBUS_DESAXE_LIB "scribus_desaxe_lib")
IF(HAVE_OSG)
  SET(SCRIBUS_PRC_LIB "scribus_prc_lib")
ENDIF (HAVE_OSG)
IF(HAVE_BOOST)
  SET(SCRIBUS_2GEOM_LIB "scribus_2geom_lib")
ENDIF (HAVE_BOOST)
SET(SCRIBUS_WPG_LIB "scribus_wpg_lib")

SET(SCRIBUS_PGF_LIB "scribus_pgf_lib")

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
)

IF (GMAGICK_FOUND)
    MESSAGE(STATUS "GMAGICK: Libs: ${GMAGICK_LIBRARIES}, Inc: ${GMAGICK_INCLUDES}, Ldflags: ${GMAGICK_LDFLAGS}")
    INCLUDE_DIRECTORIES(
        BEFORE ${GMAGICK_INCLUDES}
    )
    LINK_DIRECTORIES(
        ${GMAGICK_LDFLAGS} #TODO
    )
ENDIF(GMAGICK_FOUND)

IF(HAVE_PODOFO)
  INCLUDE_DIRECTORIES(
    ${LIBPODOFO_INCLUDE_DIR}
    )
ENDIF(HAVE_PODOFO)

IF(HAVE_HYPHEN)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    ${HYPHEN_LIBRARY}
  )
ENDIF(HAVE_HYPHEN)


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

IF(HAVE_OSG)
ADD_EXECUTABLE(${EXE_NAME}
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
ELSE(HAVE_OSG)
ADD_EXECUTABLE(${EXE_NAME}
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
ENDIF (HAVE_OSG)

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
IF (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
# Use the -E / --export-dynamic flag to ensure that symbols in the Scribus
# binary are in the public symbol table for plugins. The symbols must also
# be set as SCRIBUS_API if we're building with -fvisibility=hidden .
# Should fix bug 6897 .
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES LINK_FLAGS -Wl,-E)
ENDIF (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
IF (GCC AND NOT WIN32)
# Use the -E / --export-dynamic flag to ensure that symbols in the Scribus
# binary are in the public symbol table for plugins. The symbols must also
# be set as SCRIBUS_API if we're building with -fvisibility=hidden .
# Should fix bug 6897 .
  SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES LINK_FLAGS -Wl,-E)
ENDIF (GCC AND NOT WIN32)
##


#Set up libraries to link with all found packages

IF(WANT_PRIVATE_CAIRO)
	SET(SCRIBUS_CAIRO_LIB "scribus_cairo_lib")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/cairo)
	LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/third_party/cairo)
	TARGET_LINK_LIBRARIES(${EXE_NAME} ${PIXMAN_LIBRARIES} ${SCRIBUS_CAIRO_LIB})
ELSE(WANT_PRIVATE_CAIRO)
	#Select system Cairo
	INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(${EXE_NAME} ${CAIRO_LIBRARIES})
	LINK_DIRECTORIES(${PREFIX_CAIRO_LIB})
	SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${PREFIX_CAIRO_LIB}")
ENDIF(WANT_PRIVATE_CAIRO)

MESSAGE("RPATH: ${CMAKE_INSTALL_RPATH}")

TARGET_LINK_LIBRARIES(${EXE_NAME}
  ${QT_LIBRARIES}
  ${CUPS_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  ${PYTHON_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${WIN32_ONLY_LIBS}
  ${GMAGICK_LIBRARIES}
)

# required to resolve symbols from libdl when ld is called with --no-add-needed
# #9310 for the OpenBSD addition
IF(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  TARGET_LINK_LIBRARIES(${EXE_NAME} dl)
ENDIF(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")

# #9311 for the OpenBSD addition
IF(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  TARGET_LINK_LIBRARIES(${EXE_NAME} util execinfo)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")

IF(WIN32)
  TARGET_LINK_LIBRARIES(${EXE_NAME} ${QT_QTMAIN_LIBRARY})
ENDIF(WIN32)

IF(APPLE)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    "-undefined dynamic_lookup"
    "-framework CoreFoundation" )
ENDIF(APPLE)

TARGET_LINK_LIBRARIES(${EXE_NAME}
  ${SCRIBUS_COLORMGMT_LIB}
  ${SCRIBUS_DESAXE_LIB}
  ${SCRIBUS_TEXT_LIB}
  ${SCRIBUS_STYLES_LIB}
  ${SCRIBUS_FONTS_LIB}
  ${SCRIBUS_WPG_LIB}
  ${SCRIBUS_PGF_LIB}
  ${LCMS_LIBRARIES}
)

IF(HAVE_PODOFO)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    ${LIBPODOFO_LIBRARY}
  )
ENDIF(HAVE_PODOFO)

IF(HAVE_OSG)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    ${OSG_LIBRARY}
    ${OSGGA_LIBRARY}
    ${OSGUTIL_LIBRARY}
    ${OSGDB_LIBRARY}
    ${OSGVIEWER_LIBRARY}
    ${OSGTEXT_LIBRARY}
    ${SCRIBUS_PRC_LIB}
  )
ENDIF(HAVE_OSG)

IF (HAVE_BOOST)
  TARGET_LINK_LIBRARIES(${EXE_NAME}
    ${SCRIBUS_2GEOM_LIB}
  )
ENDIF(HAVE_BOOST)

# Now build plugins
SET(PLUGIN_LIBRARIES
  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
  ${LIBXML2_LIBRARIES}
  ${ZLIB_LIBRARIES}
)
IF(WIN32)
  # IF your plugin needs more than this, please link them privately
  # in the CMakeLists.txt for your plugin.
  SET(PLUGIN_LIBRARIES
#    ${QT_LIBRARIES} part of #7210
	${PLUGIN_LIBRARIES}
    ${EXE_NAME}
    )
ENDIF(WIN32)
IF(APPLE)
  SET(PLUGIN_LIBRARIES
	${PLUGIN_LIBRARIES}
	"-undefined dynamic_lookup"
  )
ENDIF(APPLE)
MESSAGE(STATUS ${PLUGIN_LIBRARIES})

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

#Install our header files, selected from all existing dirs, excluding unwanted directories and svn files
IF(NOT WANT_NOHEADERINSTALL)
  MESSAGE(STATUS "Source header files will be installed")
  INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${INCLUDEDIR} 
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
ELSE(NOT WANT_NOHEADERINSTALL)
  MESSAGE(STATUS "No source header files will be installed")
ENDIF(NOT WANT_NOHEADERINSTALL)
