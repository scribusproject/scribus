# For general Scribus (>=1.3.2) copyright and licensing information please refer
# to the COPYING file provided with the program. Following this notice may exist
# a copyright and/or license notice that predates the release of Scribus 1.3.2
# for which a new license (GPL+exception) is in place.

if(NOT DEFINED SCRIBUS_ROOT)
	message(FATAL_ERROR "SCRIBUS_ROOT is required")
endif()

set(FORK_APPLICATION_FILES
	scribus/anchorposition.cpp
	scribus/anchorposition.h
	scribus/dynamicvariable.cpp
	scribus/dynamicvariable.h
	scribus/embeddedimageextractor.cpp
	scribus/embeddedimageextractor.h
	scribus/collectmanifest.cpp
	scribus/collectmanifest.h
	scribus/imagealphacontour.cpp
	scribus/imagealphacontour.h
	scribus/imagealphasilhouette.cpp
	scribus/imagecmykbatch.cpp
	scribus/imagecmykbatch.h
	scribus/imagecmykconversion.cpp
	scribus/imagecmykconversion.h
	scribus/imagecmykexport.cpp
	scribus/imagecmykexport.h
	scribus/imagelinkreplacement.cpp
	scribus/imagelinkreplacement.h
	scribus/imagelinkmatcher.cpp
	scribus/imagelinkmatcher.h
	scribus/styles/objectstyle.cpp
	scribus/styles/objectstyle.h
	scribus/stylequickapplymodel.cpp
	scribus/stylequickapplymodel.h
	scribus/ui/smobjectstyle.cpp
	scribus/ui/smobjectstyle.h
	scribus/ui/smobjectstylewidget.cpp
	scribus/ui/smobjectstylewidget.h
	scribus/ui/dynamicvariableinsert.cpp
	scribus/ui/dynamicvariableinsert.h
	scribus/ui/dynamicvariablemanager.cpp
	scribus/ui/dynamicvariablemanager.h
	scribus/ui/modernui.h
	scribus/ui/toolpalette.cpp
	scribus/ui/toolpalette.h
	scribus/ui/widgets/inspector_header.cpp
	scribus/ui/widgets/inspector_header.h)

set(FORK_SCRIPT_PLUGIN_FILES
	scribus/plugins/scriptplugin/cmdhistory.cpp
	scribus/plugins/scriptplugin/cmdhistory.h
	scribus/plugins/scriptplugin/cmdobjectstyleimport.cpp
	scribus/plugins/scriptplugin/cmdobjectstyleimport.h
	scribus/plugins/scriptplugin/cmdobjectstylemanagement.cpp
	scribus/plugins/scriptplugin/cmdobjectstylemanagement.h)

function(check_project_files MSVC_VERSION PROJECT_DIR_NAME PROJECT_FILE_NAME)
	set(PROJECT_DIR "${SCRIBUS_ROOT}/win32/msvc${MSVC_VERSION}/${PROJECT_DIR_NAME}")
	file(READ "${PROJECT_DIR}/${PROJECT_FILE_NAME}.vcxproj" PROJECT_CONTENTS)
	file(READ "${PROJECT_DIR}/${PROJECT_FILE_NAME}.vcxproj.filters" FILTER_CONTENTS)

	foreach(SOURCE_FILE IN LISTS ARGN)
		string(REPLACE "/" "\\" WINDOWS_SOURCE_FILE "${SOURCE_FILE}")
		set(PROJECT_REFERENCE "..\\..\\..\\${WINDOWS_SOURCE_FILE}")
		string(FIND "${PROJECT_CONTENTS}" "${PROJECT_REFERENCE}" PROJECT_INDEX)
		if(PROJECT_INDEX EQUAL -1)
			message(FATAL_ERROR
				"${SOURCE_FILE} is missing from the MSVC ${MSVC_VERSION} ${PROJECT_FILE_NAME} project")
		endif()

		string(FIND "${FILTER_CONTENTS}" "${PROJECT_REFERENCE}" FILTER_INDEX)
		if(FILTER_INDEX EQUAL -1)
			message(FATAL_ERROR
				"${SOURCE_FILE} is missing from the MSVC ${MSVC_VERSION} ${PROJECT_FILE_NAME} filters")
		endif()
	endforeach()
endfunction()

foreach(MSVC_VERSION IN ITEMS 2019 2022 2026)
	check_project_files(${MSVC_VERSION} scribus-main Scribus ${FORK_APPLICATION_FILES})
	check_project_files(${MSVC_VERSION} scriptplugin scriptplugin ${FORK_SCRIPT_PLUGIN_FILES})
endforeach()

message(STATUS "All fork application and Scripter files are registered in every Windows project")
