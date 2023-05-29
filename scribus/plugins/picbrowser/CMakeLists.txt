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

file( GLOB PICBROWSER_SVG_ICONS icons/*.svg )

install(FILES icons/testfill.png DESTINATION ${SHAREDIR}/icons/1_5_0)
install(FILES ${PICBROWSER_SVG_ICONS} DESTINATION ${SHAREDIR}/icons/1_5_0/svg)

install(FILES icons/testfill.png DESTINATION ${SHAREDIR}/icons/1_5_1)
install(FILES ${PICBROWSER_SVG_ICONS} DESTINATION ${SHAREDIR}/icons/1_5_1/svg)

install(FILES icons/testfill.png DESTINATION ${SHAREDIR}/icons/1_5_1_dark)
install(FILES ${PICBROWSER_SVG_ICONS} DESTINATION ${SHAREDIR}/icons/1_5_1_dark/svg)

install(FILES icons/testfill.png DESTINATION ${SHAREDIR}/icons/1_7_0)
install(FILES ${PICBROWSER_SVG_ICONS} DESTINATION ${SHAREDIR}/icons/1_7_0/svg)

install(FILES collectionsdb.xml DESTINATION ${SHAREDIR}/plugins)
