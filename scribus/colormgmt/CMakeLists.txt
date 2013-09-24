INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

INCLUDE_DIRECTORIES( ${LCMS2_INCLUDE_DIR} )
SET(SCRIBUS_LCMS_IMPL_SOURCES
	sclcms2colormgmtengineimpl.cpp
	sclcms2colorprofileimpl.cpp
	sclcms2colortransformimpl.cpp
)

SET(SCRIBUS_COLORMGMT_LIB_SOURCES
	sccolormgmtengine.cpp
	sccolormgmtenginedata.cpp
	sccolormgmtenginefactory.cpp
	sccolormgmtimplelem.cpp
	sccolormgmtstructs.cpp
	sccolorprofile.cpp
	sccolorprofilecache.cpp
	sccolorprofiledata.cpp
	sccolorspace.cpp
	sccolorspacedata.cpp
	sccolortransform.cpp
	sccolortransformpool.cpp
	${SCRIBUS_LCMS_IMPL_SOURCES}
)

SET(SCRIBUS_COLORMGMT_LIB "scribus_colormgmt_lib")
ADD_LIBRARY(${SCRIBUS_COLORMGMT_LIB} STATIC ${SCRIBUS_COLORMGMT_LIB_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_COLORMGMT_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
