INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

SET(SCRIBUS_PGF_LIB_SOURCES
        Decoder.cpp
        Encoder.cpp
        PGFimage.cpp
        Stream.cpp
        Subband.cpp
        WaveletTransform.cpp
       )


SET(SCRIBUS_PGF_LIB "scribus_pgf_lib")
ADD_LIBRARY(${SCRIBUS_PGF_LIB} STATIC ${SCRIBUS_PGF_LIB_SOURCES})
SET_TARGET_PROPERTIES(${SCRIBUS_PGF_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
