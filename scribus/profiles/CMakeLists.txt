INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
Fogra27L.icm
Readme
sRGB.icm
srgb.license
tr01_d50.icm
  DESTINATION ${LIBDIR}profiles
)
