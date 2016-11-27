include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/scribus
)

include_directories( ${Boost_INCLUDE_DIR} )
add_subdirectory (meshdistortion)
add_subdirectory (pathalongpath)
