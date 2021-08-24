include(FindPkgConfig)
if (HAVE_REVENGE)
	pkg_check_modules(LIBPAGEMAKER libpagemaker-0.0)
	if (LIBPAGEMAKER_FOUND)
		message("Building Pagemaker Importer with librevenge")
		include_directories(
			${CMAKE_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/scribus
			${LIBREVENGE_INCLUDE_DIRS}
			${LIBREVENGE_GENERATORS_INCLUDE_DIRS}
			${LIBREVENGE_STREAM_INCLUDE_DIRS}
			${LIBPAGEMAKER_INCLUDE_DIRS}
			${SCRIBUS_AUTOGEN_INCLUDE_PATH}
			)
		set(IMPORTPM_PLUGIN_SOURCES ../revenge/rawpainter.cpp importpm.cpp importpmplugin.cpp)
		set(SCRIBUS_IMPORTPM_PLUGIN "importpm")

		add_library(${SCRIBUS_IMPORTPM_PLUGIN} MODULE ${IMPORTPM_PLUGIN_SOURCES})
		target_link_libraries(${SCRIBUS_IMPORTPM_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBPAGEMAKER_LDFLAGS})
		if(WANT_PCH)
			target_precompile_headers(${SCRIBUS_IMPORTPM_PLUGIN} PRIVATE "../../plugins_pch.h")
		endif()
		install(TARGETS ${SCRIBUS_IMPORTPM_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
		add_dependencies(Plugins ${SCRIBUS_IMPORTPM_PLUGIN})
	endif ()
endif ()
