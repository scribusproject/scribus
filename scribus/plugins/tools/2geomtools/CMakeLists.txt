INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/scribus
)

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
  MESSAGE("Boost Library Found OK")
ENDIF (Boost_FOUND)

# FIND_PATH(BOOST_INCLUDE_DIR boost/weak_ptr.hpp 
#                             /usr/include 
#                             /usr/local/include
#                             $ENV{BOOST_DIR} )
# 
# 
# IF (BOOST_INCLUDE_DIR)
#   SET(Boost_FOUND TRUE)
# ENDIF (BOOST_INCLUDE_DIR)

IF(Boost_FOUND)
  MESSAGE(STATUS "Boost: FOUND, building 2geomtools")
  ADD_SUBDIRECTORY (lib2geom)
  SET(GEOMLIB ${CMAKE_CURRENT_BINARY_DIR}/lib2geom/2geom)
  LINK_DIRECTORIES(${PLUGIN_LIBRARIES} ${GEOMLIB})
  ADD_SUBDIRECTORY (meshdistortion)
  ADD_SUBDIRECTORY (pathalongpath)
  INCLUDE_DIRECTORIES( ${BOOST_INCLUDE_DIR} )
ELSE(Boost_FOUND)
  MESSAGE(STATUS "Boost: NOT FOUND, not building 2geomtools")
ENDIF(Boost_FOUND)

