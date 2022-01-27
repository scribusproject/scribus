include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(DOCX_IM_PLUGIN_SOURCES
	docxim.cpp
)

set(SCRIBUS_DOCX_IM_PLUGIN "docximplugin")

add_library(${SCRIBUS_DOCX_IM_PLUGIN} MODULE ${DOCX_IM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_DOCX_IM_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_DOCX_IM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_DOCX_IM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
