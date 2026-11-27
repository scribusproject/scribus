include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(RST_IM_PLUGIN_SOURCES
	rstparser.cpp
	rstim.cpp
)

set(SCRIBUS_RST_IM_PLUGIN "rstimplugin")

add_library(${SCRIBUS_RST_IM_PLUGIN} MODULE ${RST_IM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_RST_IM_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_RST_IM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_RST_IM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}/gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

