include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(ODT_IM_PLUGIN_SOURCES
	importodt.cpp
)

set(SCRIBUS_ODT_IM_PLUGIN "odtimplugin")

add_library(${SCRIBUS_ODT_IM_PLUGIN} MODULE ${ODT_IM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_ODT_IM_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_ODT_IM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_ODT_IM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

