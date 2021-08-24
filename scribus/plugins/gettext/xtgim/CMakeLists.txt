include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(XTG_IM_PLUGIN_SOURCES
	xtgim.cpp
	xtgscanner.cpp
)

set(SCRIBUS_XTG_IM_PLUGIN "xtgimplugin")

add_library(${SCRIBUS_XTG_IM_PLUGIN} MODULE ${XTG_IM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_XTG_IM_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_XTG_IM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_XTG_IM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

# set_target_properties(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")
