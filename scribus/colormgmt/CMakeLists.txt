include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

include_directories( ${LCMS2_INCLUDE_DIR} )
set(SCRIBUS_LCMS_IMPL_SOURCES
	sclcms2colormgmtengineimpl.cpp
	sclcms2colorprofileimpl.cpp
	sclcms2colortransformimpl.cpp
)

set(SCRIBUS_COLORMGMT_LIB_SOURCES
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

set(SCRIBUS_COLORMGMT_LIB "scribus_colormgmt_lib")
add_library(${SCRIBUS_COLORMGMT_LIB} STATIC ${SCRIBUS_COLORMGMT_LIB_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_COLORMGMT_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
