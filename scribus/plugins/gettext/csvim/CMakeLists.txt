include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(CSV_GT_PLUGIN_SOURCES
	csvim.cpp
	csvdia.cpp
)

set(SCRIBUS_CSV_GT_PLUGIN "csvim")



add_library(${SCRIBUS_CSV_GT_PLUGIN} MODULE ${CSV_GT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_CSV_GT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_CSV_GT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_CSV_GT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
