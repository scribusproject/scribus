include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${FREETYPE_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_STYLES_MOC_CLASSES
)

set(SCRIBUS_STYLES_LIB_SOURCES
	style.cpp
	stylecontext.cpp
	stylecontextproxy.cpp
	linestyle.cpp
	charstyle.cpp
	paragraphstyle.cpp
	tablestyle.cpp
	cellstyle.cpp
)

QT5_WRAP_CPP(SCRIBUS_STYLES_MOC_SOURCES ${SCRIBUS_STYLES_MOC_CLASSES})

set(SCRIBUS_STYLES_LIB "scribus_styles_lib")
add_library(${SCRIBUS_STYLES_LIB} STATIC ${SCRIBUS_STYLES_LIB_SOURCES} ${SCRIBUS_STYLES_MOC_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_STYLES_LIB}
	PROPERTIES
	COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
)
