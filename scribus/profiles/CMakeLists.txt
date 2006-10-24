INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
GenericCMYK.icm
GenericCMYK.txt
Readme
sRGB.icm
srgb.license
  DESTINATION ${LIBDIR}profiles
)
