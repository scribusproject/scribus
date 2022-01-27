include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(PATHCONNECT_PLUGIN_SOURCES
	pathconnectdialog.cpp
	pathconnect.cpp
)

set(SCRIBUS_PATHCONNECT_PLUGIN "pathconnect")

add_library(${SCRIBUS_PATHCONNECT_PLUGIN} MODULE ${PATHCONNECT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_PATHCONNECT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_PATHCONNECT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_PATHCONNECT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
