#For those that want it, show the SVN version in the About dialog
if (WANT_SVNVERSION)
	add_definitions(-DHAVE_SVNVERSION)
	message(STATUS "SVN Version requested in About dialog")
	# a custom target that is always built

	if (NOT CMAKE_VERSION VERSION_LESS "3.2.0")
		add_custom_target(svnheader ALL COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_SOURCE_DIR}/cmake/modules/getsvn.cmake BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h)
	else ()
		add_custom_target(svnheader ALL DEPENDS svn_header ) # svn_header is nothing more than a unique string
		# creates svnheader.h using cmake script
		add_custom_command(OUTPUT svn_header ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
		COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_MODULE_PATH}/getsvn.cmake)
		set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
			PROPERTIES GENERATED TRUE
			HEADER_FILE_ONLY TRUE)
	endif ()
endif ()

if(WIN32)
	add_definitions(-DQT_DLL)
endif()


include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${HARFBUZZ_INCLUDE_DIRS}
	${HARFBUZZ_SUBSET_INCLUDE_DIRS}
	${hunspell_INCLUDE_DIRS}
)

include(CMakeLists_Sources.txt)
include(api/CMakeLists.txt)
include(colormgmt/CMakeLists.txt)
include(desaxe/CMakeLists.txt)
include(fonts/CMakeLists.txt)
include(styles/CMakeLists.txt)
include(text/CMakeLists.txt)

add_subdirectory(dtd)
# add_subdirectory(ui/qml)
if(WITH_TESTS)
	add_subdirectory(tests)
endif()
if (WANT_DESIGNER)
	add_subdirectory(designer)
endif ()
add_subdirectory(third_party)





set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

set_property(SOURCE third_party/fparser/file.hh PROPERTY SKIP_AUTOGEN ON)
set_property(SOURCE third_party/fparser/fparser.hh PROPERTY SKIP_AUTOGEN ON)

#if(WANT_QTADS)
  MESSAGE(STATUS "Qt Advanced Docking System included")
  link_directories(${CMAKE_CURRENT_BINARY_DIR}/third_party/Qt-Advanced-Docking-System)
#endif()

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}/third_party/wpg
	${CMAKE_CURRENT_BINARY_DIR}/third_party/pgf
	${CMAKE_CURRENT_BINARY_DIR}/third_party/rtf-qt
	${CMAKE_CURRENT_BINARY_DIR}/third_party/zip
	${HARFBUZZ_LIBRARY_DIRS}
	${HARFBUZZ_SUBSET_LIBRARY_DIRS}
	${hunspell_LIBRARY_DIRS}
	${ICU_LIBRARY_DIRS}
)

if(WITH_TESTS)
	link_directories( ${CMAKE_CURRENT_BINARY_DIR}/tests )
	set(SCRIBUS_TESTS_LIB "scribus_tests_lib")
else()
	set(SCRIBUS_TESTS_LIB "")
endif()

if(HAVE_OSG)
	link_directories( ${CMAKE_CURRENT_BINARY_DIR}/third_party/prc )
	set(SCRIBUS_PRC_LIB "scribus_prc_lib")
else()
	set(SCRIBUS_PRC_LIB "")
endif()

if(HAVE_BOOST)
	link_directories( ${CMAKE_CURRENT_BINARY_DIR}/third_party/lib2geom )
	set(SCRIBUS_2GEOM_LIB "scribus_2geom_lib")
else()
	set(SCRIBUS_2GEOM_LIB "")
endif()

set(SCRIBUS_WPG_LIB "scribus_wpg_lib")
set(SCRIBUS_PGF_LIB "scribus_pgf_lib")
set(SCRIBUS_ZIP_LIB "scribus_zip_lib")
set(SCRIBUS_RTF_LIB "scribus_rtf_lib")

if (GMAGICK_FOUND)
	message(STATUS "GMAGICK: Libs: ${GMAGICK_LDFLAGS}, Inc: ${GMAGICK_INCLUDE_DIRS}, Ldflags: ${GMAGICK_LDFLAGS}")
	include_directories(
		BEFORE ${GMAGICK_INCLUDE_DIRS}
	)
	link_directories(
		${GMAGICK_LIBRARY}
	)
endif()

if(HAVE_PODOFO)
	if(OPENSSL_FOUND)
		message(STATUS "Including OpenSSL files for PoDoFo >= 0.9.5")
		include_directories(${OPENSSL_INCLUDE_DIR})
	else()
#		message(STATUS "Not including OpenSSL files because this system's PoDoFo does not require them")
	endif()
	include_directories(${LIBPODOFO_INCLUDE_DIR})

endif()

if(HAVE_HYPHEN)
	target_link_libraries(${EXE_NAME} PRIVATE ${HYPHEN_LIBRARY})
endif()


if(GESTURE_FRAME_PREVIEW)
	 message(STATUS "Enable gesture frame preview [experimental]")
	 set(GESTURE_FRAME_PREVIEW_SOURCES	pageitempreview.cpp)
	 add_definitions(-DGESTURE_FRAME_PREVIEW)
endif()

if(APPLEBUNDLE)
	set(EXE_NAME_PREFIX "Scribus")
else()
	set(EXE_NAME_PREFIX "scribus")
endif()
set(EXE_NAME "${EXE_NAME_PREFIX}${TAG_VERSION}" CACHE STRING "Scribus version")

if (NOT DEFINED CMAKE_GLOBAL_AUTOGEN_TARGET_NAME)
	set(SCRIBUS_AUTOGEN_INCLUDE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}_autogen/include")
else()
	set(SCRIBUS_AUTOGEN_INCLUDE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}_${CMAKE_GLOBAL_AUTOGEN_TARGET_NAME}/include")
endif()
message(STATUS "Autogen Include Path: ${SCRIBUS_AUTOGEN_INCLUDE_PATH}")

if(HAVE_OSG)
	add_executable(${EXE_NAME}
		${SCRIBUS_MAIN_CPP}
		${SCRIBUS_SOURCES}
		${SCRIBUS_API_SOURCES}
		${SCRIBUS_COLORMGMT_SOURCES}
		${SCRIBUS_DESAXE_SOURCES}
		${SCRIBUS_FONTS_SOURCES}
		${SCRIBUS_STYLES_SOURCES}
		${SCRIBUS_TEXT_SOURCES}
		${SCRIBUS_WIN32_ONLY_SOURCES}
		${GESTURE_FRAME_PREVIEW_SOURCES}
		${SCRIBUS_PRC_SOURCES}
	)
else()
	add_executable(${EXE_NAME}
		${SCRIBUS_MAIN_CPP}
		${SCRIBUS_SOURCES}
		${SCRIBUS_API_SOURCES}
		${SCRIBUS_COLORMGMT_SOURCES}
		${SCRIBUS_DESAXE_SOURCES}
		${SCRIBUS_FONTS_SOURCES}
		${SCRIBUS_STYLES_SOURCES}
		${SCRIBUS_TEXT_SOURCES}
		${SCRIBUS_WIN32_ONLY_SOURCES}
		${GESTURE_FRAME_PREVIEW_SOURCES}
	)
endif ()

if(WANT_PCH)
	target_precompile_headers(${EXE_NAME} PRIVATE scribus_pch.h)
endif()

if(WIN32)
	set_target_properties(${EXE_NAME}
		PROPERTIES
		COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
		ENABLE_EXPORTS TRUE
	)
	set(WIN32_ONLY_LIBS mscms.lib)
else()
	set(WIN32_ONLY_LIBS)
endif()

##


#Set up libraries to link with all found packages
include_directories(${cairo_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} PRIVATE ${cairo_LIBRARIES})
link_directories(${PREFIX_cairo_LIB})
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${PREFIX_cairo_LIB}")

message("RPATH: ${CMAKE_INSTALL_RPATH}")

target_link_libraries(${EXE_NAME}
	PUBLIC
	Qt6::Core
	Qt6::Core5Compat
	Qt6::Gui
	Qt6::Widgets
	Qt6::Network
	Qt6::Xml
	Qt6::PrintSupport
	Qt6::Svg
	)

if (HAVE_OSG)
	target_link_libraries(${EXE_NAME}
		PUBLIC
		Qt6::OpenGL
		Qt6::OpenGLWidgets
		)
endif()

target_link_libraries(${EXE_NAME}
	PUBLIC
	${LIBXML2_LIBRARIES}
	${ZLIB_LIBRARIES}
	PRIVATE
	${CUPS_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${GMAGICK_LIBRARIES}
	${HARFBUZZ_LIBRARIES}
	${HARFBUZZ_SUBSET_LIBRARIES}
	${ICU_LIBRARIES}
	${JPEG_LIBRARIES}
	${LCMS2_LIBRARIES}
	${PNG_LIBRARIES}
	${TIFF_LIBRARIES}
	${WIN32_ONLY_LIBS}
)

if (TARGET Fontconfig::Fontconfig)
	target_link_libraries(${EXE_NAME} PRIVATE Fontconfig::Fontconfig)
endif()

if (NOT WIN32 AND NOT HAIKU)
	target_link_libraries(${EXE_NAME} PRIVATE ${CUPS_LIBRARIES})
endif ()

# required to resolve symbols from libdl when ld is called with --no-add-needed
# #9310 for the OpenBSD addition
if(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND NOT HAIKU)
  target_link_libraries(${EXE_NAME} PUBLIC ${CMAKE_DL_LIBS})
endif()

# #9311 for the OpenBSD addition
if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
  target_link_libraries(${EXE_NAME} PRIVATE util execinfo)
endif()

if(HAIKU)
  target_link_libraries(${EXE_NAME} PRIVATE execinfo)
endif(HAIKU)

if(WIN32)
  target_link_libraries(${EXE_NAME} PRIVATE ${QT_QTMAIN_LIBRARY})
endif()

if(APPLE)
	target_link_libraries(${EXE_NAME}
		PUBLIC
		"-undefined dynamic_lookup"
		PRIVATE
		"-framework CoreFoundation"
		)
	target_link_options(${EXE_NAME}
		PUBLIC
		"-Wl,-no_fixup_chains"
	)
endif()

if (WANT_SVNVERSION)
	add_dependencies(${EXE_NAME} svnheader)
endif ()


target_link_libraries(${EXE_NAME} PRIVATE
	${SCRIBUS_WPG_LIB}
	${SCRIBUS_PGF_LIB}
	${SCRIBUS_ZIP_LIB}
	${SCRIBUS_RTF_LIB}
)

#if(WANT_QTADS)
  target_link_libraries(${EXE_NAME} PUBLIC qt6advanceddocking)
#endif()

if(WITH_TESTS)
	target_link_libraries(${EXE_NAME} PRIVATE
		${QT_QTTEST_LIBRARY}
		${SCRIBUS_TESTS_LIB}
	)
endif()

if(HAVE_PODOFO)
	target_link_libraries(${EXE_NAME} PRIVATE ${LIBPODOFO_LIBRARY})
endif()

if(HAVE_JXL)
	target_link_libraries(${EXE_NAME} PRIVATE ${JXL_LIBRARY} ${JXL_THREADS_LIBRARY})
endif()

if(HAVE_OSG)
	target_link_libraries(${EXE_NAME} PRIVATE
		${OPENTHREADS_LIBRARY}
		${OSG_LIBRARY}
		${OSGGA_LIBRARY}
		${OSGUTIL_LIBRARY}
		${OSGDB_LIBRARY}
		${OSGVIEWER_LIBRARY}
		${OSGTEXT_LIBRARY}
		${SCRIBUS_PRC_LIB}
	)
endif()

if (HAVE_BOOST)
	target_link_libraries(${EXE_NAME} PRIVATE ${SCRIBUS_2GEOM_LIB})
endif()

# Now build plugins

# IF your plugin needs additional libraries, please link them privately
# in the CMakeLists.txt for your plugin.

add_custom_target(Plugins ALL)

target_sources(Plugins
  PRIVATE
    designer/uiwidgets.h designer/uiwidgets.cpp
    designer/formwidget_plugin.h designer/formwidget_plugin.cpp
    designer/sectioncontainer_extension.h designer/sectioncontainer_extension.cpp
    designer/sectioncontainer_extensionfactory.h designer/sectioncontainer_extensionfactory.cpp
    designer/sectioncontainer_plugin.h designer/sectioncontainer_plugin.cpp
)
add_subdirectory(plugins)

#do we want a lib suffix on osx? doubt it
if(APPLEBUNDLE)
	install(DIRECTORY DESTINATION MacOS)
	install(TARGETS ${EXE_NAME}
	RUNTIME DESTINATION MacOS
	LIBRARY DESTINATION lib
	)
#	add_custom_command(TARGET ${EXE_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} ARGS -Dinput_file=${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.app/Contents/MacOS/${EXE_NAME} -P ${CMAKE_MODULE_PATH}/ScribusIngestOSXBundleLibraries.cmake
#	)
#message(STATUS "Installing apple bundle")

#	set(APPS "${APPLE_APP_DIR}")
#	set(DIRS ${QT_LIBRARY_DIRS})
#	install(CODE "
#		file(GLOB_RECURSE QTPLUGINS
#		  \"\${CMAKE_INSTALL_PREFIX}/${LIBDIR}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#		include(BundleUtilities)
#		fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#		" COMPONENT Runtime)

else()
	if(WIN32)
		SET(RUNTIME_DEST ".")
	else()
		SET(RUNTIME_DEST "bin")
	endif()
	install(TARGETS ${EXE_NAME}
		RUNTIME DESTINATION ${RUNTIME_DEST}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
endif()

#Install our header files, selected from all existing dirs, excluding unwanted directories and svn files
if(WANT_HEADERINSTALL)
	message(STATUS "Source header files will be installed")
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${INCLUDEDIR}
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "dicts" EXCLUDE
		PATTERN "doc" EXCLUDE
		PATTERN "dtd" EXCLUDE
		PATTERN "editorconfig" EXCLUDE
		PATTERN "icon*" EXCLUDE
		PATTERN "manpages" EXCLUDE
		PATTERN "old" EXCLUDE
		PATTERN ".svn" EXCLUDE
	)
else()
	message(STATUS "No source header files will be installed")
endif()

#Install our stylesheet
install(FILES
	scribus.css
	DESTINATION ${LIBDIR}
)
