##############################################################################################################
########## Find Dependencies                                                                        ##########

#<< Qt
set(QT_MIN_VERSION "5.10.0")
set(CMAKE_PREFIX_PATH "${QT_PREFIX}/lib/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core ${QT_MIN_VERSION} REQUIRED)
if(Qt5Core_FOUND)
	message(STATUS "----- USE QT 5-----")
else()
	message(FATAL_ERROR "ERROR: No QT 5 found")
endif()
find_package(Qt5Widgets ${QT_MIN_VERSION} REQUIRED)
if(Qt5Widgets_FOUND)
	message(STATUS "----- USE QT Widgets-----")
else()
	message(FATAL_ERROR "ERROR: No QT Widgets found")
endif()
find_package(Qt5Gui ${QT_MIN_VERSION} REQUIRED)
if(Qt5Gui_FOUND)
	message(STATUS "----- USE Qt5Gui -----")
else()
	message(FATAL_ERROR "ERROR: Qt5Gui found")
endif()
find_package(Qt5Xml ${QT_MIN_VERSION} REQUIRED)
if(Qt5Xml_FOUND)
	message(STATUS "----- USE QT 5 XML -----")
else()
	message(FATAL_ERROR "ERROR: No QT 5 XML found")
endif()
find_package(Qt5Network ${QT_MIN_VERSION} REQUIRED)
if(Qt5Network_FOUND)
	message(STATUS "----- USE Qt5Network -----")
else()
	message(FATAL_ERROR "ERROR: No Qt5Network found")
endif()
find_package(Qt5OpenGL ${QT_MIN_VERSION} REQUIRED)
if(Qt5OpenGL_FOUND)
	message(STATUS "----- USE Qt5OpenGL -----")
else()
	message(FATAL_ERROR "ERROR: No Qt5OpenGL found")
endif()
find_package(Qt5LinguistTools ${QT_MIN_VERSION} REQUIRED)
if(Qt5LinguistTools_FOUND)
	message(STATUS "----- USE Qt5LinguistTools -----")
else()
	message(FATAL_ERROR "ERROR: No Qt5LinguistTools found")
endif()
#find_package(Qt5Quick ${QT_MIN_VERSION} REQUIRED)
#if(Qt5Quick_FOUND)
#    message(STATUS "----- USE Qt5Quick -----")
#else()
#    message(FATAL_ERROR "ERROR: No Qt5Quick found")
#endif()
find_package(Qt5PrintSupport ${QT_MIN_VERSION} REQUIRED)
if(Qt5PrintSupport_FOUND)
	message(STATUS "----- USE Qt5PrintSupport -----")
else()
	message(FATAL_ERROR "ERROR: No Qt5PrintSupport found")
endif()
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
add_definitions(${Qt5Gui_DEFINITIONS})
include_directories(${Qt5Xml_INCLUDE_DIRS})
add_definitions(${Qt5Xml_DEFINITIONS})
include_directories(${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Network_DEFINITIONS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})
add_definitions(${Qt5OpenGL_DEFINITIONS})
#include_directories(${Qt5Quick_INCLUDE_DIRS})
#add_definitions(${Qt5Quick_DEFINITIONS})
include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
add_definitions(${Qt5PrintSupport_DEFINITIONS})

message(STATUS "Qt VERSION: ${Qt5Widgets_VERSION}")
#if (NOT WANT_CPP11 AND Qt5Widgets_VERSION_MAJOR EQUAL 5 AND Qt5Widgets_VERSION_MINOR GREATER 6)
#	message(STATUS "Your Qt version is 5.7.0 or superior, enabling C++11 compiler features")
#	set(WANT_CPP11 ON)
#endif()
#>> Qt




#<< ZLIB, PNG
if (WIN32)
	# On win32 we can use Qt's zlib and libpng, so we use some
	# custom cmake includes. This permits us to honour the
	# USE_QT_ZLIB_PNGLIB flag if passed.
	set(PNG_DIR ${CMAKE_MODULE_PATH})
	set(ZLIB_DIR ${CMAKE_MODULE_PATH})
endif()
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
	message("ZLIB Library Found OK")
	set(HAVE_LIBZ ON)
endif()
#find_package(PNG REQUIRED)
#if (PNG_FOUND)
#	message("PNG Library Found OK")
#endif()
#>> ZLIB, PNG

if (WANT_NOOSG)
	message("Building without 3D Extension")
else()
	find_package(OSG)
	if(OSG_FOUND)
		message("OSG found OK")
		set(HAVE_OSG ON)
		find_package(OpenGL)
		include(CheckFunctionExists)
		CHECK_FUNCTION_EXISTS(memrchr HAVE_MEMRCHR)
	else()
		message("No OSG found, building without 3D Extension")
	endif()
endif()

#<< JPEG, TIFF
find_package(JPEG REQUIRED)
if (JPEG_FOUND)
	message("JPEG Library Found OK")
endif()
find_package(TIFF REQUIRED)
if (TIFF_FOUND)
	message("TIFF Library Found OK")
	set(HAVE_TIFF ON)
else()
	message(FATAL_ERROR "Could not find the TIFF Library")
endif()
#<< JPEG, TIFF

#<< PYTHON
#
#find_package(PythonInterp 2)
#find_package(PythonLibs 2 REQUIRED)
#if (PYTHON_LIBRARY)
#	message("Python Library Found OK")
#	set(HAVE_PYTHON ON)
#	set(COMPILE_PYTHON ON)
#endif()
#
find_package (Python3 REQUIRED COMPONENTS Interpreter Development)
if (Python3_Development_FOUND)
	message("Python 3.x Library Found OK (Version ${Python3_VERSION})")
	message("Interpreter: ${Python3_EXECUTABLE}")
	message("Include directories: ${Python3_INCLUDE_DIRS}")
	message("Library directories: ${Python3_LIBRARY_DIRS}")
	message("Runtime library directories: ${Python3_RUNTIME_LIBRARY_DIRS}")
	set(HAVE_PYTHON ON)
	set(COMPILE_PYTHON ON)
endif()
#>> PYTHON

#<< FreeType2
find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
	message(STATUS "FreeType2 Library Found OK")
	include(CheckLibraryExists)
	CHECK_LIBRARY_EXISTS(${FREETYPE_LIBRARY} FT_Get_First_Char "" HAVE_FREETYPE_FIRST)
	CHECK_LIBRARY_EXISTS(${FREETYPE_LIBRARY} FT_Get_Next_Char "" HAVE_FREETYPE_NEXT)
	# here we have an alternative
	# a) Setup CFLAGS with the FREETYPE_INCLUDE_DIR_freetype2 provided by cmake
	# b) Insert ${FREETYPE_INCLUDE_DIRS} in include sections of CMakeLists that need it
	# I prefer (b) and at some point & from my opinion it should  be just needed in scribus/fonts/CmakeLists.txt - pm
else()
	message(FATAL_ERROR "No Freetype Found")
endif()
#>> FreeType2

#<<Render Library
if (WANT_PRIVATE_CAIRO)
	message("We no longer support the private cairo library as the latest public release is more up to date, setting WANT_SYSTEM_CAIRO=1")
endif()

if (RENDER_LIB)
	if (RENDER_LIB STREQUAL "QTARTHUR")
		message("Qt4's Arthur as renderer is not supported anymore, performing renderer detection again")
	endif()
endif()

set(CAIRO_DIR ${CMAKE_MODULE_PATH})
find_package(CAIRO REQUIRED)
if(CAIRO_FOUND)
	message("CAIRO Library Found OK")
	set(RENDER_LIB "CAIRO_SYSTEM" CACHE STRING "Set render library to cairo")
else()
	message(FATAL_ERROR "No Cairo Found")
endif()
#>>Render Library

#<<CUPS
if(NOT WIN32 AND NOT HAIKU)
	find_package(Cups REQUIRED)
	if(CUPS_FOUND)
		set(HAVE_CUPS ON)
		message("CUPS Library Found OK")
	endif()
else()
	# Windows doesn't use CUPS
	set(CUPS_INCLUDE_DIR)
endif()
#>>CUPS

#<<LibXML2
find_package(LibXml2 REQUIRED)
if(LIBXML2_FOUND)
	set(HAVE_XML ON)
	message("LIBXML2 Library Found OK")
endif()
#>>LibXML2

#<<LittleCMS
set(LCMS_DIR ${CMAKE_MODULE_PATH})
find_package(LCMS2 REQUIRED)
if(LCMS2_FOUND)
	set(HAVE_LCMS2 ON)
	set(HAVE_CMS ON)
	set(CMS_INC "\"lcms2.h\"")
	message("LittleCMS-2 Library Found OK")
else()
	message(FATAL_ERROR "Could not find the LittleCMS 2 Library")
endif()
#>>LittleCMS

#<<FontConfig
if(NOT WIN32)
	set(FONTCONFIG_DIR ${CMAKE_MODULE_PATH})
	find_package(FONTCONFIG)
	if(FONTCONFIG_FOUND)
		message("FontConfig Found OK")
		set(HAVE_FONTCONFIG ON)
	endif()
else()
	# Windows builds neither use nor require fontconfig
	set(FONTCONFIG_INCLUDE_DIR)
endif()
#>>FontConfig

#<<HUNSPELL for Speelling support
option(WANT_HUNSPELL "Enable support for spell checking with hunspell" ON)
if (WANT_HUNSPELL)
	find_package(HUNSPELL)
	if (HUNSPELL_FOUND)
		message("Hunspell Found OK")
		set(HAVE_HUNSPELL ON)
	else()
		message("Hunspell or its developer libraries NOT found - Disabling support for spell checking")
	endif()
endif()
#>>HUNSPELL for Speelling support

#<<PoDoFo for AI PDF import
option(WITH_PODOFO "Enable support for PDF embedded in AI" ON)
if (WITH_PODOFO)
	find_package(LIBPODOFO)
	if (LIBPODOFO_FOUND)
		message("PoDoFo found OK")
		set(HAVE_PODOFO ON)
	else()
		message("PoDoFo NOT found - Disabling support for PDF embedded in AI")
	endif()
endif()
#>>PoDoFo for AI PDF import

#<<Boost for 2Geom Tools
option(WITH_BOOST "Enable support for Boost based enhancements" ON)
if (WITH_BOOST)
	find_package(Boost)
	if (Boost_FOUND)
		message("Boost Library Found OK")
		set(HAVE_BOOST ON)
	else()
		message("Boost: NOT FOUND, not building 2geomtools")
	endif()
endif()
#>>Boost for 2Geom Tools

#<< GraphicsMagick for image import
if (WANT_GRAPHICSMAGICK)
	set(GMAGICK_DIR ${CMAKE_MODULE_PATH})
	pkg_check_modules(GMAGICK GraphicsMagick)
	# find_package(GMAGICK)
	if(GMAGICK_FOUND)
		message(" GraphicsMagick Found OK")
	else()
		message(" GraphicsMagick NOT found! Building without additional image format support!")
	endif()
else()
	message("Building without  GraphicsMagick (use -DWANT_GRAPHICSMAGICK=1 to enable)")
endif()
#>> GraphicsMagick for image import

#<<Poppler for PDF import
find_package(POPPLER REQUIRED)
if (POPPLER_FOUND)
	set(HAVE_POPPLER ON)
	message(STATUS "Found poppler")
	message(STATUS "Found poppler libs: ${POPPLER_LIBRARY}")
	message(STATUS "Found poppler includes: ${POPPLER_INCLUDE_DIR}")
else()
	message(FATAL_ERROR "Could not find poppler library")
endif()
#>>Poppler for PDF import

# librevenge for MsPub import
pkg_check_modules(LIBREVENGE librevenge-0.0)
if (LIBREVENGE_FOUND)
	pkg_check_modules(LIBREVENGE_STREAM librevenge-stream-0.0)
	if (LIBREVENGE_STREAM_FOUND)
		pkg_check_modules(LIBREVENGE_GENERATORS librevenge-generators-0.0)
		if (LIBREVENGE_GENERATORS_FOUND)
			set(HAVE_REVENGE ON)
		endif()
	endif()
endif()

# CTL support
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz>=0.9.42 harfbuzz-icu)
pkg_check_modules(ICU REQUIRED icu-uc)
