include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(HTML_GT_PLUGIN_SOURCES
	htmlim.cpp
	htmlreader.cpp
)

set(SCRIBUS_HTML_GT_PLUGIN "htmlim")



add_library(${SCRIBUS_HTML_GT_PLUGIN} MODULE ${HTML_GT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_HTML_GT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_HTML_GT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_HTML_GT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
