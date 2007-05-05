INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
#Fogra27L.icm
Readme
sRGB.icm
srgb.license
GenericCMYK.icm
GenericCMYK.txt
  DESTINATION ${LIBDIR}profiles
)
