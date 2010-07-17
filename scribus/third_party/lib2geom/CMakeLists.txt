INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

SET(2GEOM_VERSION 0.1.0)

# We need to pass -fPIC only to lib2geom on amd64 - avoid it elsewhere see: http://www.gentoo.org/proj/en/base/amd64/howtos/index.xml?part=1&chap=3
IF (ARCH_X86_64 EQUAL 1)
  SET(CMAKE_CXX_FLAGS "$(CMAKE_CXX_FLAGS) -fPIC")
ENDIF (ARCH_X86_64 EQUAL 1)

OPTION(2GEOM_BUILD_SHARED
  "Build lib2geom and libtoy as shared libraries."
  OFF)
SET(LIB_TYPE STATIC)

SET(2GEOM_SRC
svg-path.h
svg-path.cpp
#svg-path-parser.h
#svg-path-parser.cpp
ord.h
nearest-point.cpp
nearest-point.h
bezier-clipping.cpp
bezier-curve.cpp
bezier-curve.h
circle.cpp
circle.h
conicsec.h
conicsec.cpp
conic_section_clipper.cpp
conic_section_clipper.h
conic_section_clipper_cr.h
conic_section_clipper_impl.h
curve.h
curve.cpp
curves.h
ellipse.cpp
ellipse.h
elliptical-arc.cpp
elliptical-arc.h
hvlinesegment.h
line.cpp
line.h
ray.h
sbasis-curve.h
path.cpp
path.h
path-intersection.cpp
path-intersection.h
pathvector.cpp
pathvector.h
forward.h
shape.cpp
shape.h
region.cpp
region.h
crossing.h
crossing.cpp
sweep.cpp
sweep.h
toposweep.cpp
toposweep.h
poly.cpp
poly.h
quadtree.cpp
quadtree.h
redblacktree.cpp
redblacktree.h
rtree.cpp
rtree.h
affine.cpp
affine.h
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
sbasis-of.h
sbasis.cpp
sbasis.h
sbasis-2d.h
sbasis-2d.cpp
sbasisN.h
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
recursive-bezier-intersection.cpp
intersection-by-smashing.h
intersection-by-smashing.cpp
geom.cpp
geom.h
math-utils.h
utils.cpp
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
svg-elliptical-arc.cpp
svg-elliptical-arc.h
numeric/matrix.cpp
scribushelper.h
scribushelper.cpp
)

SET(SCRIBUS_2GEOM_LIB "scribus_2geom_lib")
ADD_LIBRARY(${SCRIBUS_2GEOM_LIB} STATIC ${2GEOM_SRC})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_2GEOM_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
