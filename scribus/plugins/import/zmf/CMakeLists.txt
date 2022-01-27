include(FindPkgConfig)
if (HAVE_REVENGE)
	pkg_check_modules(LIBZMF libzmf-0.0)
	if (LIBZMF_FOUND)
		message("Building Zoner Draw Importer")
		include_directories(
			${CMAKE_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/scribus
			${LIBREVENGE_INCLUDE_DIRS}
			${LIBREVENGE_GENERATORS_DIRS}
			${LIBREVENGE_STREAM_INCLUDE_DIRS}
			${LIBZMF_INCLUDE_DIRS}
			${SCRIBUS_AUTOGEN_INCLUDE_PATH}
			)
		set(IMPORTZMF_PLUGIN_SOURCES ../revenge/rawpainter.cpp importzmf.cpp importzmfplugin.cpp)
		set(SCRIBUS_IMPORTZMF_PLUGIN "importzmf")
		add_library(${SCRIBUS_IMPORTZMF_PLUGIN} MODULE ${IMPORTZMF_PLUGIN_SOURCES} ${IMPORTZMF_PLUGIN_MOC_SOURCES})
		target_link_libraries(${SCRIBUS_IMPORTZMF_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBZMF_LDFLAGS})
		if(WANT_PCH)
			target_precompile_headers(${SCRIBUS_IMPORTZMF_PLUGIN} PRIVATE "../../plugins_pch.h")
		endif()
		install(TARGETS ${SCRIBUS_IMPORTZMF_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
		add_dependencies(Plugins ${SCRIBUS_IMPORTZMF_PLUGIN})
	endif ()
endif ()
