include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTDRW_PLUGIN_SOURCES
	importdrw.cpp
	importdrwplugin.cpp
)

set(SCRIBUS_IMPORTDRW_PLUGIN "importdrw")

add_library(${SCRIBUS_IMPORTDRW_PLUGIN} MODULE ${IMPORTDRW_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTDRW_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTDRW_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTDRW_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTDRW_PLUGIN})

