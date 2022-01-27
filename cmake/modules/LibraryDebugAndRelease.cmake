#
# This macro is used when we may have a debug and or release build of a library,
# and we want to produce a single easy to use library string that'll do the right
# thing. If both debug and release versions are available, we'll automatically use the
# debug version for debug builds and the release version for release builds.
#
# If only one build exists, we use that one irrespective of build type.
#
macro(SET_LIBRARY_FROM_DEBUG_AND_RELEASE _NAME)

	if(NOT DEFINED "${_NAME}_LIBRARY_RELEASE" OR NOT DEFINED "${_NAME}_LIBRARY_DEBUG")
		message(FATAL_ERROR "${_NAME}_LIBRARY_DEBUG OR ${_NAME}_LIBRARY_RELEASE undefined")
	endif(NOT DEFINED "${_NAME}_LIBRARY_RELEASE" OR NOT DEFINED "${_NAME}_LIBRARY_DEBUG")
	if(${_NAME}_LIBRARY_RELEASE AND ${_NAME}_LIBRARY_DEBUG)
		set(${_NAME}_LIBRARY "optimized;${${_NAME}_LIBRARY_RELEASE};debug;${${_NAME}_LIBRARY_DEBUG}")
	else(${_NAME}_LIBRARY_RELEASE AND ${_NAME}_LIBRARY_DEBUG)
		if(${_NAME}_LIBRARY_DEBUG)
			message("WARNING: ${_NAME} debug library will be used even for release builds")
			set(${_NAME}_LIBRARY ${${_NAME}_LIBRARY_DEBUG})
		else(${_NAME}_LIBRARY_DEBUG)
			set(${_NAME}_LIBRARY ${${_NAME}_LIBRARY_RELEASE})
		endif(${_NAME}_LIBRARY_DEBUG)
	endif(${_NAME}_LIBRARY_RELEASE AND ${_NAME}_LIBRARY_DEBUG)

endmacro(SET_LIBRARY_FROM_DEBUG_AND_RELEASE)
