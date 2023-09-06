##############################################################################################################
########## Find Dependencies                                                                        ##########

#<<PoDoFo for AI PDF import
option(WITH_PODOFO "Enable support for PDF embedded in AI" ON)
if (WITH_PODOFO)
	find_package(LIBPODOFO 0.9.0)
	if (LIBPODOFO_FOUND)
		message(STATUS "PoDoFo Found OK, Version: " ${LIBPODOFO_VERSION})
		set(HAVE_PODOFO ON)
		if(LIBPODOFO_VERSION VERSION_GREATER_EQUAL "0.10.0")
			if (CMAKE_CXX_STANDARD LESS 17)
				message(STATUS "C++17 is the minimum C++ standard since podofo 0.10.0")
				message(STATUS "Enabling C++17 compiler features")
				set(CMAKE_CXX_STANDARD 17)
			endif()
		endif()
	else()
		message("PoDoFo NOT found - Disabling support for PDF embedded in AI")
	endif()
endif()
#>>PoDoFo for AI PDF import

#<<Poppler for PDF import
find_package(poppler REQUIRED)
if (poppler_FOUND)
	set(HAVE_POPPLER ON)
	message(STATUS "Found poppler")
	message(STATUS "Found poppler libs: ${poppler_LIBRARY}")
	message(STATUS "Found poppler includes: ${poppler_INCLUDE_DIR}")
	if (poppler_VERSION VERSION_GREATER_EQUAL 22.01.0)
		message(STATUS "Poppler Version:" ${poppler_VERSION})
		if (CMAKE_CXX_STANDARD LESS 17)
			message(STATUS "C++17 is the minimum C++ standard since poppler 22.01.0")
			message(STATUS "Enabling C++17 compiler features")
			set(CMAKE_CXX_STANDARD 17)
		endif()
	endif()
else()
	message(FATAL_ERROR "Could not find poppler library")
endif()
#>>Poppler for PDF import

#<< Qt
#Qt less than 6
set(CMAKE_PREFIX_PATH "${QT_PREFIX}/lib/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (NOT WANT_QT6)
	message(STATUS "Qt5 Requested")
	set(QT_MIN_VERSION "5.14.0")
	add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050E00)
	find_package(Qt5Core ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5Core_FOUND)
		message(FATAL_ERROR "ERROR: No QT 5 found")
	endif()
	find_package(Qt5Widgets ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5Widgets_FOUND)
		message(FATAL_ERROR "ERROR: No QT Widgets found")
	endif()
	find_package(Qt5Gui ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5Gui_FOUND)
		message(FATAL_ERROR "ERROR: No Qt5Gui found")
	endif()
	find_package(Qt5Xml ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5Xml_FOUND)
		message(FATAL_ERROR "ERROR: No QT 5 XML found")
	endif()
	find_package(Qt5Network ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5Network_FOUND)
		message(FATAL_ERROR "ERROR: No Qt5Network found")
	endif()
	find_package(Qt5OpenGL ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5OpenGL_FOUND)
		message(FATAL_ERROR "ERROR: No Qt5OpenGL found")
	endif()
	find_package(Qt5LinguistTools ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5LinguistTools_FOUND)
		message(FATAL_ERROR "ERROR: No Qt5LinguistTools found")
	endif()
	#find_package(Qt5Quick ${QT_MIN_VERSION} REQUIRED)
	#if(Qt5Quick_FOUND)
	#    message(STATUS "----- USE Qt5Quick -----")
	#else()
	#    message(FATAL_ERROR "ERROR: No Qt5Quick found")
	#endif()
	find_package(Qt5PrintSupport ${QT_MIN_VERSION} REQUIRED)
	if(NOT Qt5PrintSupport_FOUND)
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
	include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
	add_definitions(${Qt5PrintSupport_DEFINITIONS})

	message(STATUS "Qt Version Found: ${Qt5Widgets_VERSION}")
else()
#WANT QT6
    message(STATUS "Qt6 Requested")
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


	#	include_directories(${Qt5Xml_INCLUDE_DIRS})
#	add_definitions(${Qt5Xml_DEFINITIONS})

#	include_directories(${Qt5OpenGL_INCLUDE_DIRS})
#	add_definitions(${Qt5OpenGL_DEFINITIONS})
#	include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
#	add_definitions(${Qt5PrintSupport_DEFINITIONS})
	message(STATUS "Qt Version Found: ${Qt6Widgets_VERSION}")
endif()




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
if (NOT WANT_PYTHON_2X)
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
else()
	message("Python 2.x Requested")
	find_package (Python2 REQUIRED COMPONENTS Interpreter Development)
	if (Python2_Development_FOUND)
		message("Python 2.x Library Found OK (Version ${Python2_VERSION})")
		message("Interpreter: ${Python2_EXECUTABLE}")
		message("Include directories: ${Python2_INCLUDE_DIRS}")
		message("Library directories: ${Python2_LIBRARY_DIRS}")
		message("Runtime library directories: ${Python2_RUNTIME_LIBRARY_DIRS}")
		set(HAVE_PYTHON ON)
	endif()
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
option(WANT_HUNSPELL "Enable support for spell checking with hunspell" ON)
if (WANT_HUNSPELL)
	find_package(hunspell)
	if (hunspell_FOUND)
		message("hunspell Found OK")
		set(HAVE_HUNSPELL ON)
	else()
		message("hunspell or its developer libraries NOT found - Disabling support for spell checking")
	endif()
endif()
#>>HUNSPELL for Spelling support

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
