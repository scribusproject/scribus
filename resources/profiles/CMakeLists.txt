include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
GenericCMYK.icm
GenericCMYK.txt
Readme
sRGB.icm
srgb.license
  DESTINATION ${SHAREDIR}profiles
)
