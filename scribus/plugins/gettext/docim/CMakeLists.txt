include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(DOC_GT_PLUGIN_SOURCES
	docim.cpp
)

set(SCRIBUS_DOC_GT_PLUGIN "docim")

add_library(${SCRIBUS_DOC_GT_PLUGIN} MODULE ${DOC_GT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_DOC_GT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_DOC_GT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_DOC_GT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
