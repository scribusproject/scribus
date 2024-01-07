##############################################################################################################
########## CMake Setup                                                                              ##########

#Set our CMake minimum version
#Require 2.8.9 for Qt5 and CMAKE_POSITION_INDEPENDENT_CODE property
#Require 3.1.0 for Qt 5.7 C++ 11 easy support
#Require 3.2.0 for add_custom_target with byproducts
#Require 3.12 for new Find Python modules
#Require 3.14 for new Fontconfig module

cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)
message(STATUS "CMake Version: ${CMAKE_VERSION}")
if (WANT_PCH)
	if(${CMAKE_VERSION} VERSION_LESS "3.16.0")
		message(STATUS "Precompiled headers requested however CMake version < 3.16.0")
		set(WANT_PCH OFF)
	else()
		message(STATUS "Precompiled headers enabled")
	endif()
else()
	set(WANT_PCH OFF)
	message(STATUS "Precompiled headers disabled")
endif()

#Set our version values
#Final version is ${VERSION} = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_SUFFIX}
#where VERSION_SUFFIX is of the form "", "svn" or "Nsvn" (N being the minor patch level)
set (VERSION_MAJOR "1")
set (VERSION_MINOR "6")
set (VERSION_PATCH "1")
set (VERSION_SUFFIX "")
set (VERSION ${VERSION_MAJOR})
if (VERSION_MINOR GREATER -1)
	set (VERSION ${VERSION}.${VERSION_MINOR})
endif()
if (VERSION_PATCH GREATER -1)
	set (VERSION ${VERSION}.${VERSION_PATCH})
endif()

#Project Setup
#Do this now, prior to adding the suffix for SVN
project(scribus VERSION ${VERSION}) # LANGUAGES CXX C)
if (VERSION_SUFFIX)
	set (VERSION ${VERSION}.${VERSION_SUFFIX})
endif()
message(STATUS "Scribus ${VERSION} will be built and installed into ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


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
set(CMAKE_INCLUDE_CURRENT_DIR ON)
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
	set(ARCH_X86 ON)
endif()

string(REGEX MATCH "(x86_64-*)|(X86_64-*)|(AMD64-*)|(amd64-*)" _machine_x86_64 "${MACHINE}")
if (_machine_x86_64)
	message(STATUS "Found target X86_64")
	set(ARCH_X86_64 ON)
endif()

string(REGEX MATCH "(sparc64-*)|(SPARC64-*)" _machine_sparc_64 "${MACHINE}")
if (_machine_sparc_64)
	message(STATUS "Found target SPARC 64")
	set(ARCH_SPARC_64 ON)
endif()

string(REGEX MATCH "(mips64-*)|(MIPS64-*)" _machine_mips_64 "${MACHINE}")
if (_machine_mips_64)
	message(STATUS "Found target MIPS 64")
	set(ARCH_MIPS_64 ON)
endif()

string(REGEX MATCH "(ppc-*)|(powerpc-*)" _machine_ppc "${MACHINE}")
if (_machine_ppc)
	message(STATUS "Found target PPC")
	set(ARCH_PPC ON)
endif()

string(REGEX MATCH "(ppc64-*)|(PPC64-*)|(powerpc64-*)" _machine_ppc_64 "${MACHINE}")
if (_machine_ppc_64)
	message(STATUS "Found target PPC64")
	set(ARCH_PPC_64 ON)
endif()

string(REGEX MATCH "(sparc-*)" _machine_sparc "${MACHINE}")
if (_machine_sparc)
	message(STATUS "Found target Sparc")
	set(ARCH_SPARC ON)
endif()

string(REGEX MATCH "(sparcv9-*)" _machine_sparcv9 "${MACHINE}")
if (_machine_sparcv9)
	message(STATUS "Found target Sparc v9")
	set(ARCH_SPARCV9 ON)
endif()

string(REGEX MATCH "(sparc64-*)" _machine_sparc64 "${MACHINE}")
if (_machine_sparc64)
	message(STATUS "Found target Sparc64")
	set(ARCH_SPARC64 ON)
	set(ARCH64BIT ON)
endif()

string(REGEX MATCH "(hppa+)" _machine_hppa "${MACHINE}")
if (_machine_hppa)
	message(STATUS "Found target Hppa")
	set(ARCH_HPPA ON)
endif()

string(REGEX MATCH "(arm+)" _machine_arm "${MACHINE}")
if (_machine_arm)
	message(STATUS "Found target arm")
	set(ARCH_ARM ON)
endif()

string(REGEX MATCH "(arm64+)" _machine_arm64 "${MACHINE}")
if (_machine_arm64)
	message(STATUS "Found target arm64")
	set(ARCH_ARM_64 ON)
endif()

# We need to pass -fPIC to lib2geom on amd64, mips, mipsel, and hppa. See:
# http://www.gentoo.org/proj/en/base/amd64/howtos/index.xml?part=1&chap=3 and
# http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=559133

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## Do our Apple OSX version setup
if (APPLE)
	if ((CMAKE_SIZEOF_VOID_P EQUAL 8) AND (ARCH_ARM_64))
		string(REGEX REPLACE ".*-darwin([0-9]+).*" "\\1" APPLE_OS_VERSION "${MACHINE}")
		if (APPLE_OS_VERSION EQUAL "23")
			message(STATUS "Found macOS Sonoma Target: Apple, 64 bit, X86")
			set(APPLE_14_00_X ON CACHE BOOL "Found macOS Sonoma Target: Apple, 64 bit, ARM")
		endif()
		if (APPLE_OS_VERSION EQUAL "22")
			message(STATUS "Found macOS Ventura Target: Apple, 64 bit, ARM")
			set(APPLE_13_00_X ON CACHE BOOL "Found macOS Ventura Target: Apple, 64 bit, ARM")
		endif()
		if (APPLE_OS_VERSION EQUAL "21")
			message(STATUS "Found macOS Monterey Target: Apple, 64 bit, ARM")
			set(APPLE_12_00_X ON CACHE BOOL "Found macOS Monterey Target: Apple, 64 bit, ARM")
		endif()
		if (APPLE_OS_VERSION EQUAL "20")
			message(STATUS "Found macOS Big Sur Target: Apple, 64 bit, ARM")
			set(APPLE_11_00_X ON CACHE BOOL "Found macOS Big Sur Target: Apple, 64 bit, ARM")
		endif()
	endif()
	if ((CMAKE_SIZEOF_VOID_P EQUAL 8) AND (ARCH_X86 OR ARCH_X86_64))
		string(REGEX REPLACE ".*-darwin([0-9]+).*" "\\1" APPLE_OS_VERSION "${MACHINE}")
		if (APPLE_OS_VERSION EQUAL "23")
			message(STATUS "Found macOS Sonoma Target: Apple, 64 bit, X86")
			set(APPLE_14_00_X ON CACHE BOOL "Found macOS Sonoma Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "22")
			message(STATUS "Found macOS Ventura Target: Apple, 64 bit, X86")
			set(APPLE_13_00_X ON CACHE BOOL "Found macOS Ventura Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "21")
			message(STATUS "Found macOS Monterey Target: Apple, 64 bit, X86")
			set(APPLE_12_00_X ON CACHE BOOL "Found macOS Monterey Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "20")
			message(STATUS "Found macOS Big Sur Target: Apple, 64 bit, X86")
			set(APPLE_11_00_X ON CACHE BOOL "Found macOS Big Sur Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "19")
			message(STATUS "Found macOS Catalina Target: Apple, 64 bit, X86")
			set(APPLE_10_15_X ON CACHE BOOL "Found macOS Catalina Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "18")
			message(STATUS "Found macOS Mojave Target: Apple, 64 bit, X86")
			set(APPLE_10_14_X ON CACHE BOOL "Found macOS Mojave Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "17")
			message(STATUS "Found macOS High Sierra Target: Apple, 64 bit, X86")
			set(APPLE_10_13_X ON CACHE BOOL "Found macOS High Sierra Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "16")
			message(STATUS "Found macOS Sierra Target: Apple, 64 bit, X86")
			set(APPLE_10_12_X ON CACHE BOOL "Found macOS Sierra Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "15")
			message(STATUS "Found OSX El Capitan Target: Apple, 64 bit, X86")
			set(APPLE_10_11_X ON CACHE BOOL "Found OSX El Capitan Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "14")
			message(STATUS "Found OSX Yosemite Target: Apple, 64 bit, X86")
			set(APPLE_10_10_X ON CACHE BOOL "Found OSX Yosemite Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "13")
			message(STATUS "Found OSX Mavericks Target: Apple, 64 bit, X86")
			set(APPLE_10_9_X ON CACHE BOOL "Found OSX Mavericks Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "12")
			message(STATUS "Found OSX Mountain Lion Target: Apple, 64 bit, X86")
			set(APPLE_10_8_X ON CACHE BOOL "Found OSX Mountain Lion Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "11")
			message(STATUS "Found OSX Lion Target: Apple, 64 bit, X86")
			set(APPLE_10_7_X ON CACHE BOOL "Found OSX Lion Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "10")
			message(STATUS "Found OSX Snow Leopard Target: Apple, 64 bit, X86")
			set(APPLE_10_6_X ON CACHE BOOL "Found OSX Snow Leopard Target: Apple, 64 bit, X86")
		endif()
		if (APPLE_OS_VERSION EQUAL "9")
			message(STATUS "Found OSX Leopard Target: Apple, 32 bit, X86")
			set(APPLE_10_5_X ON CACHE BOOL "Found OSX Leopard Target: Apple, 32 bit, X86")
		endif()
		unset(ARCH_X86)
	endif()
endif()


#convert any 64 bit build into generic 64 tag for below
if (ARCH_X86_64 OR ARCH_SPARC_64 OR ARCH_MIPS_64 OR ARCH_PPC_64 OR ARCH_ARM_64)
	set(ARCH64BIT ON)
endif()

message(STATUS "Building for target ${MACHINE}")

##############################################################################################################
########## Relocatability                                                                           ##########

if (APPLEBUNDLE OR WIN32)
	if (WANT_RELOCATABLE)
		message(STATUS "Ignoring relocatable option on Win32 or OSX when building bundle")
		set(WANT_RELOCATABLE OFF)
	endif()
endif()

if (WANT_RELOCATABLE)
	message(STATUS "Enabling relocatable binaries")
	set(WANT_RELOCATABLE ON CACHE BOOL "Enable relocatable binaries")
	add_definitions(-DWANT_RELOCATABLE)
endif()

##############################################################################################################
########## Versioning Setup                                                                         ##########

#On Apple, we ignore the versioning tag so all items are "scribus" not "scribus-version"
if (NOT BUILD_OSX_BUNDLE)
	set(BUILD_OSX_BUNDLE ${APPLE} CACHE BOOL "Building MacOS X Bundle")
endif()

#Simplify future conditionals for Apple
if (APPLE AND BUILD_OSX_BUNDLE)
	set(APPLEBUNDLE ON CACHE BOOL "Building Apple Bundle")
endif()

#Announce we cached a version request before, overridden below for OSX
if(TAG_VERSION)
	if (NOT APPLEBUNDLE)
		message(STATUS "Previously selected version tag: ${TAG_VERSION}")
	endif()
	set(WANT_VERSIONING ON)
	set(CACHED_VERSIONING ON)
endif()

#Remove version tags on OSX so our bundle is Scribus.app
if (APPLEBUNDLE OR WIN32)
	if (WANT_VERSIONING OR CACHED_VERSIONING)
		message(STATUS "Ignoring version tag on Win32 or OSX when building bundle")
		set(WANT_VERSIONING OFF)
		set(CACHED_VERSIONING OFF)
	endif()
endif()

#Store our version string if required
if (WANT_VERSIONING AND NOT CACHED_VERSIONING)
	if (NOT CUSTOM_VERSIONTAG)
		set(TAG_VERSION "-${VERSION}" CACHE STRING "Version string")
	else()
		set(TAG_VERSION ${CUSTOM_VERSIONTAG} CACHE STRING "Version string")
	endif()
endif()

##############################################################################################################
########## Install/Directory Setup                                                                  ##########
include (CMakeLists_Directories.cmake)

##############################################################################################################
########## Build Setup                                                                              ##########

#Convert our simpler command line option to the CMake style
#None, Debug, Release, .. or custom ones
if(WANT_DEBUG)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set Debug Build Type" FORCE)
endif()
if (WANT_RELEASEWITHDEBUG)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Set Release with Debug Info Build Type" FORCE)
endif()
if(NOT WANT_DEBUG AND NOT WANT_RELEASEWITHDEBUG)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Set Release Build Type" FORCE)
endif()

set(CMAKE_ENABLE_EXPORTS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

#Based on our build type, setup our build options
if(APPLE)
	### Include our Apple configure commands
	include(CMakeLists_Apple.cmake)
else()
	if(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
		# Windows build with Visual Studio
		# Surely there's a better way to identify the compiler?
		set(CMAKE_CXX_FLAGS_DEBUG)
		set(CMAKE_C_FLAGS_DEBUG)
	else()
		# vanilla gcc
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Wall -fstrict-aliasing")
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -Wall -fstrict-aliasing")
	endif()
	if(${CMAKE_GENERATOR} MATCHES "^(Visual Studio|NMake).*")
		set(CMAKE_CXX_FLAGS_RELEASE)
		set(CMAKE_C_FLAGS_RELEASE)
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall")
	#	add_definitions(-DNDEBUG)
	endif()
endif()
if(WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN
					-DAVOID_WIN32_FILEIO
					-D_CRT_SECURE_NO_DEPRECATE
					-D_USE_MATH_DEFINES
					-DCOMPILE_PLUGIN_AS_DLL
					)
	set(DLL_USE_NATIVE_API ON)
endif()

#C++11 Support
#as of 1.5.5.svn, require C++11
if(NOT WANT_CPP14 AND NOT WANT_CPP17)
	message(STATUS "Enabling C++11 compiler features")
	set(CMAKE_CXX_STANDARD 11)
endif()
if (WANT_CPP14)
	message(STATUS "Enabling C++14 compiler features")
	set(CMAKE_CXX_STANDARD 14)
elseif(WANT_CPP17)
		message(STATUS "Enabling C++17 compiler features")
		set(CMAKE_CXX_STANDARD 17)
endif()
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

##############################################################################################################
########## Find Dependencies                                                                        ##########
### Include our Dependency finding commands
include(CMakeLists_Dependencies.cmake)


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
#	${FONTCONFIG_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${LCMS2_INCLUDE_DIR}
	${LIBXML2_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
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
add_subdirectory(resources/unicodemap)

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
		scribus-1.5.9.tar.bz2
		scribus-1.5.9.tar.7z
		scribus-1.5.9.tar.gz
		scribus-1.5.9.svn.tar.bz2
		scribus-1.5.9.svn.tar.7z
		scribus-1.5.9.svn.tar.gz
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
