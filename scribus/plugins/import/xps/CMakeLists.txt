include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${ZLIB_INCLUDE_DIR}
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTXPS_PLUGIN_SOURCES
	importxps.cpp
	importxpsplugin.cpp
	xpsimportoptions.cpp
)

set(SCRIBUS_IMPORTXPS_PLUGIN "importxps")

add_library(${SCRIBUS_IMPORTXPS_PLUGIN} MODULE ${IMPORTXPS_PLUGIN_SOURCES})

link_directories( /usr/local/lib )
target_link_libraries(${SCRIBUS_IMPORTXPS_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTXPS_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTXPS_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTXPS_PLUGIN})

# set_target_properties(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")
