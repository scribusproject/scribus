##############################################################################################################
########## CMake Setup                                                                              ##########

#Set our CMake minimum version
#Require 2.8.9 for Qt5 and CMAKE_POSITION_INDEPENDENT_CODE property
#Require 3.1.0 for Qt 5.7 C++ 11 easy support
#Require 3.2.0 for add_custom_target with byproducts
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)


# Configure CCache if available and wanted
if (WANT_CCACHE)
    find_program(CCACHE_FOUND ccache)
	if(CCACHE_FOUND)
		message(STATUS "Enabling ccache")
		set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
		set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
	endif()
endif()

#Pretty colors
set(CMAKE_COLOR_MAKEFILE ON)
#Don't force verbose
set(CMAKE_VERBOSE_MAKEFILE OFF)
#Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
#Don't allow in source builds
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
#set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


#RPATH setup - more below too
if (WANT_NORPATH OR WANT_DISTROBUILD)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
else()
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_SKIP_RULE_DEPENDENCY TRUE)
set(CMAKE_SKIP_BUILD_RPATH TRUE)

include(CheckIncludeFile)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckTypeSize)
include(TestBigEndian)
include(GNUInstallDirs)
#include(FeatureSummary)

#enable_testing()

#Set the custom CMake module directory where our include/lib finders are
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

##############################################################################################################
########## toplevel compiler flags                                                                  ##########
message(STATUS "Shared Library Flags: ${CMAKE_SHARED_LIBRARY_C_FLAGS}")

#Project Setup
project(scribus)
#Set our version values
#Final version is ${VERSION} = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_SUFFIX}
#where VERSION_SUFFIX is of the form "", "svn" or "Nsvn" (N being the minor patch level)
set (VERSION_MAJOR "1")
set (VERSION_MINOR "5")
set (VERSION_PATCH "4")
set (VERSION_SUFFIX "")
set (VERSION ${VERSION_MAJOR})
if (VERSION_MINOR GREATER -1)
	set (VERSION ${VERSION}.${VERSION_MINOR})
endif()
if (VERSION_PATCH GREATER -1)
	set (VERSION ${VERSION}.${VERSION_PATCH})
endif()
if (VERSION_SUFFIX)
	set (VERSION ${VERSION}.${VERSION_SUFFIX})
endif()
message(STATUS "Scribus ${VERSION} will be built and installed into ${CMAKE_INSTALL_PREFIX}")

#Set the permissions to be used when installing plugins
set(PLUGIN_PERMISSIONS WORLD_EXECUTE GROUP_EXECUTE OWNER_EXECUTE WORLD_READ GROUP_READ OWNER_READ OWNER_WRITE)

#Our main directory is scribus
set(MAIN_DIR_NAME "scribus")

##############################################################################################################
########## check for the CPU we build for                                                           ##########

execute_process(
	COMMAND ${CMAKE_C_COMPILER} -dumpmachine
	OUTPUT_VARIABLE MACHINE
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

## Find out what machine/cpu we are running on
message(STATUS  "Machine: ${MACHINE}, void pointer size: ${CMAKE_SIZEOF_VOID_P}")
string(REGEX MATCH "(i[0-9]86-*)|(athlon-*)|(pentium-*)" _machine_x86 "${MACHINE}")
if (_machine_x86)
	message(STATUS "Found target X86")
	set(ARCH_X86 1)
endif()

string(REGEX MATCH "(x86_64-*)|(X86_64-*)|(AMD64-*)|(amd64-*)" _machine_x86_64 "${MACHINE}")
if (_machine_x86_64)
	message(STATUS "Found target X86_64")
	set(ARCH_X86_64 1)
endif()

string(REGEX MATCH "(sparc64-*)|(SPARC64-*)" _machine_sparc_64 "${MACHINE}")
if (_machine_sparc_64)
	message(STATUS "Found target SPARC 64")
	set(ARCH_SPARC_64 1)
endif()

string(REGEX MATCH "(mips64-*)|(MIPS64-*)" _machine_mips_64 "${MACHINE}")
if (_machine_mips_64)
	message(STATUS "Found target MIPS 64")
	set(ARCH_MIPS_64 1)
endif()

string(REGEX MATCH "(ppc-*)|(powerpc-*)" _machine_ppc "${MACHINE}")
if (_machine_ppc)
	message(STATUS "Found target PPC")
	set(ARCH_PPC 1)
endif()

string(REGEX MATCH "(ppc64-*)|(PPC64-*)|(powerpc64-*)" _machine_ppc_64 "${MACHINE}")
if (_machine_ppc_64)
	message(STATUS "Found target PPC64")
	set(ARCH_PPC_64 1)
endif()

string(REGEX MATCH "(sparc-*)" _machine_sparc "${MACHINE}")
if (_machine_sparc)
	message(STATUS "Found target Sparc")
	set(ARCH_SPARC 1)
endif()

string(REGEX MATCH "(sparcv9-*)" _machine_sparcv9 "${MACHINE}")
if (_machine_sparcv9)
	message(STATUS "Found target Sparc v9")
	set(ARCH_SPARCV9 1)
endif()

string(REGEX MATCH "(sparc64-*)" _machine_sparc64 "${MACHINE}")
if (_machine_sparc64)
	message(STATUS "Found target Sparc64")
	set(ARCH_SPARC64 1)
	set(ARCH64BIT 1)
endif()

string(REGEX MATCH "(hppa*)" _machine_hppa "${MACHINE}")
if (_machine_hppa)
	message(STATUS "Found target Hppa")
	set(ARCH_HPPA 1)
endif()

# We need to pass -fPIC to lib2geom on amd64, mips, mipsel, and hppa. See:
# http://www.gentoo.org/proj/en/base/amd64/howtos/index.xml?part=1&chap=3 and
# http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=559133

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## Do our Apple OSX version setup
if (APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND (ARCH_X86 EQUAL 1 OR ARCH_X86_64 EQUAL 1))
	string(REGEX REPLACE ".*-darwin([0-9]+).*" "\\1" _apple_ver "${MACHINE}")
	if (_apple_ver EQUAL "17")
		message(STATUS "Found macOS High Sierra Target: Apple, 64 bit, X86")
		set(APPLE_10_13_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "16")
	    message(STATUS "Found macOS Sierra Target: Apple, 64 bit, X86")
		set(APPLE_10_12_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "15")
		message(STATUS "Found OSX El Capitan Target: Apple, 64 bit, X86")
		set(APPLE_10_11_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "14")
		message(STATUS "Found OSX Yosemite Target: Apple, 64 bit, X86")
		set(APPLE_10_10_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "13")
		message(STATUS "Found OSX Mavericks Target: Apple, 64 bit, X86")
		set(APPLE_10_9_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "12")
		message(STATUS "Found OSX Mountain Lion Target: Apple, 64 bit, X86")
		set(APPLE_10_8_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "11")
		message(STATUS "Found OSX Lion Target: Apple, 64 bit, X86")
		set(APPLE_10_7_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "10")
		message(STATUS "Found OSX Snow Leopard Target: Apple, 64 bit, X86")
		set(APPLE_10_6_X 1 CACHE TYPE BOOL)
	endif()
	if (_apple_ver EQUAL "9")
		message(STATUS "Found OSX Leopard Target: Apple, 32 bit, X86")
		set(APPLE_10_5_X 1 CACHE TYPE BOOL)
	endif()
	unset(ARCH_X86)
endif()


#convert any 64 bit build into generic 64 tag for below
if (ARCH_X86_64 OR ARCH_SPARC_64 OR ARCH_MIPS_64 OR ARCH_PPC_64)
	set(ARCH64BIT 1)
endif()

message(STATUS "Building for target ${MACHINE}")

##############################################################################################################
########## Versioning Setup                                                                         ##########

#On Apple, we ignore the versioning tag so all items are "scribus" not "scribus-version"
if (NOT BUILD_OSX_BUNDLE)
	set(BUILD_OSX_BUNDLE ${APPLE} CACHE TYPE BOOL)
endif()

#Simplify future conditionals for Apple
if (APPLE AND BUILD_OSX_BUNDLE)
	set(APPLEBUNDLE TRUE CACHE TYPE BOOL)
endif()

#Announce we cached a version request before, overridden below for OSX
if(TAG_VERSION)
	if (NOT APPLEBUNDLE)
		message(STATUS "Previously selected version tag: ${TAG_VERSION}")
	endif()
	set(WANT_VERSIONING 1)
	set(CACHED_VERSIONING 1)
endif()

#Remove version tags on OSX so our bundle is Scribus.app
if (APPLEBUNDLE OR WIN32)
	if (WANT_VERSIONING OR CACHED_VERSIONING)
		message(STATUS "Ignoring version tag on Win32 or OSX when building bundle")
		set(WANT_VERSIONING 0)
		set(CACHED_VERSIONING 0)
	endif()
endif()

#Store our version string if required
if (WANT_VERSIONING AND NOT CACHED_VERSIONING)
	if (NOT CUSTOM_VERSIONTAG)
		set(TAG_VERSION "-${VERSION}" CACHE TYPE STRING)
	else()
		set(TAG_VERSION ${CUSTOM_VERSIONTAG} CACHE TYPE STRING)
	endif()
endif()

##############################################################################################################
########## Install/Directory Setup                                                                  ##########

if (APPLEBUNDLE)
set (APPLE_APP_DIR "${CMAKE_INSTALL_PREFIX}")
	if ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/Contents/?$")
		# all ok
	else()
		if ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/?$")
			# add "/Contents" subpath
			set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/Contents")
		else()
			# no bundle name spcified, use appname and version
			set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/Scribus${VERSION}.app/Contents")
		endif()
	endif()
	message(STATUS "APPLEBUNDLE install prefix: ${APPLE_APP_DIR}")
endif()


#Setup all the directories we will use
#cmake_policy(SET CMP0005 OLD)
#SHARE
if(WANT_VERSIONING)
	set(SHAREDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/")
else()
	set(SHAREDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}/")
endif()
if(NOT IS_ABSOLUTE ${SHAREDIR})
	add_definitions(-DSHAREDIR="${CMAKE_INSTALL_PREFIX}/${SHAREDIR}")
else()
	add_definitions(-DSHAREDIR="${SHAREDIR}")
endif()

#SHARE - use the default on Apple as TAG_VERSION is empty
if(WIN32 OR OS2)
	set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/")
else()
	if(TAG_VERSION OR BUILD_OSX_BUNDLE)
		set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${MAIN_DIR_NAME}${TAG_VERSION}/")
	else()
		if(NOT WANT_VERSIONING)
			set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${MAIN_DIR_NAME}/")
		else()
			set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${MAIN_DIR_NAME}-${VERSION}/")
		endif()
	endif()
endif()
if(NOT IS_ABSOLUTE ${DOCDIR})
	add_definitions(-DDOCDIR="${CMAKE_INSTALL_PREFIX}/${DOCDIR}")
else()
	add_definitions(-DDOCDIR="${DOCDIR}")
endif()

#ICONS
if(WIN32)
	set(ICONDIR "${CMAKE_INSTALL_DATAROOTDIR}/icons/")
else()
	set(ICONDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/icons/")
endif()
if(NOT IS_ABSOLUTE ${ICONDIR})
	add_definitions(-DICONDIR="${CMAKE_INSTALL_PREFIX}/${ICONDIR}")
else()
	add_definitions(-DICONDIR="${ICONDIR}")
endif()

#SAMPLES
set(SAMPLESDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/samples/")
if(NOT IS_ABSOLUTE ${SAMPLESDIR})
	add_definitions(-DSAMPLESDIR="${CMAKE_INSTALL_PREFIX}/${SAMPLESDIR}")
else()
	add_definitions(-DSAMPLESDIR="${SAMPLESDIR}")
endif()

#SCRIPTS
set(SCRIPTSDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/scripts/")
if(NOT IS_ABSOLUTE ${SCRIPTSDIR})
	add_definitions(-DSCRIPTSDIR="${CMAKE_INSTALL_PREFIX}/${SCRIPTSDIR}")
else()
	add_definitions(-DSCRIPTSDIR="${SCRIPTSDIR}")
endif()

#TEMPLATES
if(WIN32)
	set(TEMPLATEDIR "${CMAKE_INSTALL_DATAROOTDIR}/templates/")
else()
	set(TEMPLATEDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/templates/")
endif()
if(NOT IS_ABSOLUTE ${TEMPLATEDIR})
	add_definitions(-DTEMPLATEDIR="${CMAKE_INSTALL_PREFIX}/${TEMPLATEDIR}")
else()
	add_definitions(-DTEMPLATEDIR="${TEMPLATEDIR}")
endif()

#QML
if(WIN32)
	set(QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml/")
else()
	set(QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/qml/")
endif()
add_definitions(-DQMLDIR="${CMAKE_INSTALL_PREFIX}/${QMLDIR}")

#DESKTOP
set(DESKTOPDIR "${CMAKE_INSTALL_DATAROOTDIR}/applications/")
if(NOT IS_ABSOLUTE ${DESKTOPDIR})
	add_definitions(-DDESKTOPDIR="${CMAKE_INSTALL_PREFIX}/${DESKTOPDIR}")
else()
	add_definitions(-DDESKTOPDIR="${DESKTOPDIR}")
endif()

#APPDATA

set(APPDATADIR "${CMAKE_INSTALL_DATAROOTDIR}/metainfo/")
if(NOT IS_ABSOLUTE ${APPDATADIRR})
	add_definitions(-DAPPDATADIR="${CMAKE_INSTALL_PREFIX}/${APPDATADIR}")
else()
	add_definitions(-DAPPDATADIR="${APPDATADIR}")
endif()


# APPLICATION_DATA_DIR
# It allows specify the user configuration directory in the compile time.
# Example: I want to use special ~/.scribus135svn config directory for my trunk
#          cmake -DAPPLICATION_DATA_DIR=.scribus135svn ...
if (APPLICATION_DATA_DIR)
    message(STATUS "Using ApplicationDataDir ${APPLICATION_DATA_DIR} instead of the platform default.")
	add_definitions(-DAPPLICATION_DATA_DIR="${APPLICATION_DATA_DIR}")
else()
    message(STATUS "Using standard ApplicationDataDir. You can change it with -DAPPLICATION_DATA_DIR")
endif()

if (APPLICATION_CONFIG_DIR)
    message(STATUS "Using ApplicationConfigDir ${APPLICATION_CONFIG_DIR} instead of the platform default.")
	add_definitions(-DAPPLICATION_CONFIG_DIR="${APPLICATION_CONFIG_DIR}")
else()
    message(STATUS "Using standard ApplicationConfigDir. You can change it with -DAPPLICATION_CONFIG_DIR")
endif()

#LIB LOCATION - lib or lib64 normally
#LIB
if(WANT_LIB64)
	if(NOT ARCH_X86_64)
		message(STATUS "WARNING: BUILDING 64 Bit build on non 64 bit architecture")
	endif()
	 set(LIB_SUFFIX "64" CACHE TYPE STRING)
endif()
set(LIB_DIR_NAME "lib${LIB_SUFFIX}")
if (APPLEBUNDLE)
	set(LIBDIR "lib/")
else()
	set(LIBDIR "${LIB_DIR_NAME}/${MAIN_DIR_NAME}${TAG_VERSION}/")
endif()
add_definitions(-DLIBDIR="${CMAKE_INSTALL_PREFIX}/${LIBDIR}")

#PLUGINS
if(WIN32)
	set(PLUGINDIR "plugins")
else()
	if (APPLEBUNDLE)
		set(PLUGINDIR "${LIBDIR}")
	else()
		set(PLUGINDIR "${LIB_DIR_NAME}/${MAIN_DIR_NAME}${TAG_VERSION}/plugins/")
	endif()
endif()
add_definitions(-DPLUGINDIR="${CMAKE_INSTALL_PREFIX}/${PLUGINDIR}")


#INCLUDE
set(INCLUDEDIR "include/${MAIN_DIR_NAME}${TAG_VERSION}/")
#NO ADD DEFINITIONS, WE ONLY INSTALL WITH IT

#RPATH
if (WANT_NORPATH OR WANT_DISTROBUILD)
	set(CMAKE_INSTALL_RPATH "")
else()
	set(CMAKE_INSTALL_RPATH ${PLUGINDIR})
endif()

# Set the XDG mime stuff
set(MIMEDIR "${CMAKE_INSTALL_DATAROOTDIR}/mime/packages/")

##############################################################################################################
########## Build Setup                                                                              ##########

#Convert our simpler command line option to the CMake style
#None, Debug, Release, .. or custom ones
# set(WANT_DEBUG 1)
if(WANT_DEBUG)
	set(CMAKE_BUILD_TYPE Debug)
endif()
if (WANT_RELEASEWITHDEBUG)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
if(NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)
	set(CMAKE_BUILD_TYPE RELEASE)
endif()



#Based on our build type, setup our build options
if(APPLE)
	if(APPLE_10_13_X)
		set(OSXMINVER "10.13" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			if(EXISTS("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk"))
				set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk")
			elseif(EXISTS("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk"))
				set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk")
			endif()
		endif()
	endif()
	if(APPLE_10_12_X)
	    set(OSXMINVER "10.12" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
		    if(EXISTS("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk"))
			    set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk")
			elseif(EXISTS("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"))
			    set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk")
			endif()
		endif()
	endif()
	if(APPLE_10_11_X)
		set(OSXMINVER "10.11" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk")
		endif()
	endif()
	if(APPLE_10_10_X)
		set(OSXMINVER "10.10" CACHE TYPE STRING)
		set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")
		if (WANT_OSX_SDK)
			if(EXISTS("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk"))
				set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk")
			elseif(EXISTS("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"))
				set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk")
			endif()
		endif()
	endif()
	if(APPLE_10_9_X)
		set(OSXMINVER "10.9" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk")
		endif()
	endif()
	if(APPLE_10_8_X)
		set(OSXMINVER "10.8" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk")
		endif()
	endif()
	if(APPLE_10_7_X)
		set(OSXMINVER "10.7" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk")
		endif()
	endif()
	if(APPLE_10_6_X)
		set(OSXMINVER "10.6" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk")
		endif()
	endif()
	if(APPLE_10_5_X)
		set(OSXMINVER "10.5" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.5.sdk")
		endif()
	endif()
	if(APPLE_10_4_X)
		set(OSXMINVER "10.4" CACHE TYPE STRING)
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.4u.sdk")
		endif()
	endif()
	# gcc/mac
	if(WANT_UNIVERSAL_BUNDLE)
		message("Setting Universal (PPC/i386) Architecture for OSX Build/Bundle")
		set(CMAKE_OSX_ARCHITECTURES "ppc;i386" )
		set(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "ppc;i386" )
		if (WANT_OSX_SDK)
			set(CMAKE_OSX_DEPLOYMENT_TARGET "10.4")
		endif()
	else()
		if(ARCH_PPC OR ARCH_PPC_64)
			message("Setting PPC Architecture for OSX Build/Bundle")
			set(CMAKE_OSX_ARCHITECTURES "ppc" )
			set(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "ppc" )
		else()
			if (WANT_OSX_SDK)
				set(CMAKE_OSX_DEPLOYMENT_TARGET "${OSXMINVER}")
			endif()
			if(APPLE_10_6_X OR APPLE_10_7_X OR APPLE_10_8_X OR APPLE_10_9_X OR APPLE_10_10_X OR APPLE_10_11_X OR APPLE_10_12_X OR APPLE_10_13_X)
				message("Setting x86_64 Architecture for OSX Build/Bundle")
				set(CMAKE_OSX_ARCHITECTURES "x86_64" )
				set(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "x86_64" )
			else()
				message("Setting i386 Architecture for OSX Build/Bundle")
				set(CMAKE_OSX_ARCHITECTURES "i386" )
				set(CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "i386" )
			endif()
		endif()
	endif()
	message(STATUS "${CMAKE_OSX_ARCHITECTURES} ${CMAKE_OSX_SYSROOT}")
	set(ENV{MACOSX_DEPLOYMENT_TARGET} ${OSXMINVER} )
	set(ENV{LD_PREBIND} 1 )
	set(ENV{LD_PREBIND_ALLOW_OVERLAP} 1 )
	if (WANT_DEBUG)
	    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
	endif()
	if (WANT_RELEASEWITHDEBUG)
	    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
		set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
	endif()
	if (NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)
	    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
		set(CMAKE_C_FLAGS_RELEASE "-O2 -fno-inline-functions -Wall -mmacosx-version-min=${OSXMINVER}")
	endif()
else()
	if(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
		# Windows build with Visual Studio
		# Surely there's a better way to identify the compiler?
		set(CMAKE_CXX_FLAGS_DEBUG)
		set(CMAKE_C_FLAGS_DEBUG)
	else()
		# vanilla gcc
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -fstrict-aliasing")
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -fstrict-aliasing")
		if (_machine_x86 OR _machine_x86_64)
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
			set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fPIC")
		endif()
	endif()
endif()

if(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
	set(CMAKE_CXX_FLAGS_RELEASE)
	set(CMAKE_C_FLAGS_RELEASE)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -fPIC")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -fPIC")
#	add_definitions(-DNDEBUG)
endif()

if(WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN
					-DAVOID_WIN32_FILEIO
					-D_CRT_SECURE_NO_DEPRECATE
					-D_USE_MATH_DEFINES
					-DCOMPILE_PLUGIN_AS_DLL
					)
	set(BUILD_WINDOWS_VERSION 1)
	set(DLL_USE_NATIVE_API 1)
endif()

##############################################################################################################
########## Find Dependencies                                                                        ##########

#<< Qt
set (QT_MIN_VERSION "5.5.0")
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
if (NOT WANT_CPP11 AND Qt5Widgets_VERSION_MAJOR EQUAL 5 AND Qt5Widgets_VERSION_MINOR GREATER 6)
	message(STATUS "Your Qt version is 5.7.0 or superior, enabling C++11 compiler features")
	set(WANT_CPP11 1)
endif()
#>> Qt


#C++11 Support
if(WANT_CPP11)
	message(STATUS "Enabling C++11 compiler features")
	set(CMAKE_CXX_STANDARD 11)
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
	set(HAVE_LIBZ 1)
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
		set(HAVE_OSG 1)
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
	set(HAVE_TIFF 1)
else()
	message(FATAL_ERROR "Could not find the TIFF Library")
endif()
#<< JPEG, TIFF

#<< PYTHON
#set(PythonLibs_FIND_VERSION 2)
find_package(PythonLibs 2 REQUIRED)
if (PYTHON_LIBRARY)
	message("Python Library Found OK")
	set(HAVE_PYTHON 1)
	set(COMPILE_PYTHON 1)
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
	set(RENDER_LIB "CAIRO_SYSTEM" CACHE TYPE STRING)
else()
	message(FATAL_ERROR "No Cairo Found")
endif()
#>>Render Library

#<<CUPS
if(NOT WIN32 AND NOT HAIKU)
	find_package(Cups REQUIRED)
	if(CUPS_FOUND)
		set(HAVE_CUPS 1)
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
	set(HAVE_XML 1)
	message("LIBXML2 Library Found OK")
endif()
#>>LibXML2

#<<LittleCMS
set(LCMS_DIR ${CMAKE_MODULE_PATH})
find_package(LCMS2 REQUIRED)
if(LCMS2_FOUND)
	set(HAVE_LCMS2 1)
	set(HAVE_CMS 1)
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
		set(HAVE_FONTCONFIG 1)
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
		set(HAVE_HUNSPELL 1)
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
		set(HAVE_PODOFO 1)
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
		set(HAVE_BOOST 1)
	else()
		message("Boost: NOT FOUND, not building 2geomtools")
	endif()
endif()
#>>Boost for 2Geom Tools

#<<GraphicksMagick for image import
if (WANT_GRAPHICSMAGICK)
	set(GMAGICK_DIR ${CMAKE_MODULE_PATH})
	pkg_check_modules(GMAGICK GraphicsMagick)
	# find_package(GMAGICK)
	if(GMAGICK_FOUND)
		message("GraphicksMagick Found OK")
	else()
		message("GraphicksMagick NOT found! Building without additional image format support!")
	endif()
else()
	message("Building without GraphicksMagick (use -DWANT_GRAPHICSMAGICK=1 to enable)")
endif()
#>>GraphicksMagick for image import

#<<Poppler for PDF import
find_package(POPPLER REQUIRED)
if (POPPLER_FOUND)
	if (FREETYPE_FOUND)
		set( HAVE_POPPLER 1 )
		message(STATUS "Found poppler")
		message(STATUS "Found poppler libs: ${POPPLER_LIBRARY}")
		message(STATUS "Found poppler includes: ${POPPLER_INCLUDE_DIR}")
	else()
		message("Poppler NOT found - Disabling support for native PDF import")
	endif()
else()
	message("Poppler NOT found - Disabling support for native PDF import")
endif()
#>>Poppler for PDF import

# librevenge for MsPub import
pkg_check_modules(LIBREVENGE librevenge-0.0)
if (LIBREVENGE_FOUND)
	pkg_check_modules(LIBREVENGE_STREAM librevenge-stream-0.0)
	if (LIBREVENGE_STREAM_FOUND)
		pkg_check_modules(LIBREVENGE_GENERATORS librevenge-generators-0.0)
		if (LIBREVENGE_GENERATORS_FOUND)
			set(HAVE_REVENGE 1)
		endif()
	endif()
endif()

# CTL support
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz>=0.9.42 harfbuzz-icu)
pkg_check_modules(ICU REQUIRED icu-uc)


##############################################################################################################
########## Include Setup                                                                            ##########

TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

CHECK_INCLUDE_FILE("dlfcn.h" HAVE_DLFCN_H)
if(HAVE_DLFCN_H)
	add_definitions(-DHAVE_DLFCN_H)
endif()

CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
	add_definitions(-DHAVE_UNISTD_H)
endif()

CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
# if(HAVE_SYS_TYPES_H)
#   add_definitions(-DHAVE_SYS_TYPES_H)
# endif()

CHECK_INCLUDE_FILE("sys/stat.h" HAVE_SYS_STAT_H)
# if(HAVE_SYS_STAT_H)
#   add_definitions(-DHAVE_SYS_STAT_H)
# endif()
#>>Test for existing include files


#Create configure files.. config.h and uninstall
#config.h
include (ConfigureChecks.cmake)
	if(WIN32)
		set(CONFIG_NAME win-config.h)
	else()
		set(CONFIG_NAME config.h)
	endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_NAME})
add_definitions(-DHAVE_CONFIG_H)



#Set up include dirs with all found packages
include_directories(
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

if (NOT WIN32 AND NOT HAIKU)
include_directories(
	${CUPS_INCLUDE_DIR}
)
endif()

##############################################################################################################
########## Uninstall Setup                                                                          ##########

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY
)

add_custom_target(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)


##############################################################################################################
########## Add our subdirs                                                                          ##########

#Add our source subdirs
add_subdirectory(scribus)
add_subdirectory(doc)
add_subdirectory(resources/dicts)
add_subdirectory(resources/editorconfig)
add_subdirectory(resources/iconsets)
add_subdirectory(resources/keysets)
add_subdirectory(resources/loremipsum)
add_subdirectory(resources/manpages)
add_subdirectory(resources/profiles)
add_subdirectory(resources/swatches)
add_subdirectory(resources/templates)
add_subdirectory(resources/translations)

#Install our READMEs etc.
install(FILES
	AUTHORS
	ChangeLog
	COPYING
	LINKS
	README
	TRANSLATION
	DESTINATION ${DOCDIR}
)

if (APPLE)
	install(FILES
		README.MacOSX
		DESTINATION ${DOCDIR}
	)

	#	execute_process (COMMAND mkdir -p "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats")
	#	if (NOT EXISTS "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats/libqjpeg.dylib")
	#message(STATUS ${QT_PREFIX})
	#message(STATUS "@QT_PREFIX@/plugins/imageformats/libqjpeg.dylib")
	#message(STATUS "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats/libqjpeg.dylib")
	#		execute_process (COMMAND ditto "@QT_PREFIX@/plugins/imageformats/libqjpeg.dylib" "${CMAKE_INSTALL_PREFIX}/PlugIns/imageformats/libqjpeg.dylib")
	#	endif()

endif()

#Install the .desktop file
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/scribus.desktop.in
	${CMAKE_CURRENT_SOURCE_DIR}/scribus.desktop
)
install(FILES
	scribus.desktop
	RENAME scribus${TAG_VERSION}.desktop
	DESTINATION ${DESKTOPDIR}
)

#Install our MIME data
install(FILES
	scribus.xml
	RENAME scribus${TAG_VERSION}.xml
	DESTINATION ${MIMEDIR}
)


#Install the appdata file
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/scribus.appdata.xml.in
	${CMAKE_CURRENT_SOURCE_DIR}/scribus.appdata.xml
)
install(FILES
	scribus.appdata.xml
	RENAME scribus${TAG_VERSION}.appdata.xml
	DESTINATION ${APPDATADIR}
)


#If building an Apple bundle, install these specific files
if(APPLEBUNDLE)
	install(FILES
		Scribus.app/Contents/Info.plist
		DESTINATION ${CMAKE_INSTALL_PREFIX}
	)
	install(FILES
		Scribus.app/Contents/Resources/Scribus.icns
		Scribus.app/Contents/Resources/Scribus-doc.icns
		DESTINATION ${CMAKE_INSTALL_PREFIX}/Resources
	)
endif()

##############################################################################################################
########## Install/CPack Setup                                                                      ##########

# If the cmake version includes cpack, use it for tarball building
if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Scribus is an open source publishing application for Linux, Mac OSX and Windows")
	set(CPACK_PACKAGE_VENDOR "Scribus Team")
	set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/README")
	set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
	set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
	set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Scribus ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
	set(CPACK_PACKAGE_EXECUTABLES "scribus${TAG_VERSION}")
	set(CPACK_SOURCE_PACKAGE_FILE_NAME "scribus-${VERSION}${VERSION_SUFFIX}")
	set(CPACK_SOURCE_GENERATOR TBZ2)
	set(CPACK_SYSTEM_NAME "")
	set(CPACK_TOPLEVEL_TAG "")
	set(CPACK_SOURCE_IGNORE_FILES
		CMakeCache.txt
		scribus-1.5.4.tar.bz2
		scribus-1.5.4.tar.Z
		scribus-1.5.4.tar.gz
		scribus-1.5.4.svn.tar.bz2
		scribus-1.5.4.svn.tar.Z
		scribus-1.5.4.svn.tar.gz
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
	include(CPack)
endif()

#include (cmakeconfigreport optional)

# Output everything we've found
#feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES)
