INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${LCMS_INCLUDE_DIR}
)

IF(HAVE_LCMS21)
	SET(SCRIBUS_LCMS_IMPL_SOURCES
		sclcms2colormgmtengineimpl.cpp
		sclcms2colorprofileimpl.cpp
		sclcms2colortransformimpl.cpp
	)
ELSE(HAVE_LCMS21)
	SET(SCRIBUS_LCMS_IMPL_SOURCES
		sclcmscolormgmtengineimpl.cpp
		sclcmscolorprofileimpl.cpp
		sclcmscolortransformimpl.cpp
	)
ENDIF(HAVE_LCMS21)

SET(SCRIBUS_COLORMGMT_LIB_SOURCES
	sccolormgmtengine.cpp
	sccolormgmtstructs.cpp
	sccolorprofile.cpp
	sccolorprofilecache.cpp
	sccolorprofiledata.cpp
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
