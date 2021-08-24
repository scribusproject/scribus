include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTCVG_PLUGIN_SOURCES
	importcvg.cpp
	importcvgplugin.cpp
)

set(SCRIBUS_IMPORTCVG_PLUGIN "importcvg")

add_library(${SCRIBUS_IMPORTCVG_PLUGIN} MODULE ${IMPORTCVG_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTCVG_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTCVG_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTCVG_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTCVG_PLUGIN})

