include(FindPkgConfig)
if (HAVE_REVENGE)
	pkg_check_modules(LIBQXP libqxp-0.0)
	if (LIBQXP_FOUND)
		message("Building QuarkXPress Importer")
		include_directories(
			${CMAKE_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/scribus
			${LIBREVENGE_INCLUDE_DIRS}
			${LIBREVENGE_GENERATORS_INCLUDE_DIRS}
			${LIBREVENGE_STREAM_INCLUDE_DIRS}
			${LIBQXP_INCLUDE_DIRS}
			${SCRIBUS_AUTOGEN_INCLUDE_PATH}
			)
		set(IMPORTQXP_PLUGIN_SOURCES ../revenge/rawpainter.cpp importqxp.cpp importqxpplugin.cpp)
		set(SCRIBUS_IMPORTQXP_PLUGIN "importqxp")

		add_library(${SCRIBUS_IMPORTQXP_PLUGIN} MODULE ${IMPORTQXP_PLUGIN_SOURCES})
		target_link_libraries(${SCRIBUS_IMPORTQXP_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBQXP_LDFLAGS})
		if(WANT_PCH)
			target_precompile_headers(${SCRIBUS_IMPORTQXP_PLUGIN} PRIVATE "../../plugins_pch.h")
		endif()
		install(TARGETS ${SCRIBUS_IMPORTQXP_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
		add_dependencies(Plugins ${SCRIBUS_IMPORTQXP_PLUGIN})
	endif ()
endif ()
