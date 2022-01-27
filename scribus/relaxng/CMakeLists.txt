include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
)


include("generatedfiles.cmake")

# if(QT4_WRAP_CPP)
  QT4_WRAP_CPP(SCRIBUS_RELAXNG_MOC_SOURCES ${SCRIBUS_RELAXNG_HEADERS})
# endif()



set(SCRIBUS_RELAXNG_LIB "scribus_relaxng_lib")
add_library(${SCRIBUS_RELAXNG_LIB} STATIC ${SCRIBUS_RELAXNG_SOURCES} ${SCRIBUS_RELAXNG_MOC_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_RELAXNG_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )

