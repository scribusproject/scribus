# - Try to find GraphicsMagick
# Once done, this will define
#
#  GMAGICK_FOUND - system has zziplib
#  GMAGICK_INCLUDE_DIRS - the zziplib include directories
#  GMAGICK_LIBRARIES - link these to use zziplib

# Note: Detecting GraphicsMagick is hard, as all header files have the same names as the ImageMagick headers. But they are not compatible.

include(FindPkgConfig)

# Use pkg-config to get hints about paths
pkg_check_modules(GMAGICK_PKGCONF GraphicsMagick)

# Include dir
find_path(GMAGICK_INCLUDE_DIR
  NAMES GraphicsMagick/magick/api.h
  PATHS ${GMAGICK_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(GMAGICK_LIBRARY
  NAMES GraphicsMagick 
  PATHS ${GMAGICK_PKGCONF_LIBRARY_DIRS}
)

MESSAGE("GRAPHICS_MAGIC debug: pkgconf: ${GMAGICK_PKGCONF_INCLUDE_DIRS},${GMAGICK_PKGCONF_LIBRARY_DIRS} include:${GMAGICK_INCLUDE_DIR} lib:${GMAGICK_LIBRARY}")
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMAGICK DEFAULT_MSG GMAGICK_LIBRARY GMAGICK_INCLUDE_DIR)
IF(GMAGICK_FOUND)
    set(GMAGICK_INCLUDES ${GMAGICK_INCLUDE_DIR}/GraphicsMagick)
    set(GMAGICK_LIBRARIES ${GMAGICK_LIBRARY})
ELSE()
    set(GMAGICK_INCLUDES)
    set(GMAGICK_LIBRARIES)
ENDIF()
