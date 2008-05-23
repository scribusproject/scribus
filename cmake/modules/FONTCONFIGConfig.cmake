#
# Find the native FONTCONFIG includes and library
#

# This module defines
# FONTCONFIG_INCLUDE_DIR, where to find art*.h etc
# FONTCONFIG_LIBRARY, the libraries to link against to use FONTCONFIG.
# FONTCONFIG_FOUND, If false, do not try to use FONTCONFIG.
# LIBFONTCONFIG_LIBS, link information
# LIBFONTCONFIG_CFLAGS, cflags for include information

IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
  INCLUDE(UsePkgConfig)
  PKGCONFIG(fontconfig _fontconfigIncDir _fontconfigLinkDir _fontconfigLinkFlags _fontconfigCflags)
  SET(FONTCONFIG_LIBS ${_fontconfigCflags})
ELSE (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
  INCLUDE(FindPkgConfig)
  pkg_search_module(FONTCONFIG REQUIRED fontconfig)
ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)

#INCLUDE(UsePkgConfig)

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#PKGCONFIG(fontconfig _fontconfigIncDir _fontconfigLinkDir _fontconfigLinkFlags _fontconfigCflags)

#SET(FONTCONFIG_LIBS ${_fontconfigCflags})

IF(BUILD_OSX_BUNDLE)
  FIND_PATH(FONTCONFIG_INCLUDE_DIR
    NAMES fontconfig/fontconfig.h
    PATHS ${FONTCONFIG_INCLUDE_DIRS} /opt/local/include
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(FONTCONFIG_LIBRARY 
    NAMES fontconfig
    PATHS ${FONTCONFIG_LIBRARY_DIRS} /opt/local/lib
    NO_DEFAULT_PATH
  )
ELSE(BUILD_OSX_BUNDLE)
  FIND_PATH(FONTCONFIG_INCLUDE_DIR 
    NAMES fontconfig/fontconfig.h
	PATHS ${FONTCONFIG_INCLUDE_DIRS} 
    ${_fontconfigIncDir}
    /usr/include
    /usr/local/include
    PATH_SUFFIXES fontconfig
  )
  # quick hack as the above finds it nicely but our source includes the libart_lgpl text at the moment
  #STRING(REGEX REPLACE "/libart_lgpl" "" FONTCONFIG_INCLUDE_DIR ${FONTCONFIG_INCLUDE_DIR})
  FIND_LIBRARY(FONTCONFIG_LIBRARY NAMES fontconfig
    PATHS ${FONTCONFIG_LIBRARY_DIRS} /usr/lib /usr/local/lib
  )
ENDIF(BUILD_OSX_BUNDLE)


# MESSAGE(STATUS "fclib ${FONTCONFIG_LIBRARY}")
# MESSAGE(STATUS "fcinclude ${FONTCONFIG_INCLUDE_DIR}")


IF (FONTCONFIG_LIBRARY)
  IF (FONTCONFIG_INCLUDE_DIR)
    SET( FONTCONFIG_FOUND "YES" )
    SET( FONTCONFIG_LIBRARIES ${FONTCONFIG_LIBRARY} )
	FIND_PROGRAM(FONTCONFIG_CONFIG NAMES fontconfig-config PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin)
# 	EXEC_PROGRAM(${FONTCONFIG_CONFIG} ARGS --libs OUTPUT_VARIABLE FONTCONFIG_LIBS)
# 	EXEC_PROGRAM(${FONTCONFIG_CONFIG} ARGS --cflags OUTPUT_VARIABLE FONTCONFIG_CFLAGS)
#  	MESSAGE(STATUS ${FONTCONFIG_LIBS})
#  	MESSAGE(STATUS ${FONTCONFIG_CFLAGS})
  ENDIF (FONTCONFIG_INCLUDE_DIR)
ENDIF (FONTCONFIG_LIBRARY)
