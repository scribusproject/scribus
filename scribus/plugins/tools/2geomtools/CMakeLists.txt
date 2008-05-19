INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

FIND_PATH(BOOST_INCLUDE_DIR boost/weak_ptr.hpp 
                            /usr/include 
                            /usr/local/include
                            $ENV{BOOST_DIR} )


IF (BOOST_INCLUDE_DIR)
  SET(BOOST_FOUND TRUE)
ENDIF (BOOST_INCLUDE_DIR)


INCLUDE (CheckIncludeFiles)

IF(BOOST_FOUND)
	MESSAGE(STATUS "boost: FOUND building 2geomtools")
	ADD_SUBDIRECTORY (lib2geom)
	SET(GEOMLIB ${CMAKE_CURRENT_BINARY_DIR}/lib2geom/2geom)
	LINK_DIRECTORIES(${PLUGIN_LIBRARIES} ${GEOMLIB})
	ADD_SUBDIRECTORY (meshdistortion)
	ADD_SUBDIRECTORY (pathalongpath)
INCLUDE_DIRECTORIES( ${BOOST_INCLUDE_DIR} )
ELSE(BOOST_FOUND)
     MESSAGE(STATUS "boost: NOT FOUND not building 2geomtools")
ENDIF(BOOST_FOUND)
