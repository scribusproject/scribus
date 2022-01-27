include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTPCT_PLUGIN_SOURCES
	importpct.cpp
	importpctplugin.cpp
)

set(SCRIBUS_IMPORTPCT_PLUGIN "importpct")

add_library(${SCRIBUS_IMPORTPCT_PLUGIN} MODULE ${IMPORTPCT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTPCT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTPCT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTPCT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTPCT_PLUGIN})
