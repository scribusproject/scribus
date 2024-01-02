##############################################################################################################
########## Install/Directory Setup                                                                  ##########

if (APPLEBUNDLE)
	set (APPLE_APP_DIR "${CMAKE_INSTALL_PREFIX}")
	if ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/Contents/?$")
		# all ok
	else()
		if ("${CMAKE_INSTALL_PREFIX}" MATCHES "[.]app/?$")
			# add "/Contents" subpath
			set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/Contents")
		else()
			# no bundle name specified, use appname and version
			set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/Scribus${VERSION}.app/Contents")
		endif()
	endif()
	message(STATUS "APPLEBUNDLE install prefix: ${APPLE_APP_DIR}")
endif()


#Setup all the directories we will use
#cmake_policy(SET CMP0005 OLD)
#SHARE
if(WANT_VERSIONING)
	set(SHAREDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/")
else()
	set(SHAREDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}/")
endif()

if(IS_ABSOLUTE ${SHAREDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: SHAREDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${SHAREDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DSHAREDIR="${CMAKE_INSTALL_PREFIX}/${SHAREDIR}")
else()
	add_definitions(-DSHAREDIR="${SHAREDIR}")
endif()

#DOC - use the default on Apple as TAG_VERSION is empty
if(NOT DOCDIR)
	if(WIN32 OR OS2)
		set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/")
	elseif(TAG_VERSION OR BUILD_OSX_BUNDLE)
		set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${MAIN_DIR_NAME}${TAG_VERSION}/")
	elseif(NOT WANT_VERSIONING)
		set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${MAIN_DIR_NAME}/")
	else()
		set(DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${MAIN_DIR_NAME}-${VERSION}/")
	endif()
endif()

if(IS_ABSOLUTE ${DOCDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: DOCDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${DOCDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DDOCDIR="${CMAKE_INSTALL_PREFIX}/${DOCDIR}")
else()
	add_definitions(-DDOCDIR="${DOCDIR}")
endif()

#ICONS
if(WIN32)
	set(ICONDIR "${CMAKE_INSTALL_DATAROOTDIR}/icons/")
else()
	set(ICONDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/icons/")
endif()

if(IS_ABSOLUTE ${ICONDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: ICONDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${ICONDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DICONDIR="${CMAKE_INSTALL_PREFIX}/${ICONDIR}")
else()
	add_definitions(-DICONDIR="${ICONDIR}")
endif()

#SAMPLES
if(WIN32)
	set(SAMPLESDIR "${CMAKE_INSTALL_DATAROOTDIR}/samples/")
else()
	set(SAMPLESDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/samples/")
endif()

if(IS_ABSOLUTE ${SAMPLESDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: SAMPLESDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${SAMPLESDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DSAMPLESDIR="${CMAKE_INSTALL_PREFIX}/${SAMPLESDIR}")
else()
	add_definitions(-DSAMPLESDIR="${SAMPLESDIR}")
endif()

#SCRIPTS
if(WIN32)
	set(SCRIPTSDIR "${CMAKE_INSTALL_DATAROOTDIR}/scripts/")
else()
	set(SCRIPTSDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/scripts/")
endif()

if(IS_ABSOLUTE ${SCRIPTSDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: SCRIPTSDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${SCRIPTSDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DSCRIPTSDIR="${CMAKE_INSTALL_PREFIX}/${SCRIPTSDIR}")
else()
	add_definitions(-DSCRIPTSDIR="${SCRIPTSDIR}")
endif()

#TEMPLATES
if(WIN32)
	set(TEMPLATEDIR "${CMAKE_INSTALL_DATAROOTDIR}/templates/")
else()
	set(TEMPLATEDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/templates/")
endif()

if(IS_ABSOLUTE ${TEMPLATEDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: TEMPLATEDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${TEMPLATEDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DTEMPLATEDIR="${CMAKE_INSTALL_PREFIX}/${TEMPLATEDIR}")
else()
	add_definitions(-DTEMPLATEDIR="${TEMPLATEDIR}")
endif()

#QML
if(WIN32)
	set(QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml/")
else()
	set(QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/${MAIN_DIR_NAME}${TAG_VERSION}/qml/")
endif()

if(IS_ABSOLUTE ${QMLDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: QMLDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${QMLDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DQMLDIR="${CMAKE_INSTALL_PREFIX}/${QMLDIR}")
else()
	add_definitions(-DQMLDIR="${QMLDIR}")
endif()

#DESKTOP
set(DESKTOPDIR "${CMAKE_INSTALL_DATAROOTDIR}/applications/")

if(IS_ABSOLUTE ${DESKTOPDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: DESKTOPDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${DESKTOPDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DDESKTOPDIR="${CMAKE_INSTALL_PREFIX}/${DESKTOPDIR}")
else()
	add_definitions(-DDESKTOPDIR="${DESKTOPDIR}")
endif()

#APPDATA
set(APPDATADIR "${CMAKE_INSTALL_DATAROOTDIR}/metainfo/")

if(IS_ABSOLUTE ${APPDATADIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: APPDATADIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${APPDATADIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DAPPDATADIR="${CMAKE_INSTALL_PREFIX}/${APPDATADIR}")
else()
	add_definitions(-DAPPDATADIR="${APPDATADIR}")
endif()


# APPLICATION_DATA_DIR
# It allows specify the user configuration directory in the compile time.
# Example: I want to use special ~/.scribus135svn config directory for my trunk
#          cmake -DAPPLICATION_DATA_DIR=.scribus135svn ...
if (APPLICATION_DATA_DIR)
	message(STATUS "Using ApplicationDataDir ${APPLICATION_DATA_DIR} instead of the platform default.")
	add_definitions(-DAPPLICATION_DATA_DIR="${APPLICATION_DATA_DIR}")
else()
	message(STATUS "Using standard ApplicationDataDir. You can change it with -DAPPLICATION_DATA_DIR")
endif()

if (APPLICATION_CONFIG_DIR)
	message(STATUS "Using ApplicationConfigDir ${APPLICATION_CONFIG_DIR} instead of the platform default.")
	add_definitions(-DAPPLICATION_CONFIG_DIR="${APPLICATION_CONFIG_DIR}")
else()
	message(STATUS "Using standard ApplicationConfigDir. You can change it with -DAPPLICATION_CONFIG_DIR")
endif()

#LIB LOCATION - lib or lib64 normally
#LIB
if(WANT_LIB64)
	if(NOT ARCH_X86_64)
		message(STATUS "WARNING: BUILDING 64 Bit build on non 64 bit architecture")
	endif()
	set(LIB_SUFFIX "64" CACHE STRING "Library directory suffix")
endif()

set(LIB_DIR_NAME "lib${LIB_SUFFIX}")
if (APPLEBUNDLE)
	set(LIBDIR "lib/")
else()
	set(LIBDIR "${LIB_DIR_NAME}/${MAIN_DIR_NAME}${TAG_VERSION}/")
endif()

if(IS_ABSOLUTE ${LIBDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: LIBDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${LIBDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DLIBDIR="${CMAKE_INSTALL_PREFIX}/${LIBDIR}")
else()
	add_definitions(-DLIBDIR="${LIBDIR}")
endif()

#PLUGINS
if(WIN32)
	set(PLUGINDIR "plugins")
elseif (APPLEBUNDLE)
	set(PLUGINDIR "${LIBDIR}")
else()
	set(PLUGINDIR "${LIB_DIR_NAME}/${MAIN_DIR_NAME}${TAG_VERSION}/plugins/")
endif()

if(IS_ABSOLUTE ${PLUGINDIR} AND WANT_RELOCATABLE)
	message(FATAL_ERROR "ERROR: PLUGINDIR must be relative when using WANT_RELOCATABLE option")
elseif(NOT IS_ABSOLUTE ${PLUGINDIR} AND NOT WANT_RELOCATABLE)
	add_definitions(-DPLUGINDIR="${CMAKE_INSTALL_PREFIX}/${PLUGINDIR}")
else()
	add_definitions(-DPLUGINDIR="${PLUGINDIR}")
endif()


#INCLUDE
set(INCLUDEDIR "include/${MAIN_DIR_NAME}${TAG_VERSION}/")
#NO ADD DEFINITIONS, WE ONLY INSTALL WITH IT

#RPATH
if (WANT_NORPATH OR WANT_DISTROBUILD)
	set(CMAKE_INSTALL_RPATH "")
else()
	set(CMAKE_INSTALL_RPATH ${PLUGINDIR})
endif()

# Set the XDG mime stuff
set(MIMEDIR "${CMAKE_INSTALL_DATAROOTDIR}/mime/packages/")
