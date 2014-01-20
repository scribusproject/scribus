##############################################################################################################
########## CMake Setup                                                                              ##########

#Set our CMake minimum version
#Require 2.8.9 for Qt5
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9 FATAL_ERROR)

#Pretty colors
SET(CMAKE_COLOR_MAKEFILE ON)
#Dont force verbose
SET(CMAKE_VERBOSE_MAKEFILE OFF)
#Include current dir
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)
#RPATH setup - more below too
IF (WANT_NORPATH OR WANT_DISTROBUILD)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
ELSE (WANT_NORPATH OR WANT_DISTROBUILD)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
ENDIF (WANT_NORPATH OR WANT_DISTROBUILD)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_SKIP_RULE_DEPENDENCY TRUE)
SET(CMAKE_SKIP_BUILD_RPATH TRUE)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CheckTypeSize)
INCLUDE(TestBigEndian)

#ENABLE_TESTING()

#Set the custom CMake module directory where our include/lib finders are
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

##############################################################################################################
########## toplevel compiler flags                                                                  ##########
MESSAGE(STATUS "Shared Library Flags: ${CMAKE_SHARED_LIBRARY_C_FLAGS}")

#Project Setup
PROJECT(scribus)
#Set our version values
#Final version is ${VERSION} = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_SUFFIX}
#where VERSION_SUFFIX is of the form "", "svn" or "Nsvn" (N being the minor patch level)
SET (VERSION_MAJOR "1")
SET (VERSION_MINOR "5")
SET (VERSION_PATCH "0")
SET (VERSION_SUFFIX "svn")
SET (VERSION ${VERSION_MAJOR})
IF (VERSION_MINOR GREATER -1)
  SET (VERSION ${VERSION}.${VERSION_MINOR})
ENDIF (VERSION_MINOR GREATER -1)
IF (VERSION_PATCH GREATER -1)
  SET (VERSION ${VERSION}.${VERSION_PATCH})
ENDIF (VERSION_PATCH GREATER -1)
IF (VERSION_SUFFIX)
  SET (VERSION ${VERSION}.${VERSION_SUFFIX})
ENDIF (VERSION_SUFFIX)
MESSAGE(STATUS "Scribus ${VERSION} will be built to install into ${CMAKE_INSTALL_PREFIX}")

#Set the permissions to be used when installing plugins
SET(PLUGIN_PERMISSIONS WORLD_EXECUTE GROUP_EXECUTE OWNER_EXECUTE WORLD_READ GROUP_READ OWNER_READ OWNER_WRITE)

#Our main directory is scribus
SET(MAIN_DIR_NAME "scribus")

##############################################################################################################
########## check for the CPU we build for                                                           ##########

EXECUTE_PROCESS(
    COMMAND ${CMAKE_C_COMPILER} -dumpmachine
    OUTPUT_VARIABLE MACHINE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
MESSAGE(STATUS  "Machine: ${MACHINE}, void pointer size: ${CMAKE_SIZEOF_VOID_P}")
STRING(REGEX MATCH "(i?86-*)|(athlon-*)|(pentium-*)" _machine_x86 "${MACHINE}")
IF (_machine_x86)
    MESSAGE(STATUS "Found target X86")
    SET(ARCH_X86 1)
ENDIF (_machine_x86)
IF (APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND ARCH_X86 EQUAL 1)
  MESSAGE(STATUS "Found Snow Leopard Target: Apple, 64 bit, X86")
  UNSET(ARCH_X86)
  SET(ARCH_X86_64 1)
  SET(APPLE_10_6_X 1 CACHE TYPE BOOL)
ENDIF (APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND ARCH_X86 EQUAL 1)

STRING(REGEX MATCH "(x86_64-*)|(X86_64-*)|(AMD64-*)|(amd64-*)" _machine_x86_64 "${MACHINE}")
IF (_machine_x86_64)
    MESSAGE(STATUS "Found target X86_64")
    SET(ARCH_X86_64 1)
ENDIF (_machine_x86_64)

STRING(REGEX MATCH "(sparc64-*)|(SPARC64-*)" _machine_sparc_64 "${MACHINE}")
IF (_machine_sparc_64)
    MESSAGE(STATUS "Found target SPARC 64")
    SET(ARCH_SPARC_64 1)
ENDIF (_machine_sparc_64)

STRING(REGEX MATCH "(mips64-*)|(MIPS64-*)" _machine_mips_64 "${MACHINE}")
IF (_machine_mips_64)
    MESSAGE(STATUS "Found target MIPS 64")
    SET(ARCH_MIPS_64 1)
ENDIF (_machine_mips_64)

STRING(REGEX MATCH "(ppc-*)|(powerpc-*)" _machine_ppc "${MACHINE}")
IF (_machine_ppc)
    MESSAGE(STATUS "Found target PPC")
    SET(ARCH_PPC 1)
ENDIF (_machine_ppc)

STRING(REGEX MATCH "(ppc64-*)|(PPC64-*)|(powerpc64-*)" _machine_ppc_64 "${MACHINE}")
IF (_machine_ppc_64)
    MESSAGE(STATUS "Found target PPC64")
    SET(ARCH_PPC_64 1)
ENDIF (_machine_ppc_64)

STRING(REGEX MATCH "(sparc-*)" _machine_sparc "${MACHINE}")
IF (_machine_sparc)
    MESSAGE(STATUS "Found target Sparc")
    SET(ARCH_SPARC 1)
ENDIF (_machine_sparc)

STRING(REGEX MATCH "(sparcv9-*)" _machine_sparcv9 "${MACHINE}")
IF (_machine_sparcv9)
    MESSAGE(STATUS "Found target Sparc v9")
    SET(ARCH_SPARCV9 1)
ENDIF (_machine_sparcv9)

STRING(REGEX MATCH "(sparc64-*)" _machine_sparc64 "${MACHINE}")
IF (_machine_sparc64)
    MESSAGE(STATUS "Found target Sparc64")
    SET(ARCH_SPARC64 1)
    SET(ARCH64BIT 1)
ENDIF (_machine_sparc64)

#convert any 64 bit build into generic 64 tag for below
IF (ARCH_X86_64 OR ARCH_SPARC_64 OR ARCH_MIPS_64 OR ARCH_PPC_64) 
  SET(ARCH64BIT 1)
ENDIF (ARCH_X86_64 OR ARCH_SPARC_64 OR ARCH_MIPS_64 OR ARCH_PPC_64) 

MESSAGE(STATUS "Building for target ${MACHINE}")

##############################################################################################################
########## Versioning Setup                                                                         ##########

#On Apple, we ignore the versioning tag so all items are "scribus" not "scribus-version"
IF (NOT BUILD_OSX_BUNDLE)
  SET(BUILD_OSX_BUNDLE ${APPLE} CACHE TYPE BOOL)
ENDIF (NOT BUILD_OSX_BUNDLE)

#Simplify future conditionals for Apple
IF (APPLE AND BUILD_OSX_BUNDLE)
  SET(APPLEBUNDLE TRUE CACHE TYPE BOOL)
ENDIF (APPLE AND BUILD_OSX_BUNDLE)

#Announce we cached a version request before, overridden below for OSX
IF(TAG_VERSION)
  IF (NOT APPLEBUNDLE)
    MESSAGE(STATUS "Previously selected version tag: ${TAG_VERSION}")
  ENDIF (NOT APPLEBUNDLE)
  SET(WANT_VERSIONING 1)
  SET(CACHED_VERSIONING 1)
ENDIF(TAG_VERSION)

#Remove version tags on OSX so our bundle is Scribus.app
IF (APPLEBUNDLE)
  IF (WANT_VERSIONING OR CACHED_VERSIONING)
    MESSAGE(STATUS "Ignoring version tag on OSX when building bundle")
    SET(WANT_VERSIONING 0)
    SET(CACHED_VERSIONING 0)
  ENDIF (WANT_VERSIONING OR CACHED_VERSIONING)
ENDIF (APPLEBUNDLE)

#Store our version string if require
IF (WANT_VERSIONING AND NOT CACHED_VERSIONING)
  IF (NOT CUSTOM_VERSIONTAG)
    SET(TAG_VERSION "-${VERSION}" CACHE TYPE STRING)
  ELSE (NOT CUSTOM_VERSIONTAG)
    SET(TAG_VERSION ${CUSTOM_VERSIONTAG} CACHE TYPE STRING)
  ENDIF (NOT CUSTOM_VERSIONTAG)
ENDIF (WANT_VERSIONING AND NOT CACHED_VERSIONING)

##############################################################################################################
########## Install/Directory Setup                                                                  ##########

IF (APPLEBUNDLE)
  IF ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/Contents/?$")
    # all ok
  ELSE ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/Contents/?$")
    IF ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/?$")
      # add "/Contents" subpath
      SET(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/Contents")
    ELSE ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/?$")
      # no bundle name spcified, use appname and version
      SET(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/Scribus${VERSION}.app/Contents")
    ENDIF ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/?$")
  ENDIF ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/Contents/?$")

  MESSAGE(STATUS "APPLEBUNDLE install prefix: ${CMAKE_INSTALL_PREFIX}")
ENDIF (APPLEBUNDLE)


#Setp all the directories we will use
#MAN
CMAKE_POLICY(SET CMP0005 OLD)
SET(MANDIR "share/man/")
IF(WANT_VERSIONING)
    SET(SHAREDIR "share/${MAIN_DIR_NAME}${TAG_VERSION}/")
ELSE(WANT_VERSIONING)
    SET(SHAREDIR "share/${MAIN_DIR_NAME}/")
ENDIF(WANT_VERSIONING)
ADD_DEFINITIONS("-DSHAREDIR=\\\"${CMAKE_INSTALL_PREFIX}/${SHAREDIR}\\\"")

#SHARE - use the default on Apple as TAG_VERSION is empty
IF(WIN32 OR OS2)
  SET(DOCDIR "share/doc/")
ELSE (WIN32 OR OS2)
  IF(TAG_VERSION OR BUILD_OSX_BUNDLE)
    SET(DOCDIR "share/doc/${MAIN_DIR_NAME}${TAG_VERSION}/")
  ELSE(TAG_VERSION OR BUILD_OSX_BUNDLE)
        IF(NOT WANT_VERSIONING)
            SET(DOCDIR "share/doc/${MAIN_DIR_NAME}/")
        ELSE(NOT WANT_VERSIONING)
            SET(DOCDIR "share/doc/${MAIN_DIR_NAME}-${VERSION}/")
        ENDIF(NOT WANT_VERSIONING)
  ENDIF (TAG_VERSION OR BUILD_OSX_BUNDLE)
ENDIF (WIN32 OR OS2)
ADD_DEFINITIONS("-DDOCDIR=\\\"${CMAKE_INSTALL_PREFIX}/${DOCDIR}\\\"")

#ICONS
IF(WIN32)
  SET(ICONDIR "share/icons/")
ELSE(WIN32)
  SET(ICONDIR "share/${MAIN_DIR_NAME}${TAG_VERSION}/icons/")
ENDIF(WIN32)
ADD_DEFINITIONS("-DICONDIR=\\\"${CMAKE_INSTALL_PREFIX}/${ICONDIR}\\\"")

#SAMPLES
SET(SAMPLESDIR "share/${MAIN_DIR_NAME}${TAG_VERSION}/samples/")
ADD_DEFINITIONS("-DSAMPLESDIR=\\\"${CMAKE_INSTALL_PREFIX}/${SAMPLESDIR}\\\"")

#SCRIPTS
SET(SCRIPTSDIR "share/${MAIN_DIR_NAME}${TAG_VERSION}/scripts/")
ADD_DEFINITIONS("-DSCRIPTSDIR=\\\"${CMAKE_INSTALL_PREFIX}/${SCRIPTSDIR}\\\"")

#TEMPLATES
IF(WIN32)
  SET(TEMPLATEDIR "share/templates/")
ELSE(WIN32)
  SET(TEMPLATEDIR "share/${MAIN_DIR_NAME}${TAG_VERSION}/templates/")
ENDIF(WIN32)
ADD_DEFINITIONS("-DTEMPLATEDIR=\\\"${CMAKE_INSTALL_PREFIX}/${TEMPLATEDIR}\\\"")

#QML
IF(WIN32)
  SET(QMLDIR "share/qml/")
ELSE(WIN32)
  SET(QMLDIR "share/${MAIN_DIR_NAME}${TAG_VERSION}/qml/")
ENDIF(WIN32)
ADD_DEFINITIONS("-DQMLDIR=\\\"${CMAKE_INSTALL_PREFIX}/${QMLDIR}\\\"")

#DESKTOP
SET(DESKTOPDIR "share/applications/")
ADD_DEFINITIONS("-DDESKTOPDIR=\\\"${CMAKE_INSTALL_PREFIX}/${DESKTOPDIR}\\\"")

# APPLICATION_DATA_DIR
# It allows specify the user configuration directory in the compile time.
# Example: I want to use special ~/.scribus135svn config directory for my trunk
#          cmake -DAPPLICATION_DATA_DIR=.scribus135svn ...
IF (APPLICATION_DATA_DIR)
	MESSAGE(STATUS "Using ApplicationDataDir ${APPLICATION_DATA_DIR} instead of the platform default.")
    ADD_DEFINITIONS("-DAPPLICATION_DATA_DIR=\\\"${APPLICATION_DATA_DIR}\\\"")
ELSE (APPLICATION_DATA_DIR)
    MESSAGE(STATUS "Using standard ApplicationDataDir. You can change it with -DAPPLICATION_DATA_DIR")
ENDIF (APPLICATION_DATA_DIR)

#LIB LOCATION - lib or lib64 normally
#LIB
IF(WANT_LIB64)
  IF(NOT ARCH_X86_64)
    MESSAGE(STATUS "WARNING: BUILDING 64 Bit build on non 64 bit architecture")
  ENDIF(NOT ARCH_X86_64)
  SET(LIB_SUFFIX "64" CACHE TYPE STRING)
ENDIF(WANT_LIB64)
SET(LIB_DIR_NAME "lib${LIB_SUFFIX}")
SET(LIBDIR "${LIB_DIR_NAME}/${MAIN_DIR_NAME}${TAG_VERSION}/")
ADD_DEFINITIONS("-DLIBDIR=\\\"${CMAKE_INSTALL_PREFIX}/${LIBDIR}\\\"")

#C++11 Support
IF(WANT_CPP11)
	MESSAGE(STATUS "Enabling C++11 compiler features")
	ADD_DEFINITIONS("-std=c++11")
ENDIF(WANT_CPP11)

#PLUGINS
IF(WIN32)
  SET(PLUGINDIR "plugins")
ELSE(WIN32)
  SET(PLUGINDIR "${LIB_DIR_NAME}/${MAIN_DIR_NAME}${TAG_VERSION}/plugins/")
ENDIF(WIN32)
ADD_DEFINITIONS("-DPLUGINDIR=\\\"${CMAKE_INSTALL_PREFIX}/${PLUGINDIR}\\\"")


#INCLUDE
SET(INCLUDEDIR "include/${MAIN_DIR_NAME}${TAG_VERSION}/")
#NO ADD DEFINITIONS, WE ONLY INSTALL WITH IT

#RPATH
IF (WANT_NORPATH OR WANT_DISTROBUILD)
  set(CMAKE_INSTALL_RPATH "")
ELSE (WANT_NORPATH OR WANT_DISTROBUILD)
  set(CMAKE_INSTALL_RPATH ${PLUGINDIR})
ENDIF (WANT_NORPATH OR WANT_DISTROBUILD)

# Set the XDG mime stuff
SET(MIMEDIR "share/mime/packages/")
SET(MIMELNKDIR "share/mimelnk/application/")

##############################################################################################################
########## Build Setup                                                                              ##########

#Convert our simpler command line option to the CMake style
#None, Debug, Release, .. or custom ones
# SET(WANT_DEBUG 1)
IF(WANT_DEBUG)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF(WANT_DEBUG)
IF (WANT_RELEASEWITHDEBUG)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF (WANT_RELEASEWITHDEBUG)
IF(NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)
  SET(CMAKE_BUILD_TYPE RELEASE)
ENDIF(NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)



#Based on our build type, setup our build options
IF(APPLE)
  IF(APPLE_10_6_X)
    SET(OSXMINVER "10.6" CACHE TYPE STRING)
  ELSE(APPLE_10_6_X)
    SET(OSXMINVER "10.4" CACHE TYPE STRING)
  ENDIF(APPLE_10_6_X)
  # gcc/mac
  IF(WANT_UNIVERSAL_BUNDLE)
    MESSAGE("Setting Universal (PPC/i386) Architecture for OSX Build/Bundle")
    SET(CMAKE_OSX_ARCHITECTURES "ppc;i386" )
    SET(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "ppc;i386" )
    SET(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.4u.sdk")
	SET(CMAKE_OSX_DEPLOYMENT_TARGET "10.4")
  ELSE(WANT_UNIVERSAL_BUNDLE)
    IF(ARCH_PPC OR ARCH_PPC_64)
      MESSAGE("Setting PPC Architecture for OSX Build/Bundle")
      SET(CMAKE_OSX_ARCHITECTURES "ppc" )
      SET(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "ppc" )
	ELSE(ARCH_PPC OR ARCH_PPC_64)
	  IF(APPLE_10_6_X)
		MESSAGE("Setting x86_64 Architecture for OSX Build/Bundle")
		SET(CMAKE_OSX_ARCHITECTURES "x86_64" )
		SET(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "x86_64" )
	  ELSE(APPLE_10_6_X)
		MESSAGE("Setting i386 Architecture for OSX Build/Bundle")
		SET(CMAKE_OSX_ARCHITECTURES "i386" )
		SET(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "i386" )
	  ENDIF(APPLE_10_6_X)
    ENDIF(ARCH_PPC OR ARCH_PPC_64)
  ENDIF(WANT_UNIVERSAL_BUNDLE)
  MESSAGE(STATUS "${CMAKE_OSX_ARCHITECTURES} ${CMAKE_OSX_SYSROOT}")
  SET(ENV{MACOSX_DEPLOYMENT_TARGET} ${OSXMINVER} )
  SET( ENV{LD_PREBIND} 1 )
  SET( ENV{LD_PREBIND_ALLOW_OVERLAP} 1 )
  IF (WANT_DEBUG)
	SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
	SET(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
  ENDIF (WANT_DEBUG)
  IF (WANT_RELEASEWITHDEBUG)
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g3 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
	SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g3 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
  ENDIF (WANT_RELEASEWITHDEBUG)
  IF (NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)
	SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
	SET(CMAKE_C_FLAGS_RELEASE "-O2 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
  ENDIF (NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)
ELSE(APPLE)
  IF(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
    # Windows build with Visual Studio
    # Surely there's a better way to identify the compiler?
    SET(CMAKE_CXX_FLAGS_DEBUG)
    SET(CMAKE_C_FLAGS_DEBUG)
  ELSE(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
    # vanilla gcc
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -fstrict-aliasing")
    SET(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -Wall -fstrict-aliasing")
    
    IF (_machine_x86 OR _machine_x86_64)
      SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
      SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fPIC")
    ENDIF (_machine_x86 OR _machine_x86_64)
    
  ENDIF(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
ENDIF(APPLE)

IF(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
  SET(CMAKE_CXX_FLAGS_RELEASE)
  SET(CMAKE_C_FLAGS_RELEASE)
ELSE(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
  SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -fPIC")
  SET(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -fPIC")
  ADD_DEFINITIONS(-DNDEBUG)
ENDIF(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN
                  -DAVOID_WIN32_FILEIO
                  -D_CRT_SECURE_NO_DEPRECATE
                  -D_USE_MATH_DEFINES
                  -DCOMPILE_PLUGIN_AS_DLL
  )
  SET(BUILD_WINDOWS_VERSION 1)
  SET(DLL_USE_NATIVE_API 1)
ENDIF(WIN32)

##############################################################################################################
########## Find Dependencies                                                                        ##########

#<< Qt
	SET (QT_MIN_VERSION "5.0.0")
	SET(CMAKE_PREFIX_PATH "${QT_PREFIX}/lib/cmake")
	SET(CMAKE_INCLUDE_CURRENT_DIR ON)
	FIND_PACKAGE(Qt5Core REQUIRED)
	IF(Qt5Core_FOUND)
		MESSAGE(STATUS "----- USE QT 5-----")
	ELSE(Qt5Core_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No QT 5 found")
	ENDIF(Qt5Core_FOUND)
	FIND_PACKAGE(Qt5Widgets REQUIRED)
	IF(Qt5Widgets_FOUND)
		MESSAGE(STATUS "----- USE QT Widgets-----")
	ELSE(Qt5Widgets_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No QT Widgets found")
	ENDIF(Qt5Widgets_FOUND)
	FIND_PACKAGE(Qt5Gui REQUIRED)
	IF(Qt5Gui_FOUND)
		MESSAGE(STATUS "----- USE Qt5Gui -----")
	ELSE(Qt5Gui_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: Qt5Gui found")
	ENDIF(Qt5Gui_FOUND)
	FIND_PACKAGE(Qt5Xml REQUIRED)
	IF(Qt5Xml_FOUND)
		MESSAGE(STATUS "----- USE QT 5 XML -----")
	ELSE(Qt5Xml_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No QT 5 XML found")
	ENDIF(Qt5Xml_FOUND)
	FIND_PACKAGE(Qt5WebKit REQUIRED)
	IF(Qt5WebKit_FOUND)
		MESSAGE(STATUS "----- USE Qt5WebKit -----")
	ELSE(Qt5WebKit_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No Qt5WebKit found")
	ENDIF(Qt5WebKit_FOUND)
	FIND_PACKAGE(Qt5WebKitWidgets REQUIRED)
	IF(Qt5WebKitWidgets_FOUND)
		MESSAGE(STATUS "----- USE Qt5WebKitWidgets -----")
	ELSE(Qt5WebKitWidgets_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No Qt5WebKitWidgets found")
	ENDIF(Qt5WebKitWidgets_FOUND)
	FIND_PACKAGE(Qt5Network REQUIRED)
	IF(Qt5Network_FOUND)
		MESSAGE(STATUS "----- USE Qt5Network -----")
	ELSE(Qt5Network_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No Qt5Network found")
	ENDIF(Qt5Network_FOUND)
	FIND_PACKAGE(Qt5OpenGL REQUIRED)
	IF(Qt5OpenGL_FOUND)
		MESSAGE(STATUS "----- USE Qt5OpenGL -----")
	ELSE(Qt5OpenGL_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No Qt5OpenGL found")
	ENDIF(Qt5OpenGL_FOUND)
	FIND_PACKAGE(Qt5LinguistTools REQUIRED)
	IF(Qt5LinguistTools_FOUND)
		MESSAGE(STATUS "----- USE Qt5LinguistTools -----")
	ELSE(Qt5LinguistTools_FOUND)
		MESSAGE(FATAL_ERROR "ERROR: No Qt5LinguistTools found")
	ENDIF(Qt5LinguistTools_FOUND)
	INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5Core_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5Gui_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5Xml_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5Xml_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5WebKit_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5WebKit_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5WebKitWidgets_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5WebKitWidgets_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5Network_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5Network_DEFINITIONS})
	INCLUDE_DIRECTORIES(${Qt5OpenGL_INCLUDE_DIRS})
	ADD_DEFINITIONS(${Qt5OpenGL_DEFINITIONS})
#>> Qt

#<< ZLIB, PNG
IF (WIN32)
  # On win32 we can use Qt's zlib and libpng, so we use some
  # custom cmake includes. This permits us to honour the 
  # USE_QT_ZLIB_PNGLIB flag if passed.
  SET(PNG_DIR ${CMAKE_MODULE_PATH})
  SET(ZLIB_DIR ${CMAKE_MODULE_PATH})
ENDIF (WIN32)
FIND_PACKAGE(ZLIB REQUIRED)
IF (ZLIB_FOUND)
  MESSAGE("ZLIB Library Found OK")
  SET(HAVE_LIBZ 1)
ENDIF(ZLIB_FOUND)
FIND_PACKAGE(PNG REQUIRED)
IF (PNG_FOUND)
  MESSAGE("PNG Library Found OK")
ENDIF(PNG_FOUND)
#>> ZLIB, PNG

IF (WANT_NOOSG)
  MESSAGE("Building without 3D Extension")
ELSE (WANT_NOOSG)
  FIND_PACKAGE(OSG)
  IF(OSG_FOUND)
    MESSAGE("OSG found OK")
    SET(HAVE_OSG 1)
    FIND_PACKAGE(OpenGL)
  ELSE(OSG_FOUND)
	MESSAGE("No OSG found, building without 3D Extension")
  ENDIF(OSG_FOUND)
ENDIF (WANT_NOOSG)

#<< JPEG, TIFF
FIND_PACKAGE(JPEG REQUIRED)
IF (JPEG_FOUND)
  MESSAGE("JPEG Library Found OK")
ENDIF(JPEG_FOUND)
FIND_PACKAGE(TIFF REQUIRED)
IF (TIFF_FOUND)
  MESSAGE("TIFF Library Found OK")
  SET(HAVE_TIFF 1)
ELSE(TIFF_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the TIFF Library")
ENDIF(TIFF_FOUND)
#<< JPEG, TIFF

#<< PYTHON
SET(PythonLibs_FIND_VERSION 2)
FIND_PACKAGE(PythonLibs REQUIRED)
IF (PYTHON_LIBRARY)
  MESSAGE("Python Library Found OK")
  SET(HAVE_PYTHON 1)
  SET(COMPILE_PYTHON 1)
ENDIF(PYTHON_LIBRARY)
#>> PYTHON

#<< FreeType2
  FIND_PACKAGE(Freetype REQUIRED)
  IF (FREETYPE_FOUND)
    MESSAGE(STATUS "FreeType2 Library Found OK")
    INCLUDE(CheckLibraryExists)
    CHECK_LIBRARY_EXISTS(${FREETYPE_LIBRARY} FT_Get_First_Char "" HAVE_FREETYPE_FIRST)
    CHECK_LIBRARY_EXISTS(${FREETYPE_LIBRARY} FT_Get_Next_Char "" HAVE_FREETYPE_NEXT)
    # here we have an alternative
    # a) Setup CFLAGS with the FREETYPE_INCLUDE_DIR_freetype2 provided by cmake
    # b) Insert ${FREETYPE_INCLUDE_DIRS} in include sections of CMakeLists that need it
    # I prefer (b) and at some point & from my opinion it should  be just needed in scribus/fonts/CmakeLists.txt - pm
  ELSE (FREETYPE_FOUND)
    MESSAGE(FATAL_ERROR "No Freetype Found")
  ENDIF(FREETYPE_FOUND)
#>> FreeType2

#<<Render Library
IF (WANT_PRIVATE_CAIRO)
  MESSAGE("We no longer support the private cairo library as the latest public release is more up to date, setting WANT_SYSTEM_CAIRO=1")
ENDIF(WANT_PRIVATE_CAIRO)

IF (RENDER_LIB)
  IF (RENDER_LIB STREQUAL "QTARTHUR")
    MESSAGE("Qt4's Arthur as renderer is not supported anymore, performing renderer detection again")
  ENDIF (RENDER_LIB STREQUAL "QTARTHUR")
ENDIF(RENDER_LIB)

SET(CAIRO_DIR ${CMAKE_MODULE_PATH})
FIND_PACKAGE(CAIRO REQUIRED)
IF(CAIRO_FOUND)
  MESSAGE("CAIRO Library Found OK")
  SET(RENDER_LIB "CAIRO_SYSTEM" CACHE TYPE STRING)
ELSE(CAIRO_FOUND)
  MESSAGE(FATAL_ERROR "No Cairo Found")
ENDIF(CAIRO_FOUND)
#>>Render Library

#<<CUPS
IF(NOT WIN32 AND NOT HAIKU)
  FIND_PACKAGE(Cups REQUIRED)
  IF(CUPS_FOUND)
    SET(HAVE_CUPS 1)
    MESSAGE("CUPS Library Found OK")
  ENDIF(CUPS_FOUND)
ELSE(NOT WIN32 AND NOT HAIKU)
  # Windows doesn't use CUPS
  SET(CUPS_INCLUDE_DIR)
ENDIF(NOT WIN32 AND NOT HAIKU)
#>>CUPS

#<<LibXML2
FIND_PACKAGE(LibXml2 REQUIRED)
IF(LIBXML2_FOUND)
  SET(HAVE_XML 1)
  MESSAGE("LIBXML2 Library Found OK")
ENDIF(LIBXML2_FOUND)
#>>LibXML2

#<<LittleCMS
SET(LCMS_DIR ${CMAKE_MODULE_PATH})
FIND_PACKAGE(LCMS2 REQUIRED)
IF(LCMS2_FOUND)
  SET(HAVE_LCMS2 1)
  SET(HAVE_CMS 1)
  SET(CMS_INC "\"lcms2.h\"")
  MESSAGE("LittleCMS-2 Library Found OK")
ELSE(LCMS2_FOUND)
   MESSAGE(FATAL_ERROR "Could not find the LittleCMS 2 Library")
ENDIF(LCMS2_FOUND)
#>>LittleCMS

#<<FontConfig
IF(NOT WIN32)
  SET(FONTCONFIG_DIR ${CMAKE_MODULE_PATH})
  FIND_PACKAGE(FONTCONFIG)
  IF(FONTCONFIG_FOUND)
	MESSAGE("FontConfig Found OK")
	SET(HAVE_FONTCONFIG 1)
  ENDIF(FONTCONFIG_FOUND)
ELSE(NOT WIN32)
  # Windows builds neither use nor require fontconfig
  SET(FONTCONFIG_INCLUDE_DIR)
ENDIF(NOT WIN32)
#>>FontConfig

#<<HUNSPELL for Speelling support
OPTION(WANT_HUNSPELL "Enable support for spell checking with hunspell" ON)
IF (WANT_HUNSPELL)
  FIND_PACKAGE(HUNSPELL)
  IF (HUNSPELL_FOUND)
	MESSAGE("Hunspell Found OK")
	SET(HAVE_HUNSPELL 1)
  ELSE (HUNSPELL_FOUND)
	MESSAGE("Hunspell or its developer libraries NOT found - Disabling support for spell checking")
  ENDIF (HUNSPELL_FOUND)
ENDIF (WANT_HUNSPELL)
#>>HUNSPELL for Speelling support

#<<PoDoFo for AI PDF import
OPTION(WITH_PODOFO "Enable support for PDF embedded in AI" ON)
IF (WITH_PODOFO)
  FIND_PACKAGE(LIBPODOFO)
  IF (LIBPODOFO_FOUND)
	MESSAGE("PoDoFo found OK")
	SET(HAVE_PODOFO 1)
  ELSE (LIBPODOFO_FOUND)
	MESSAGE("PoDoFo NOT found - Disabling support for PDF embedded in AI")
  ENDIF (LIBPODOFO_FOUND)
ENDIF (WITH_PODOFO)
#>>PoDoFo for AI PDF import

#<<Boost for 2Geom Tools
OPTION(WITH_BOOST "Enable support for Boost based enhancements" ON)
IF (WITH_BOOST)
  FIND_PACKAGE(Boost)
  IF (Boost_FOUND)
	MESSAGE("Boost Library Found OK")
	SET(HAVE_BOOST 1)
  ELSE(Boost_FOUND)
	MESSAGE("Boost: NOT FOUND, not building 2geomtools")
  ENDIF (Boost_FOUND)
ENDIF (WITH_BOOST)
#>>Boost for 2Geom Tools

#<<GraphicksMagick for image import
IF (WANT_GRAPHICSMAGICK)
  SET(GMAGICK_DIR ${CMAKE_MODULE_PATH})
  FIND_PACKAGE(GMAGICK)
  IF(GMAGICK_FOUND)
	MESSAGE("GraphicksMagick Found OK")
  ELSE(GMAGICK_FOUND)
	MESSAGE("GraphicksMagick NOT found! Building without additional image format support!")
  ENDIF(GMAGICK_FOUND)
ELSE (WANT_GRAPHICSMAGICK)
  MESSAGE("Building without GraphicksMagick (use -DWANT_GRAPHICSMAGICK=1 to enable)")
ENDIF (WANT_GRAPHICSMAGICK)
#>>GraphicksMagick for image import

#<<Poppler for PDF import
FIND_PACKAGE(POPPLER)
IF (POPPLER_FOUND)
	IF (FREETYPE_FOUND)
		SET( HAVE_POPPLER 1 )
		MESSAGE(STATUS "Found poppler")
		MESSAGE(STATUS "Found poppler libs: ${POPPLER_LIBRARY}")
		MESSAGE(STATUS "Found poppler includes: ${POPPLER_INCLUDE_DIR}")
	ELSE (FREETYPE_FOUND)
		MESSAGE("Poppler NOT found - Disabling support for native PDF import")
	ENDIF (FREETYPE_FOUND)
ELSE (POPPLER_FOUND)
	MESSAGE("Poppler NOT found - Disabling support for native PDF import")
ENDIF (POPPLER_FOUND)
#>>Poppler for PDF import


##############################################################################################################
########## Include Setup                                                                            ##########

TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
  
CHECK_INCLUDE_FILE("dlfcn.h" HAVE_DLFCN_H)
IF(HAVE_DLFCN_H)
  ADD_DEFINITIONS(-DHAVE_DLFCN_H)
ENDIF(HAVE_DLFCN_H)

CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
IF(HAVE_UNISTD_H)
  ADD_DEFINITIONS(-DHAVE_UNISTD_H)
ENDIF(HAVE_UNISTD_H)

CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
# IF(HAVE_SYS_TYPES_H)
#   ADD_DEFINITIONS(-DHAVE_SYS_TYPES_H)
# ENDIF(HAVE_SYS_TYPES_H)

CHECK_INCLUDE_FILE("sys/stat.h" HAVE_SYS_STAT_H)
# IF(HAVE_SYS_STAT_H)
#   ADD_DEFINITIONS(-DHAVE_SYS_STAT_H)
# ENDIF(HAVE_SYS_STAT_H)
#>>Test for existing include files


#Create configure files.. config.h and uninstall
#config.h
INCLUDE (ConfigureChecks.cmake)
  IF(WIN32)
    SET(CONFIG_NAME win-config.h)
  ELSE(WIN32)
    SET(CONFIG_NAME config.h)
  ENDIF(WIN32)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_NAME})
ADD_DEFINITIONS("-DHAVE_CONFIG_H")



#Set up include dirs with all found packages
INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/scribus
  ${QT_INCLUDES}
  ${FREETYPE_INCLUDE_DIR}
  ${FONTCONFIG_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${LCMS2_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${TIFF_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${OSG_INCLUDE_DIR}
  ${GSL_INCLUDE_DIR}
)

IF (NOT WIN32 AND NOT HAIKU)
INCLUDE_DIRECTORIES(
  ${CUPS_INCLUDE_DIR}
)
ENDIF (NOT WIN32 AND NOT HAIKU)

##############################################################################################################
########## Uninstall Setup                                                                          ##########

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY
)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
  
  
##############################################################################################################
########## Add our subdirs                                                                          ##########

#Add our source subdirs
ADD_SUBDIRECTORY(scribus)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(resources/dicts)
ADD_SUBDIRECTORY(resources/editorconfig)
ADD_SUBDIRECTORY(resources/icons)
ADD_SUBDIRECTORY(resources/keysets)
ADD_SUBDIRECTORY(resources/loremipsum)
ADD_SUBDIRECTORY(resources/profiles)
ADD_SUBDIRECTORY(resources/swatches)
ADD_SUBDIRECTORY(resources/templates)
ADD_SUBDIRECTORY(resources/translations)

#Install our READMEs etc.
INSTALL(FILES
  AUTHORS
  BUILDING
  ChangeLog
  ChangeLogSVN
  COPYING
  LINKS
  NEWS
  PACKAGING
  README
  TODO
  TRANSLATION
  DESTINATION ${DOCDIR}
)

IF (APPLE)
  INSTALL(FILES
    README.MacOSX
    DESTINATION ${DOCDIR}
  )

#	EXECUTE_PROCESS (COMMAND mkdir -p "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats")
#	IF (NOT EXISTS "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats/libqjpeg.dylib")
#MESSAGE(STATUS ${QT_PREFIX})
#MESSAGE(STATUS "@QT_PREFIX@/plugins/imageformats/libqjpeg.dylib")
#MESSAGE(STATUS "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats/libqjpeg.dylib")
#		EXECUTE_PROCESS (COMMAND ditto "@QT_PREFIX@/plugins/imageformats/libqjpeg.dylib" "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats/libqjpeg.dylib")
#	ENDIF()

ENDIF (APPLE)

#Install our MIME data
INSTALL(FILES
  scribus.xml
  DESTINATION ${MIMEDIR}
)
#KDE workaround until it starts supporting freedesktop spec
INSTALL(FILES
  vnd.scribus.desktop
  DESTINATION ${MIMELNKDIR}
)

#If building an Apple bundle, install these specific files
IF(APPLEBUNDLE)
  INSTALL(FILES
    Scribus.app/Contents/Info.plist
    DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
  INSTALL(FILES
    Scribus.app/Contents/Resources/Scribus.icns
    Scribus.app/Contents/Resources/Scribus-doc.icns
    DESTINATION ${CMAKE_INSTALL_PREFIX}/Resources
  )
ENDIF(APPLEBUNDLE)

##############################################################################################################
########## Install/CPack Setup                                                                      ##########

# If the cmake version includes cpack, use it for tarball building
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Scribus is an open source publishing application for Linux, Mac OSX and Windows")
  SET(CPACK_PACKAGE_VENDOR "Scribus Team")
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/README")
  SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
  SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
  SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Scribus ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
  SET(CPACK_PACKAGE_EXECUTABLES "scribus${TAG_VERSION}")
  SET(CPACK_SOURCE_PACKAGE_FILE_NAME "scribus-${VERSION}${VERSION_SUFFIX}")
  SET(CPACK_SOURCE_GENERATOR TBZ2)
  SET(CPACK_SYSTEM_NAME "")
  SET(CPACK_TOPLEVEL_TAG "")
  SET(CPACK_SOURCE_IGNORE_FILES
    CMakeCache.txt
    scribus-1.5.0.svn.tar.bz2
    scribus-1.5.0.svn.tar.Z
    scribus-1.5.0.svn.tar.gz
    "~$"
    "\\\\.cvsignore$"
    "\\\\.o$"
    "\\\\.svn-base$"
    "\\\\.svn$"
    "^${PROJECT_SOURCE_DIR}.*/CVS/"
    "^${PROJECT_SOURCE_DIR}/debian/"
    "^${PROJECT_SOURCE_DIR}/old/"
    "^${PROJECT_SOURCE_DIR}.*/CVSROOT/"
    "^${PROJECT_SOURCE_DIR}/admin/"
  )
  INCLUDE(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

