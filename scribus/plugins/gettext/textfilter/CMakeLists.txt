include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(TEXTFILTER_GT_PLUGIN_SOURCES
	textfilter.cpp
	tffilter.cpp
	tfdia.cpp
)

set(SCRIBUS_TEXTFILTER_GT_PLUGIN "textfilterplugin")

add_library(${SCRIBUS_TEXTFILTER_GT_PLUGIN} MODULE ${TEXTFILTER_GT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_TEXTFILTER_GT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_TEXTFILTER_GT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_TEXTFILTER_GT_PLUGIN}
LIBRARY
DESTINATION ${PLUGINDIR}gettext
PERMISSIONS ${PLUGIN_PERMISSIONS}
)
