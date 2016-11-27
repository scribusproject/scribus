##############################################################################################################
########## Report on installed dependencies and offered features                                    ##########

message("")
message("--------------------------------------------------")
message("-- Configuration Summary")
message("--------------------------------------------------")

if (HAVE_LIBZ)
	message(STATUS "libz found: Compression support enabled")
else (HAVE_LIBZ)
	message(STATUS "libz not found: Compression support disabled")
endif (HAVE_LIBZ)

if (HAVE_XML)
	message(STATUS "libxml2 found: XML support enabled")
else (HAVE_XML)
	message(STATUS "libxml2 not found: XML support disabled")
endif (HAVE_XML)

if (HAVE_PYTHON)
	message(STATUS "Python found: Scripting support enabled")
else (HAVE_PYTHON)
	message(STATUS "Python not found: Scripting support disabled")
endif (HAVE_PYTHON)

if (CAIRO_FOUND)
	message(STATUS "cairo found ok")
else (CAIRO_FOUND)
	message(STATUS "cairo not found")
endif (CAIRO_FOUND)

if (HAVE_CUPS)
	message(STATUS "cups found: Printing support enabled")
else (HAVE_CUPS)
	message(STATUS "cups not found: Print support disabled")
endif (HAVE_CUPS)

if (HAVE_LCMS2)
	message(STATUS "littleCMS2 found: Color management support enabled")
else (HAVE_LCMS2)
	message(STATUS "littleCMS2 not found: Color management support disabled")
endif (HAVE_LCMS2)

if (HAVE_HUNSPELL)
	message(STATUS "Hunspell found: Spell correction support enabled")
else (HAVE_HUNSPELL)
	message(STATUS "Hunspell not found: Spell correction support disabled")
endif (HAVE_HUNSPELL)

if (HAVE_PODOFO)
	message(STATUS "podofo found: PDF import support enabled")
else (HAVE_PODOFO)
	message(STATUS "podofo not found: PDF import support disabled")
endif (HAVE_PODOFO)

if (HAVE_POPPLER)
	message(STATUS "poppler found: PDF import support enabled")
else (HAVE_POPPLER)
	message(STATUS "poppler not found: PDF import support disabled")
endif (HAVE_POPPLER)

if (HAVE_BOOST)
	message(STATUS "boost found: Extra geometric tools support enabled")
else (HAVE_BOOST)
	message(STATUS "boost not found: Extra geometric tools support disabled")
endif (HAVE_BOOST)

message("--")
message("-- Image formats:")

if (JPEG_FOUND)
	message(STATUS "libjpeg found: JPEG support enabled")
else (JPEG_FOUND)
	message(STATUS "libjpeg not found: JPEG support disabled")
endif (JPEG_FOUND)

if (HAVE_TIFF)
	message(STATUS "libtiff found: TIFF support enabled")
else (HAVE_TIFF)
	message(STATUS "libtiff not found: TIFF support disabled")
endif (HAVE_TIFF)

if (HAVE_OSG)
	message(STATUS "osg found: Support for for OSG 3D Extension be supported")
else (HAVE_OSG)
	message(STATUS "osg not found: Support for OSG 3D Extension not be supported")
endif (HAVE_OSG)

if (HAVE_REVENGE)
	message(STATUS "librevenge found: Support for proprietary file import will be supported")
else (HAVE_REVENGE)
	message(STATUS "librevenge not found: Support for proprietary file import will not be supported")
endif (HAVE_REVENGE)

if (HAVE_LIBVISIO)
	message(STATUS "libvisio found: Support for Microsoft Visio files will be supported")
else (HAVE_LIBVISIO)
	message(STATUS "libvisio not found: Support for Microsoft Visio files will not be supported")
endif (HAVE_LIBVISIO)

if (LIBCDR_FOUND)
	message(STATUS "libcdr found: Support for CorelDraw files will be supported")
else (LIBCDR_FOUND)
	message(STATUS "libcdr not found: Support for CorelDraw files will not be supported")
endif (LIBCDR_FOUND)

if (LIBVISIO_FOUND)
	message(STATUS "libvisio found: Support for Microsoft Visio files will be supported")
else (LIBVISIO_FOUND)
	message(STATUS "libvisio not found: Support for Microsoft Visio files will not be supported")
endif (LIBVISIO_FOUND)

if (LIBMSPUB_FOUND)
	message(STATUS "libmspub found: Support for Microsoft Publisher files will be supported")
else (LIBMSPUB_FOUND)
	message(STATUS "libmspub not found: Support for Microsoft Publisher files will not be supported")
endif (LIBMSPUB_FOUND)

if (LIBPAGEMAKER_FOUND)
	message(STATUS "libpagemaker found: Support for PageMaker files will be supported")
else (LIBPAGEMAKER_FOUND)
	message(STATUS "libpagemaker not found: Support for PageMaker files will not be supported")
endif (LIBPAGEMAKER_FOUND)

if (GMAGICK_FOUND)
	message(STATUS "graphicsmagick found: Support for extra graphics formats will be supported")
else (GMAGICK_FOUND)
	message(STATUS "graphicsmagick not found: Support for extra graphics formats files will not be supported")
endif (GMAGICK_FOUND)

message("--------------------------------------------------")
message("")

