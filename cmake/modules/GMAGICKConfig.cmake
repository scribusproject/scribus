#
# Find the native GraphicksMagick++ includes and library
#

# This module defines
# GMAGICK_INCLUDE_DIR, where to find Magick++.h
# GMAGICK_LIBRARY, the libraries to link against to use GraphicksMagick++.
# GMAGICK_FOUND, If false, do not try to use GraphicksMagick++.
# GMAGICK_LIBS, link information
# GMAGICK_CFLAGS, cflags for include information


find_program(GMAGICK_CONFIG_EXECUTABLE NAMES GraphicsMagick++-config PATH ${PREFIX_GMAGICK} DOC "GraphicsMagick++-config executable")
mark_as_advanced(GMAGICK_CONFIG_EXECUTABLE)

MESSAGE(STATUS "Gmconf ${GMAGICK_CONFIG_EXECUTABLE}")

macro(_gmagickconfig_invoke _type _varname _regexp)
  set(_gmagickconfig_invoke_result)

  execute_process(
    COMMAND ${GMAGICK_CONFIG_EXECUTABLE} ${_type}
    OUTPUT_VARIABLE _gmagickconfig_invoke_result
    RESULT_VARIABLE _gmagickconfig_failed)

  if (NOT _gmagickconfig_failed)
    set(GMAGICK_FOUND "YES")
    string(REGEX REPLACE "[\r\n]"  " " _gmagickconfig_invoke_result "${_gmagickconfig_invoke_result}")
    string(REGEX REPLACE " +$"     ""  _gmagickconfig_invoke_result "${_gmagickconfig_invoke_result}")

    if (NOT ${_regexp} STREQUAL "")
      string(REGEX REPLACE "${_regexp}" " " _gmagickconfig_invoke_result "${_gmagickconfig_invoke_result}")
    endif(NOT ${_regexp} STREQUAL "")

    separate_arguments(_gmagickconfig_invoke_result)
    
    MESSAGE(STATUS "GMAGICK result for ${_type}: ${_gmagickconfig_invoke_result}")

    #message(STATUS "  ${_varname} ... ${_pkgconfig_invoke_result}")
     set(${_varname} ${_gmagickconfig_invoke_result})
#     _pkgconfig_set(${_prefix}_${_varname} "${_pkgconfig_invoke_result}")
    endif(NOT _gmagickconfig_failed)
endmacro(_gmagickconfig_invoke)

_gmagickconfig_invoke("--cppflags" GMAGICK_INCLUDES "(^| )-I")
_gmagickconfig_invoke("--ldflags" GMAGICK_LDFLAGS "(^| )-L")
_gmagickconfig_invoke("--libs" GMAGICK_LIBRARIES "(^| )-l")

if(GMAGICK_INCLUDES)
	SET(GMAGICK_FOUND "YES")
else(GMAGICK_INCLUDES)
	SET(GMAGICK_INCLUDES)
	SET(GMAGICK_LIBRARIES)
	SET(GMAGICK_LDFLAGS)
endif(GMAGICK_INCLUDES)
