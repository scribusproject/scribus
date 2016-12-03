include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${FREETYPE_INCLUDE_DIRS}
	${CAIRO_INCLUDE_DIRS}
	${HARFBUZZ_INCLUDE_DIRS}
	${ICU_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_TEXT_MOC_CLASSES
	storytext.h
	boxes.h
)

set(SCRIBUS_TEXT_LIB_SOURCES
	boxes.cpp
	frect.cpp
	fsize.cpp
	glyphcluster.cpp
	index.cpp
	screenpainter.cpp
	scrptrun.cpp
	sctext_shared.cpp
	shapedtext.cpp
	shapedtextcache.cpp
	shapedtextfeed.cpp
	specialchars.cpp
	storytext.cpp
	textlayout.cpp
	textlayoutpainter.cpp
	textshaper.cpp
	textcontext.cpp
)

QT5_WRAP_CPP(SCRIBUS_TEXT_MOC_SOURCES ${SCRIBUS_TEXT_MOC_CLASSES})

set(SCRIBUS_TEXT_LIB "scribus_text_lib")
add_library(${SCRIBUS_TEXT_LIB} STATIC ${SCRIBUS_TEXT_LIB_SOURCES} ${SCRIBUS_TEXT_MOC_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_TEXT_LIB}
	PROPERTIES
	COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
)

