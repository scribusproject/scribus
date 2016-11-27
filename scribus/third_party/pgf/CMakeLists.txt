include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_PGF_LIB_SOURCES
        Decoder.cpp
        Encoder.cpp
        PGFimage.cpp
        PGFstream.cpp
        Subband.cpp
        WaveletTransform.cpp
       )


set(SCRIBUS_PGF_LIB "scribus_pgf_lib")
add_library(${SCRIBUS_PGF_LIB} STATIC ${SCRIBUS_PGF_LIB_SOURCES})
set_target_properties(${SCRIBUS_PGF_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
