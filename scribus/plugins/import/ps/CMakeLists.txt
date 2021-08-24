include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${PODOFO_INCLUDES}
)

if(HAVE_PODOFO)
	include_directories(
		${LIBPODOFO_INCLUDE_DIR}
	)
endif()

set(IMPORTPS_PLUGIN_SOURCES
	importps.cpp
	importpsplugin.cpp
)

set(SCRIBUS_IMPORTPS_PLUGIN "importps")

add_library(${SCRIBUS_IMPORTPS_PLUGIN} MODULE ${IMPORTPS_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTPS_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTPS_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

if(HAVE_PODOFO)
	target_link_libraries(${SCRIBUS_IMPORTPS_PLUGIN}
		${LIBPODOFO_LIBRARY}
	)
endif()

install(TARGETS ${SCRIBUS_IMPORTPS_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

install(FILES 
	import.prolog
	DESTINATION ${LIBDIR}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTPS_PLUGIN})
