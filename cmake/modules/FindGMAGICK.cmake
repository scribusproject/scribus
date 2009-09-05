# - Try to find GraphicsMagick++
# Once done, this will define
#
#  GMAGICK_FOUND - system has zziplib
#  GMAGICK_INCLUDE_DIRS - the zziplib include directories
#  GMAGICK_LIBRARIES - link these to use zziplib

include(FindPkgConfig)

# Use pkg-config to get hints about paths
pkg_check_modules(GMAGICK_PKGCONF GraphicsMagick++)

# Include dir
find_path(GMAGICK_INCLUDE_DIR
  NAMES Magick++.h
  PATHS ${GMAGICK_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(GMAGICK_LIBRARY
  NAMES GraphicsMagick++
  PATHS ${GMAGICK_PKGCONF_LIBRARY_DIRS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMAGICK DEFAULT_MSG GMAGICK_LIBRARY GMAGICK_INCLUDE_DIR)
IF(GMAGICK_FOUND)
    set(GMAGICK_INCLUDES ${GMAGICK_INCLUDE_DIR})
    set(GMAGICK_LIBRARIES ${GMAGICK_LIBRARY})
ELSE()
    set(GMAGICK_INCLUDES)
    set(GMAGICK_LIBRARIES)
ENDIF()
