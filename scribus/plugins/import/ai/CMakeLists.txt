include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${PODOFO_INCLUDES}
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

if(HAVE_PODOFO)
	if(OPENSSL_FOUND)
		include_directories(${OPENSSL_INCLUDE_DIR})
	endif()
	include_directories(BEFORE ${LIBPODOFO_INCLUDE_DIR})
endif()

set(IMPORTAI_PLUGIN_SOURCES
	importai.cpp
	importaiplugin.cpp
)

set(SCRIBUS_IMPORTAI_PLUGIN "importai")

add_library(${SCRIBUS_IMPORTAI_PLUGIN} MODULE ${IMPORTAI_PLUGIN_SOURCES})

if(WIN32)
	target_link_libraries(${SCRIBUS_IMPORTAI_PLUGIN}
		${EXE_NAME}
		${TIFF_LIBRARIES}
		${PNG_LIBRARIES}
		${JPEG_LIBRARIES}
	)
else()
	target_link_libraries(${SCRIBUS_IMPORTAI_PLUGIN} ${EXE_NAME})
endif()

if(HAVE_PODOFO)
	target_link_libraries(${SCRIBUS_IMPORTAI_PLUGIN}
		${LIBPODOFO_LIBRARY}
	)
endif()

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTAI_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTAI_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTAI_PLUGIN})
