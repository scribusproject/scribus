include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTEMF_PLUGIN_SOURCES
	importemf.cpp
	importemfplugin.cpp
)

set(SCRIBUS_IMPORTEMF_PLUGIN "importemf")

add_library(${SCRIBUS_IMPORTEMF_PLUGIN} MODULE ${IMPORTEMF_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTEMF_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTEMF_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTEMF_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTEMF_PLUGIN})
