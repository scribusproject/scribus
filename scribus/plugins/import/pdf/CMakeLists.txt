include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${poppler_INCLUDE_DIR}
	${poppler_CPP_INCLUDE_DIR}
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTPDF_PLUGIN_SOURCES
	importpdf.cpp
	importpdfplugin.cpp
	pdfimportoptions.cpp
	pdftextrecognition.cpp
	slaoutput.cpp
)

if(HAVE_POPPLER)
	if(HAVE_LCMS1)
		add_definitions(-DUSE_CMS)
	endif()
	add_definitions(-DHAVE_FREETYPE_H)
endif()

set(SCRIBUS_IMPORTPDF_PLUGIN "importpdf")

add_library(${SCRIBUS_IMPORTPDF_PLUGIN} MODULE ${IMPORTPDF_PLUGIN_SOURCES})

link_directories( /usr/local/lib )
target_link_libraries(${SCRIBUS_IMPORTPDF_PLUGIN} ${EXE_NAME} ${poppler_LIBRARY})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTPDF_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTPDF_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTPDF_PLUGIN})
