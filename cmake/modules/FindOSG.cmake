# Locate gdal
# This module defines
# OSG_LIBRARY
# OSG_FOUND, if false, do not try to link to gdal 
# OSG_INCLUDE_DIR, where to find the headers
#
# $OSG_DIR is an environment variable that would
# correspond to the ./configure --prefix=$OSG_DIR
#
# Created by Robert Osfield. 

find_path(OSG_INCLUDE_DIR osg/Node
    ${OSG_DIR}/include
    $ENV{OSG_DIR}/include
    $ENV{OSG_DIR}
    $ENV{OSGDIR}/include
    $ENV{OSGDIR}
    $ENV{OSG_ROOT}/include
    NO_DEFAULT_PATH
)

find_path(OSG_INCLUDE_DIR osg/Node)

macro(FIND_OSG_LIBRARY MYLIBRARY MYLIBRARYNAME)

    find_library("${MYLIBRARY}_DEBUG"
        NAMES "${MYLIBRARYNAME}${CMAKE_DEBUG_POSTFIX}"
        PATHS
        ${OSG_DIR}/lib/Debug
        ${OSG_DIR}/lib
        $ENV{OSG_DIR}/lib/debug
        $ENV{OSG_DIR}/lib
        $ENV{OSG_DIR}
        $ENV{OSGDIR}/lib
        $ENV{OSGDIR}
        $ENV{OSG_ROOT}/lib
        NO_DEFAULT_PATH
    )

    find_library("${MYLIBRARY}_DEBUG"
        NAMES "${MYLIBRARYNAME}${CMAKE_DEBUG_POSTFIX}"
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/local/lib64
        /usr/lib
        /usr/lib64
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/lib
        /usr/freeware/lib64
    )
    
    find_library(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        ${OSG_DIR}/lib/Release
        ${OSG_DIR}/lib
        $ENV{OSG_DIR}/lib/Release
        $ENV{OSG_DIR}/lib
        $ENV{OSG_DIR}
        $ENV{OSGDIR}/lib
        $ENV{OSGDIR}
        $ENV{OSG_ROOT}/lib
        NO_DEFAULT_PATH
    )

    find_library(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/local/lib64
        /usr/lib
        /usr/lib64
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/lib
        /usr/freeware/lib64
    )
    
    if( NOT ${MYLIBRARY}_DEBUG)
        if(MYLIBRARY)
            set(${MYLIBRARY}_DEBUG ${MYLIBRARY})
         endif(MYLIBRARY)
    endif( NOT ${MYLIBRARY}_DEBUG)
           
endmacro(FIND_OSG_LIBRARY LIBRARY LIBRARYNAME)

FIND_OSG_LIBRARY(OSG_LIBRARY osg)
FIND_OSG_LIBRARY(OSGGA_LIBRARY osgGA)
FIND_OSG_LIBRARY(OSGUTIL_LIBRARY osgUtil)
FIND_OSG_LIBRARY(OSGDB_LIBRARY osgDB)
FIND_OSG_LIBRARY(OSGTEXT_LIBRARY osgText)
FIND_OSG_LIBRARY(OSGWIDGET_LIBRARY osgWidget)
FIND_OSG_LIBRARY(OSGTERRAIN_LIBRARY osgTerrain)
FIND_OSG_LIBRARY(OSGFX_LIBRARY osgFX)
FIND_OSG_LIBRARY(OSGVIEWER_LIBRARY osgViewer)
FIND_OSG_LIBRARY(OSGVOLUME_LIBRARY osgVolume)
FIND_OSG_LIBRARY(OPENTHREADS_LIBRARY OpenThreads)

set(OSG_FOUND "NO")
if(OSG_LIBRARY AND OSG_INCLUDE_DIR)
    set(OSG_FOUND "YES")
else(OSG_LIBRARY AND OSG_INCLUDE_DIR)
    set(OSG_INCLUDE_DIR "")
endif(OSG_LIBRARY AND OSG_INCLUDE_DIR)
