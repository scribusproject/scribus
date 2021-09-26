include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(SXW_IM_PLUGIN_SOURCES
	contentreader.cpp
	sxwdia.cpp
	sxwim.cpp
	stylereader.cpp
)

set(SCRIBUS_SXW_IM_PLUGIN "sxwimplugin")

add_library(${SCRIBUS_SXW_IM_PLUGIN} MODULE ${SXW_IM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_SXW_IM_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SXW_IM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_SXW_IM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
