include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	)


set(IMPORTSVG_PLUGIN_SOURCES
	svgplugin.cpp
)

set(SCRIBUS_IMPORTSVG_PLUGIN "svgimplugin")

add_library(${SCRIBUS_IMPORTSVG_PLUGIN} MODULE ${IMPORTSVG_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTSVG_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTSVG_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTSVG_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTSVG_PLUGIN})
