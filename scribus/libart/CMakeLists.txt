IF(RENDER_LIB STREQUAL "LIBART")
	INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	)
	
	SET(SCRIBUS_LIBART_LIB_SOURCES
	art_kmisc.c
	art_render_misc.c
	art_render_pattern.c
	art_rgba_affine.c
	art_rgb_affine_private.c
	art_rgb.c
	art_rgb_svp.c
	)
	
	SET(SCRIBUS_LIBART_LIB "scribus_libart_lib")
	ADD_LIBRARY(${SCRIBUS_LIBART_LIB} STATIC ${SCRIBUS_LIBART_LIB_SOURCES})
ENDIF(RENDER_LIB STREQUAL "LIBART")