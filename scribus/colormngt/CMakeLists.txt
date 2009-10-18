INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${LCMS_INCLUDE_DIR}
)

SET(SCRIBUS_COLORMNGT_LIB_SOURCES
	sccolormngtengine.cpp
	sccolormngtenginefactory.cpp
	sccolormngtimplelem.cpp
	sccolormngtstructs.cpp
	sccolorprofile.cpp
	sccolorprofilecache.cpp
	sccolortransform.cpp
	sccolortransformpool.cpp
	sclcmscolormngtengineimpl.cpp
	sclcmscolorprofileimpl.cpp
	sclcmscolortransformimpl.cpp
)
SET(SCRIBUS_COLORMNGT_LIB "scribus_colormngt_lib")
ADD_LIBRARY(${SCRIBUS_COLORMNGT_LIB} STATIC ${SCRIBUS_COLORMNGT_LIB_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_COLORMNGT_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
