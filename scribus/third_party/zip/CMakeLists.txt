include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${ZLIB_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_ZIP_LIB_SOURCES
	unzip.cpp
	zip.cpp
	zipglobal.cpp
	scribus_zip.cpp
)

set(SCRIBUS_ZIP_LIB "scribus_zip_lib")
add_library(${SCRIBUS_ZIP_LIB} STATIC ${SCRIBUS_ZIP_LIB_SOURCES})
set_target_properties(${SCRIBUS_ZIP_LIB}
	PROPERTIES
	COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
	)
