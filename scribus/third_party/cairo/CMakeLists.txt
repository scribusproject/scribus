pkg_check_modules(PIXMAN   pixman-1>=0.15.16)

IF (NOT PIXMAN_FOUND)
   MESSAGE(FATAL_ERROR "Pixman >= 0.15.16 is required")
ENDIF (NOT PIXMAN_FOUND)

INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
${FONTCONFIG_INCLUDE_DIR}
${PIXMAN_INCLUDE_DIRS}
)
# We need to pass -fPIC only to lib2geom on amd64 - avoid it elsewhere see: http://www.gentoo.org/proj/en/base/amd64/howtos/index.xml?part=1&chap=3
IF (ARCH_X86_64 EQUAL 1)
  SET(CMAKE_CXX_FLAGS "$(CMAKE_CXX_FLAGS) -fPIC")
ENDIF (ARCH_X86_64 EQUAL 1)

SET(SCRIBUS_CAIRO_LIB_SOURCES
cairo-analysis-surface.c
cairo-arc.c
cairo-array.c
cairo-atomic.c
cairo-bentley-ottmann.c
cairo-bentley-ottmann-rectangular.c
cairo-bentley-ottmann-rectilinear.c
cairo.c
cairo-cache.c
cairo-clip.c
cairo-color.c
cairo-debug.c
cairo-fixed.c
cairo-font-face.c
cairo-font-face-twin.c
cairo-font-face-twin-data.c
cairo-font-options.c
cairo-freelist.c
cairo-ft-font.c
cairo-gstate.c
cairo-hash.c
cairo-hull.c
cairo-image-info.c
cairo-image-surface.c
cairo-matrix.c
cairo-mesh-gradient-rasterizer.c
cairo-meta-surface.c
cairo-misc.c
cairo-mutex.c
cairo-output-stream.c
cairo-paginated-surface.c
cairo-path-bounds.c
cairo-path.c
cairo-path-fill.c
cairo-path-fixed.c
cairo-path-in-fill.c
cairo-path-stroke.c
cairo-pattern.c
cairo-pen.c
cairo-png.c
cairo-polygon.c
cairo-rectangle.c
cairo-region.c
cairo-rtree.c
cairo-scaled-font.c
cairo-slope.c
cairo-spans.c
cairo-spline.c
cairo-stroke-style.c
cairo-surface.c
cairo-surface-clipper.c
cairo-surface-fallback.c
cairo-surface-wrapper.c
cairo-system.c
cairo-tee-surface.c
cairo-tor-scan-converter.c
cairo-toy-font-face.c
cairo-traps.c
cairo-unicode.c
cairo-user-font.c
cairo-version.c
cairo-wideint.c
)

IF (WIN32)
  SET(CAIRO_WIN32_ONLY_SOURCES
      cairo-win32-font.c
      cairo-win32-printing-surface.c
	  cairo-win32-surface.c
      cairo-scaled-font-subsets.c
	  cairo-truetype-subset.c
     )
ELSE(WIN32)
  SET(CAIRO_WIN32_ONLY_SOURCES)
ENDIF(WIN32)


ADD_DEFINITIONS(-DHAVE_STDINT_H  -DHAVE_UINT64_T=1 -DCAIRO_NO_MUTEX=1)

SET(SCRIBUS_CAIRO_LIB "scribus_cairo_lib")
ADD_LIBRARY(${SCRIBUS_CAIRO_LIB} STATIC ${SCRIBUS_CAIRO_LIB_SOURCES} ${CAIRO_WIN32_ONLY_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_CAIRO_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )

