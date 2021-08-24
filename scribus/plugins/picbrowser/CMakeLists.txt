include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
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


add_library(${SCRIBUS_PICTUREBROWSER_PLUGIN} MODULE ${PICTUREBROWSER_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_PICTUREBROWSER_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_PICTUREBROWSER_PLUGIN} PRIVATE "../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_PICTUREBROWSER_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

install(FILES icons/pitr_green_arrows_set_1.svg icons/pitr_green_arrows_set_5.svg icons/sarxos_Paper_Sheets.svg icons/sarxos_Simple_Folder_Seek.svg icons/sarxos_Simple_Green_Yellow_Blue_Violet_Folders.svg icons/testfill.png
	DESTINATION ${SHAREDIR}/icons
)

install(FILES collectionsdb.xml DESTINATION ${SHAREDIR}/plugins)
