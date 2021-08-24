include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTCGM_PLUGIN_SOURCES
	importcgm.cpp
	importcgmplugin.cpp
)

set(SCRIBUS_IMPORTCGM_PLUGIN "importcgm")

add_library(${SCRIBUS_IMPORTCGM_PLUGIN} MODULE ${IMPORTCGM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTCGM_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTCGM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTCGM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTCGM_PLUGIN})
