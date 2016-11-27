#
# Find the native FONTCONFIG includes and library
#

# This module defines
# FONTCONFIG_INCLUDE_DIR, where to find art*.h etc
# FONTCONFIG_LIBRARY, the libraries to link against to use FONTCONFIG.
# FONTCONFIG_FOUND, If false, do not try to use FONTCONFIG.
# LIBFONTCONFIG_LIBS, link information
# LIBFONTCONFIG_CFLAGS, cflags for include information

if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
  include(UsePkgConfig)
  PKGCONFIG(fontconfig _fontconfigIncDir _fontconfigLinkDir _fontconfigLinkFlags _fontconfigCflags)
  set(FONTCONFIG_LIBS ${_fontconfigCflags})
else (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
  include(FindPkgConfig)
  pkg_search_module(FONTCONFIG REQUIRED fontconfig)
endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)

#include(UsePkgConfig)

# use pkg-config to get the directories and then use these values
# in the find_path() and find_library() calls
#PKGCONFIG(fontconfig _fontconfigIncDir _fontconfigLinkDir _fontconfigLinkFlags _fontconfigCflags)

#set(FONTCONFIG_LIBS ${_fontconfigCflags})

if(BUILD_OSX_BUNDLE)
  find_path(FONTCONFIG_INCLUDE_DIR
    NAMES fontconfig/fontconfig.h
    PATHS ${FONTCONFIG_INCLUDE_DIRS} /opt/local/include
    NO_DEFAULT_PATH
  )
  find_library(FONTCONFIG_LIBRARY 
    NAMES fontconfig
    PATHS ${FONTCONFIG_LIBRARY_DIRS} /opt/local/lib
    NO_DEFAULT_PATH
  )
else(BUILD_OSX_BUNDLE)
  find_path(FONTCONFIG_INCLUDE_DIR 
    NAMES fontconfig/fontconfig.h
	PATHS ${FONTCONFIG_INCLUDE_DIRS} 
    ${_fontconfigIncDir}
    /usr/include
    /usr/local/include
    PATH_SUFFIXES fontconfig
  )
  # quick hack as the above finds it nicely but our source includes the libart_lgpl text at the moment
  #string(REGEX REPLACE "/libart_lgpl" "" FONTCONFIG_INCLUDE_DIR ${FONTCONFIG_INCLUDE_DIR})
  find_library(FONTCONFIG_LIBRARY NAMES fontconfig
    PATHS ${FONTCONFIG_LIBRARY_DIRS} /usr/lib /usr/local/lib
  )
endif(BUILD_OSX_BUNDLE)


# message(STATUS "fclib ${FONTCONFIG_LIBRARY}")
# message(STATUS "fcinclude ${FONTCONFIG_INCLUDE_DIR}")


if (FONTCONFIG_LIBRARY)
  if (FONTCONFIG_INCLUDE_DIR)
    set( FONTCONFIG_FOUND "YES" )
    set( FONTCONFIG_LIBRARIES ${FONTCONFIG_LIBRARY} )
	find_program(FONTCONFIG_CONFIG NAMES fontconfig-config PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin)
# 	exec_program(${FONTCONFIG_CONFIG} ARGS --libs OUTPUT_VARIABLE FONTCONFIG_LIBS)
# 	exec_program(${FONTCONFIG_CONFIG} ARGS --cflags OUTPUT_VARIABLE FONTCONFIG_CFLAGS)
#  	message(STATUS ${FONTCONFIG_LIBS})
#  	message(STATUS ${FONTCONFIG_CFLAGS})
  endif (FONTCONFIG_INCLUDE_DIR)
endif (FONTCONFIG_LIBRARY)
