
include_directories( ${LCMS2_INCLUDE_DIR} )

set(SCRIBUS_LCMS_IMPL_SOURCES
	colormgmt/sclcms2colormgmtengineimpl.cpp
	colormgmt/sclcms2colorprofileimpl.cpp
	colormgmt/sclcms2colortransformimpl.cpp
)

set(SCRIBUS_COLORMGMT_SOURCES
	colormgmt/scciedata.cpp
	colormgmt/sccieilluminants.cpp
	colormgmt/sccieobservers.cpp
	colormgmt/sccolormgmtengine.cpp
	colormgmt/sccolormgmtenginedata.cpp
	colormgmt/sccolormgmtenginefactory.cpp
	colormgmt/sccolormgmtimplelem.cpp
	colormgmt/sccolormgmtstructs.cpp
	colormgmt/sccolorprofile.cpp
	colormgmt/sccolorprofilecache.cpp
	colormgmt/sccolorprofiledata.cpp
	colormgmt/sccolorspace.cpp
	colormgmt/sccolorspacedata.cpp
	colormgmt/sccolortransform.cpp
	colormgmt/sccolortransformpool.cpp
	colormgmt/sce308tables.cpp
	colormgmt/scspectralvaluesconvertor.cpp
	${SCRIBUS_LCMS_IMPL_SOURCES}
)

