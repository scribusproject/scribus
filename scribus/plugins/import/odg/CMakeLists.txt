include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${ZLIB_INCLUDE_DIR}
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTODG_PLUGIN_SOURCES
	importodg.cpp
	importodgplugin.cpp
)

set(SCRIBUS_IMPORTODG_PLUGIN "importodg")

add_library(${SCRIBUS_IMPORTODG_PLUGIN} MODULE ${IMPORTODG_PLUGIN_SOURCES})

link_directories( /usr/local/lib )
target_link_libraries(${SCRIBUS_IMPORTODG_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTODG_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTODG_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTODG_PLUGIN})

