include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(PICTUREBROWSER_PLUGIN_UI_SRC
	picturebrowser.ui
	imagedialog.ui
)

set(PICTUREBROWSER_PLUGIN_MOC_CLASSES
	collection.h
	findimage.h
	imagedialog.h
	iview.h
	loadimage.h
	multicombobox.h
	picturebrowserplugin.h
	picturebrowser.h
	pictview.h
	previewimage.h
)

set(PICTUREBROWSER_PLUGIN_SOURCES
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

set(SCRIBUS_PICTUREBROWSER_PLUGIN "picturebrowser")

QT5_WRAP_UI(PICTUREBROWSER_PLUGIN_UI_SOURCES ${PICTUREBROWSER_PLUGIN_UI_SRC} )
QT5_WRAP_CPP(PICTUREBROWSER_PLUGIN_MOC_SOURCES ${PICTUREBROWSER_PLUGIN_MOC_CLASSES})

add_library(${SCRIBUS_PICTUREBROWSER_PLUGIN} MODULE ${PICTUREBROWSER_PLUGIN_SOURCES}
${PICTUREBROWSER_PLUGIN_MOC_SOURCES} ${PICTUREBROWSER_PLUGIN_UI_SOURCES})

target_link_libraries(${SCRIBUS_PICTUREBROWSER_PLUGIN} ${PLUGIN_LIBRARIES})

install(TARGETS ${SCRIBUS_PICTUREBROWSER_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

install(FILES icons/pitr_green_arrows_set_1.svg icons/pitr_green_arrows_set_5.svg icons/sarxos_Paper_Sheets.svg icons/sarxos_Simple_Folder_Seek.svg icons/sarxos_Simple_Green_Yellow_Blue_Violet_Folders.svg icons/testfill.png
	DESTINATION ${SHAREDIR}/icons
)

install(FILES collectionsdb.xml DESTINATION ${SHAREDIR}/plugins)

# set_target_properties(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")
