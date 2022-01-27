include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

install(FILES
	pathexclusion.png
	pathintersection.png
	pathparts.png
	pathsubtraction.png
	pathunite.png
	DESTINATION ${ICONDIR}/1_5_1/
)

set(PATHFINDER_PLUGIN_SOURCES
	pathfinder.cpp
	pathfinderdialog.cpp
)

set(SCRIBUS_PATHFINDER_PLUGIN "pathfinder")


add_library(${SCRIBUS_PATHFINDER_PLUGIN} MODULE ${PATHFINDER_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_PATHFINDER_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_PATHFINDER_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_PATHFINDER_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
