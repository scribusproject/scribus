##############################################################################################################
########## Report on installed dependencies and offered features                                    ##########

MESSAGE("")
MESSAGE("--------------------------------------------------")
MESSAGE("-- Configuration Summary")
MESSAGE("--------------------------------------------------")

if (HAVE_LIBZ)
	MESSAGE(STATUS "libz found: Compression support enabled")
else (HAVE_LIBZ)
	MESSAGE(STATUS "libz not found: Compression support disabled")
endif (HAVE_LIBZ)

if (HAVE_XML)
	MESSAGE(STATUS "libxml2 found: XML support enabled")
else (HAVE_XML)
	MESSAGE(STATUS "libxml2 not found: XML support disabled")
endif (HAVE_XML)

if (HAVE_PYTHON)
	MESSAGE(STATUS "Python found: Scripting support enabled")
else (HAVE_PYTHON)
	MESSAGE(STATUS "Python not found: Scripting support disabled")
endif (HAVE_PYTHON)

if (CAIRO_FOUND)
	MESSAGE(STATUS "cairo found ok")
else (CAIRO_FOUND)
	MESSAGE(STATUS "cairo not found")
endif (CAIRO_FOUND)

if (HAVE_CUPS)
	MESSAGE(STATUS "cups found: Printing support enabled")
else (HAVE_CUPS)
	MESSAGE(STATUS "cups not found: Print support disabled")
endif (HAVE_CUPS)

if (HAVE_LCMS2)
	MESSAGE(STATUS "littleCMS2 found: Color management support enabled")
else (HAVE_LCMS2)
	MESSAGE(STATUS "littleCMS2 not found: Color management support disabled")
endif (HAVE_LCMS2)

if (HAVE_HUNSPELL)
	MESSAGE(STATUS "Hunspell found: Spell correction support enabled")
else (HAVE_HUNSPELL)
	MESSAGE(STATUS "Hunspell not found: Spell correction support disabled")
endif (HAVE_HUNSPELL)

if (HAVE_PODOFO)
	MESSAGE(STATUS "podofo found: PDF import support enabled")
else (HAVE_PODOFO)
	MESSAGE(STATUS "podofo not found: PDF import support disabled")
endif (HAVE_PODOFO)

if (HAVE_POPPLER)
	MESSAGE(STATUS "poppler found: PDF import support enabled")
else (HAVE_POPPLER)
	MESSAGE(STATUS "poppler not found: PDF import support disabled")
endif (HAVE_POPPLER)

if (HAVE_BOOST)
	MESSAGE(STATUS "boost found: Extra geometric tools support enabled")
else (HAVE_BOOST)
	MESSAGE(STATUS "boost not found: Extra geometric tools support disabled")
endif (HAVE_BOOST)

MESSAGE("--")
MESSAGE("-- Image formats:")

if (JPEG_FOUND)
	MESSAGE(STATUS "libjpeg found: JPEG support enabled")
else (JPEG_FOUND)
	MESSAGE(STATUS "libjpeg not found: JPEG support disabled")
endif (JPEG_FOUND)

if (HAVE_TIFF)
	MESSAGE(STATUS "libtiff found: TIFF support enabled")
else (HAVE_TIFF)
	MESSAGE(STATUS "libtiff not found: TIFF support disabled")
endif (HAVE_TIFF)

if (HAVE_OSG)
	MESSAGE(STATUS "osg found: Support for for OSG 3D Extension be supported")
else (HAVE_OSG)
	MESSAGE(STATUS "osg not found: Support for OSG 3D Extension not be supported")
endif (HAVE_OSG)

if (HAVE_REVENGE)
	MESSAGE(STATUS "librevenge found: Support for proprietary file import will be supported")
else (HAVE_REVENGE)
	MESSAGE(STATUS "librevenge not found: Support for proprietary file import will not be supported")
endif (HAVE_REVENGE)

if (HAVE_LIBVISIO)
	MESSAGE(STATUS "libvisio found: Support for Microsoft Visio files will be supported")
else (HAVE_LIBVISIO)
	MESSAGE(STATUS "libvisio not found: Support for Microsoft Visio files will not be supported")
endif (HAVE_LIBVISIO)

if (LIBCDR_FOUND)
	MESSAGE(STATUS "libcdr found: Support for CorelDraw files will be supported")
else (LIBCDR_FOUND)
	MESSAGE(STATUS "libcdr not found: Support for CorelDraw files will not be supported")
endif (LIBCDR_FOUND)

if (LIBVISIO_FOUND)
	MESSAGE(STATUS "libvisio found: Support for Microsoft Visio files will be supported")
else (LIBVISIO_FOUND)
	MESSAGE(STATUS "libvisio not found: Support for Microsoft Visio files will not be supported")
endif (LIBVISIO_FOUND)

if (LIBMSPUB_FOUND)
	MESSAGE(STATUS "libmspub found: Support for Microsoft Publisher files will be supported")
else (LIBMSPUB_FOUND)
	MESSAGE(STATUS "libmspub not found: Support for Microsoft Publisher files will not be supported")
endif (LIBMSPUB_FOUND)

if (LIBPAGEMAKER_FOUND)
	MESSAGE(STATUS "libpagemaker found: Support for PageMaker files will be supported")
else (LIBPAGEMAKER_FOUND)
	MESSAGE(STATUS "libpagemaker not found: Support for PageMaker files will not be supported")
endif (LIBPAGEMAKER_FOUND)

if (GMAGICK_FOUND)
	MESSAGE(STATUS "graphicsmagick found: Support for extra graphics formats will be supported")
else (GMAGICK_FOUND)
	MESSAGE(STATUS "graphicsmagick not found: Support for extra graphics formats files will not be supported")
endif (GMAGICK_FOUND)

MESSAGE("--------------------------------------------------")
MESSAGE("")

