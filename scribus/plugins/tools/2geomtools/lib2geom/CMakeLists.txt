PROJECT(lib2geom CXX)
SET(2GEOM_VERSION 0.1.0)
#CB SET(CMAKE_BACKWARDS_COMPATIBILITY 2.4)
#CB SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

#CB SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
#CB SET(CMAKE_SKIP_RPATH:BOOL OFF)
#CB SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)
# INCLUDE(FindBoost)
#CB INCLUDE (CheckIncludeFiles)


# Set Compiler Flags
# SET(CMAKE_CXX_FLAGS "${GTK2_CFLAGS} ${cairo_CFLAGS} -Wall")
#CB SET(CMAKE_CXX_FLAGS "-Wall")
#CB SET(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM}")
# We need to pass -fPIC only to lib2geom on amd64 - avoid it elsewhere see: http://www.gentoo.org/proj/en/base/amd64/howtos/index.xml?part=1&chap=3

STRING(REGEX MATCH "(x86_64-*)|(X86_64-*)|(AMD64-*)|(amd64-*)" _machine_x86_64 "${MACHINE}")
IF (_machine_x86_64)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
ENDIF (_machine_x86_64)

OPTION(2GEOM_BUILD_SHARED
  "Build lib2geom and libtoy as shared libraries."
  OFF)
SET(LIB_TYPE STATIC)

SET(2GEOM_SRC
svg-path.h
svg-path.cpp
ord.h
path.cpp
path.h
path-intersection.cpp
path-intersection.h
svg-elliptical-arc.h
shape.cpp
shape.h
region.cpp
region.h
crossing.h
crossing.cpp
sweep.cpp
sweep.h
poly.cpp
poly.h
poly-dk-solve.cpp
poly-dk-solve.h
poly-laguerre-solve.cpp
poly-laguerre-solve.h
quadtree.cpp
quadtree.h
matrix.cpp
matrix.h
transforms.cpp
transforms.h
point.h
point.cpp
point-l.h
coord.h
d2.h
d2-sbasis.h
d2-sbasis.cpp
rect.h
piecewise.h
piecewise.cpp
sbasis.cpp
sbasis.h
sbasis-2d.h
sbasis-2d.cpp
sbasis-geometric.cpp
sbasis-geometric.h
sbasis-math.h
sbasis-math.cpp
sbasis-poly.cpp
sbasis-poly.h
sbasis-roots.cpp
sbasis-to-bezier.cpp
sbasis-to-bezier.h
bezier-to-sbasis.h
basic-intersection.h
basic-intersection.cpp
geom.cpp
geom.h
utils.h
exception.h
angle.h
bezier-utils.cpp
bezier-utils.h
choose.h
circulator.h
conjugate_gradient.cpp
conjugate_gradient.h
convex-cover.cpp
convex-cover.h
solve-bezier-one-d.cpp
solve-bezier-parametric.cpp
solver.h
sturm.h
scribushelper.h
scribushelper.cpp
)

# make lib for 2geom
ADD_LIBRARY(2geom ${LIB_TYPE} ${2GEOM_SRC})
TARGET_LINK_LIBRARIES(2geom)
#TARGET_LINK_LIBRARIES(2geom " ${GTK2_LINK_FLAGS}")
#INSTALL(TARGETS 2geom
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib
#)
#FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#INSTALL(FILES ${files} DESTINATION include/2geom/2geom)

#CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/2geom.pc.in
#                ${CMAKE_BINARY_DIR}/2geom.pc @ONLY IMMEDIATE )
#INSTALL(FILES "${CMAKE_BINARY_DIR}/2geom.pc" DESTINATION lib/pkgconfig)
#
