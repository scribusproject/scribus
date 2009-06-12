INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

SET(PICTUREBROWSER_PLUGIN_UI_SRC
picturebrowser.ui
imagedialog.ui
)

SET(PICTUREBROWSER_PLUGIN_MOC_CLASSES
collection.h
imagedialog.h
loadimage.h
multicombobox.h
picturebrowserplugin.h
picturebrowser.h
previewimage.h
)

SET(PICTUREBROWSER_PLUGIN_SOURCES
collection.cpp
findimage.cpp
imagedialog.cpp
iview.cpp
loadimage.cpp
multicombobox.cpp
picturebrowser.cpp
picturebrowserplugin.cpp
pictview.cpp
previewimage.cpp
)

SET(SCRIBUS_PICTUREBROWSER_PLUGIN "picturebrowser")

QT4_WRAP_UI(PICTUREBROWSER_PLUGIN_UI_SOURCES ${PICTUREBROWSER_PLUGIN_UI_SRC} )
QT4_WRAP_CPP(PICTUREBROWSER_PLUGIN_MOC_SOURCES ${PICTUREBROWSER_PLUGIN_MOC_CLASSES})

ADD_LIBRARY(${SCRIBUS_PICTUREBROWSER_PLUGIN} MODULE ${PICTUREBROWSER_PLUGIN_SOURCES}
${PICTUREBROWSER_PLUGIN_MOC_SOURCES} ${PICTUREBROWSER_PLUGIN_UI_SOURCES})

TARGET_LINK_LIBRARIES(${SCRIBUS_PICTUREBROWSER_PLUGIN} ${PLUGIN_LIBRARIES})

INSTALL(TARGETS ${SCRIBUS_PICTUREBROWSER_PLUGIN}
  LIBRARY
  DESTINATION ${PLUGINDIR}
  PERMISSIONS ${PLUGIN_PERMISSIONS}
)

INSTALL(FILES icons/pitr_green_arrows_set_1.svg icons/pitr_green_arrows_set_5.svg icons/sarxos_Paper_Sheets.svg icons/sarxos_Simple_Folder_Seek.svg icons/sarxos_Simple_Green_Yellow_Blue_Violet_Folders.svg icons/testfill.png
  DESTINATION ${SHAREDIR}/icons
)

INSTALL(FILES collectionsdb.xml DESTINATION ${SHAREDIR}/plugins)

# SET_TARGET_PROPERTIES(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")
