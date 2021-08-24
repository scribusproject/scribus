include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTSML_PLUGIN_SOURCES
	importsml.cpp
	importsmlplugin.cpp
)

set(SCRIBUS_IMPORTSML_PLUGIN "importsml")

add_library(${SCRIBUS_IMPORTSML_PLUGIN} MODULE ${IMPORTSML_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTSML_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTSML_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTSML_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTSML_PLUGIN})
