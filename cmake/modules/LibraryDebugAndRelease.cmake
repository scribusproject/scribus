#
# This macro is used when we may have a debug and or release build of a library,
# and we want to produce a single easy to use library string that'll do the right
# thing. If both debug and release versions are available, we'll automatically use the
# debug version for debug builds and the release version for release builds.
#
# If only one build exists, we use that one irrespective of build type.
#
MACRO(SET_LIBRARY_FROM_DEBUG_AND_RELEASE _NAME)

  IF(NOT DEFINED "${_NAME}_LIBRARY_RELEASE" OR NOT DEFINED "${_NAME}_LIBRARY_DEBUG")
    MESSAGE(FATAL_ERROR "${_NAME}_LIBRARY_DEBUG OR ${_NAME}_LIBRARY_RELEASE undefined")
  ENDIF(NOT DEFINED "${_NAME}_LIBRARY_RELEASE" OR NOT DEFINED "${_NAME}_LIBRARY_DEBUG")
  IF(${_NAME}_LIBRARY_RELEASE AND ${_NAME}_LIBRARY_DEBUG)
    SET(${_NAME}_LIBRARY "optimized;${${_NAME}_LIBRARY_RELEASE};debug;${${_NAME}_LIBRARY_DEBUG}")
  ELSE(${_NAME}_LIBRARY_RELEASE AND ${_NAME}_LIBRARY_DEBUG)
    IF(${_NAME}_LIBRARY_DEBUG)
      MESSAGE("WARNING: ${_NAME} debug library will be used even for release builds")
      SET(${_NAME}_LIBRARY ${${_NAME}_LIBRARY_DEBUG})
    ELSE(${_NAME}_LIBRARY_DEBUG)
      SET(${_NAME}_LIBRARY ${${_NAME}_LIBRARY_RELEASE})
    ENDIF(${_NAME}_LIBRARY_DEBUG)
  ENDIF(${_NAME}_LIBRARY_RELEASE AND ${_NAME}_LIBRARY_DEBUG)

ENDMACRO(SET_LIBRARY_FROM_DEBUG_AND_RELEASE)
