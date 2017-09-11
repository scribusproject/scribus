if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
	include(UsePkgConfig)
	PKGCONFIG(poppler _libPopplerIncDir _libPopplerLinkDir _libPopplerLinkFlags _libPopplerCflags)
	set(PKG_POPPLER_INCLUDE_DIRS ${_libPopplerIncDir})
	set(PKG_POPPLER_LIBRARIES ${_libPopplerLinkDir})
else (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
	include(FindPkgConfig)
	pkg_search_module(POPPLER libpoppler>=0.19.0 poppler>=0.19.0)
	if (POPPLER_FOUND)
		pkg_search_module(POPPLER_CPP REQUIRED libpoppler-cpp>=0.19.0 poppler-cpp>=0.19.0)
	endif(POPPLER_FOUND)
endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
 
find_path(POPPLER_INCLUDE_DIR 
  NAMES poppler-config.h
  PATHS ${PKG_POPPLER_INCLUDE_DIRS} ${POPPLER_INCLUDE_DIRS} /usr/local/include /usr/include
  PATH_SUFFIXES poppler
  NO_DEFAULT_PATH
)

find_path(POPPLER_CPP_INCLUDE_DIR 
  NAMES poppler-version.h
  PATHS ${PKG_POPPLER_CPP_INCLUDE_DIRS} ${POPPLER_CPP_INCLUDE_DIRS} /usr/local/include /usr/include
  PATH_SUFFIXES poppler
  NO_DEFAULT_PATH
)

find_library(POPPLER_LIBRARY
  NAMES libpoppler poppler
  PATHS ${PKG_POPPLER_LIBRARIES} ${POPPLER_LIBRARY_DIRS} /usr/local/lib /usr/lib /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
  PATH_SUFFIXES poppler
  NO_DEFAULT_PATH
)

find_library(POPPLER_CPP_LIBRARY
  NAMES libpoppler-cpp poppler-cpp
  PATHS ${PKG_POPPLER_CPP_LIBRARIES} ${POPPLER_CPP_LIBRARY_DIRS} /usr/local/lib /usr/lib /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
  PATH_SUFFIXES poppler
  NO_DEFAULT_PATH
)

if (POPPLER_LIBRARY)
  if (POPPLER_INCLUDE_DIR AND POPPLER_CPP_INCLUDE_DIR)
	set( FOUND_POPPLER 1 )
    set( POPPLER_LIBRARIES ${POPPLER_LIBRARY} ${POPPLER_CPP_LIBRARY} )
    set( POPPLER_INCLUDES ${POPPLER_INCLUDE_DIR} ${POPPLER_CPP_INCLUDE_DIR} )
  endif (POPPLER_INCLUDE_DIR AND POPPLER_CPP_INCLUDE_DIR)
endif (POPPLER_LIBRARY)
