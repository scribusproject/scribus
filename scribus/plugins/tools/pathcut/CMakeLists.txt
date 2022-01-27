include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

install(FILES
	transform-crop-and-resize.png
	DESTINATION ${ICONDIR}/1_5_1/
)

set(PATHCUT_PLUGIN_SOURCES
	pathcut.cpp
)

set(SCRIBUS_PATHCUT_PLUGIN "pathcut")



add_library(${SCRIBUS_PATHCUT_PLUGIN} MODULE ${PATHCUT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_PATHCUT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_PATHCUT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_PATHCUT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
