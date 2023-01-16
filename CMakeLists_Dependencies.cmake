##############################################################################################################
########## Find Dependencies                                                                        ##########

#<< Qt
set(CMAKE_PREFIX_PATH "${QT_PREFIX}/lib/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MIN_VERSION "6.2.0")

find_package(Qt6 COMPONENTS Core REQUIRED)
add_definitions(${Qt6Core_DEFINITIONS})
include_directories(${Qt6Core_INCLUDE_DIRS})

find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
include_directories(${Qt6Core5Compat_INCLUDE_DIRS})
add_definitions(${Qt6Core5Compat_DEFINITIONS})

find_package(Qt6 COMPONENTS Gui REQUIRED)
include_directories(${Qt6Gui_INCLUDE_DIRS})
add_definitions(${Qt6Gui_DEFINITIONS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
include_directories(${Qt6Widgets_INCLUDE_DIRS})
add_definitions(${Qt6Widgets_DEFINITIONS})

find_package(Qt6 COMPONENTS Network REQUIRED)
include_directories(${Qt6Network_INCLUDE_DIRS})
add_definitions(${Qt6Network_DEFINITIONS})

find_package(Qt6 COMPONENTS OpenGL REQUIRED)
include_directories(${Qt6OpenGL_INCLUDE_DIRS})
add_definitions(${Qt6OpenGL_DEFINITIONS})

find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
include_directories(${Qt6OpenGLWidgets_INCLUDE_DIRS})
add_definitions(${Qt6OpenGLWidgets_DEFINITIONS})

find_package(Qt6 COMPONENTS PrintSupport REQUIRED)
include_directories(${Qt6PrintSupport_INCLUDE_DIRS})
add_definitions(${Qt6PrintSupport_DEFINITIONS})

find_package(Qt6 COMPONENTS Xml REQUIRED)
include_directories(${Qt6Xml_INCLUDE_DIRS})
add_definitions(${Qt6Xml_DEFINITIONS})

find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
include_directories(${Qt6LinguistTools_INCLUDE_DIRS})
add_definitions(${Qt6LinguistTools_DEFINITIONS})

message(STATUS "Qt Version Found: ${Qt6Widgets_VERSION}")


#<<PNG
if (WIN32)
	# On win32 we can use Qt's zlib and libpng, so we use some
	# custom cmake includes. This permits us to honour the
	# USE_QT_ZLIB_PNGLIB flag if passed.
	set(PNG_DIR ${CMAKE_MODULE_PATH})
	set(ZLIB_DIR ${CMAKE_MODULE_PATH})
endif()
#>> PNG
#<< ZLIB
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
	message("ZLIB Library Found OK")
	set(HAVE_LIBZ ON)
endif()
#>> ZLIB

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

#<< JPEG, PNG, TIFF
find_package(JPEG REQUIRED)
if (JPEG_FOUND)
	message("JPEG Library Found OK")
endif()

find_package(PNG 1.6 REQUIRED)
if (PNG_FOUND)
	message("PNG Library Found OK")
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
message("Python 3.x Selected")
find_package (Python3 REQUIRED COMPONENTS Interpreter Development)
if (Python3_Development_FOUND)
	message("Python 3.x Library Found OK (Version ${Python3_VERSION})")
	message("Interpreter: ${Python3_EXECUTABLE}")
	message("Include directories: ${Python3_INCLUDE_DIRS}")
	message("Library directories: ${Python3_LIBRARY_DIRS}")
	message("Runtime library directories: ${Python3_RUNTIME_LIBRARY_DIRS}")
	set(HAVE_PYTHON ON)
endif()
#>> PYTHON

#<< FreeType2
find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
	message(STATUS "FreeType2 Library Found OK")
else()
	message(FATAL_ERROR "No Freetype Found")
endif()
#>> FreeType2

set(cairo_DIR ${CMAKE_MODULE_PATH})
find_package(cairo REQUIRED)
if(cairo_FOUND)
	set(HAVE_CAIRO ON)
	message("cairo Library found OK")
else()
	message(FATAL_ERROR "No cairo library found")
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
	message("LittleCMS-2 Library Found OK")
else()
	message(FATAL_ERROR "Could not find the LittleCMS 2 Library")
endif()
#>>LittleCMS

#<<FontConfig
if(NOT WIN32)
	find_package(Fontconfig REQUIRED)
	if(Fontconfig_FOUND)
		message("FontConfig Found OK")
		set(HAVE_FONTCONFIG ON)
	endif()
endif()
#>>FontConfig

#<<HUNSPELL for Spelling support
find_package(hunspell 1.6 QUIET REQUIRED)
if (hunspell_FOUND)
	message("hunspell Found OK ${hunspell_VERSION}")
	set(HAVE_HUNSPELL ON)
	set(HUNSPELL_VERSION ${hunspell_VERSION})
else()
	message("hunspell or its developer libraries NOT found - Disabling support for spell checking")
endif()
#>>HUNSPELL for Spelling support

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
find_package(poppler REQUIRED)
if (poppler_FOUND)
	set(HAVE_POPPLER ON)
	message(STATUS "Found poppler")
	message(STATUS "Found poppler libs: ${poppler_LIBRARY}")
	message(STATUS "Found poppler includes: ${poppler_INCLUDE_DIR}")
	if (poppler_VERSION VERSION_GREATER_EQUAL 22.01.0)
		message(STATUS "Poppler Version:" ${poppler_VERSION})
		message(STATUS "C++17 is the minimum C++ standard since poppler 22.01.0")
		if (CMAKE_CXX_STANDARD LESS 17)
			message(FATAL_ERROR "Please set -DWANT_CPP17=ON on your CMake command line")
		endif()
	endif()
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
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz>=1.0.5 harfbuzz-icu)
if (HARFBUZZ_FOUND)
	message("Harfbuzz library Found OK")
endif()
pkg_check_modules(ICU REQUIRED icu-uc)

# OpenType subsetting support
pkg_check_modules(HARFBUZZ_SUBSET harfbuzz-subset>=2.4.0)
if (HARFBUZZ_SUBSET_FOUND)
	message("Harfbuzz subset library Found OK")
	set (HAVE_HARFBUZZ_SUBSET ON)
endif()
