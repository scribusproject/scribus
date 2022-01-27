#include(FindPkgConfig)
#if (HAVE_REVENGE)
#	pkg_check_modules(LIBETONYEK libetonyek-0.1)
#	if (LIBETONYEK_FOUND)
#		message("Building Pages Importer with librevenge")
#		include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/scribus ${LIBREVENGE_INCLUDE_DIRS} ${LIBREVENGE_GENERATORS_INCLUDE_DIRS} ${LIBREVENGE_STREAM_INCLUDE_DIRS} ${LIBETONYEK_INCLUDE_DIRS})
#		set(IMPORTPAGES_PLUGIN_MOC_CLASSES importpages.h importpagesplugin.h)
#		set(IMPORTPAGES_PLUGIN_SOURCES ../revenge/rawpainter.cpp importpages.cpp importpagesplugin.cpp)
#		set(IMPORTPAGES_PLUGIN "importpages")

#		add_library(${IMPORTPAGES_PLUGIN} MODULE ${IMPORTPAGES_PLUGIN_SOURCES} ${IMPORTPAGES_PLUGIN_MOC_SOURCES})
#		target_link_libraries(${IMPORTPAGES_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBETONYEK_LDFLAGS})
#		install(TARGETS ${IMPORTPAGES_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
#		add_dependencies(Plugins ${IMPORTPAGES_PLUGIN})
#	endif ()
#endif ()

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${ZLIB_INCLUDE_DIR}
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTPAGES_PLUGIN_SOURCES
	importpages.cpp
	importpagesplugin.cpp
)

set(SCRIBUS_IMPORTPAGES_PLUGIN "importpages")

add_library(${SCRIBUS_IMPORTPAGES_PLUGIN} MODULE ${IMPORTPAGES_PLUGIN_SOURCES})

link_directories( /usr/local/lib )
target_link_libraries(${SCRIBUS_IMPORTPAGES_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTPAGES_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTPAGES_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTPAGES_PLUGIN})

