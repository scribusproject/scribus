# For general Scribus (>=1.3.2) copyright and licensing information please refer
# to the COPYING file provided with the program. Following this notice may exist
# a copyright and/or license notice that predates the release of Scribus 1.3.2
# for which a new license (GPL+exception) is in place.

if(NOT DEFINED SCRIBUS_ROOT)
	message(FATAL_ERROR "SCRIBUS_ROOT is required")
endif()

set(WORKFLOW "${SCRIBUS_ROOT}/.github/workflows/windows-native-package.yml")
set(QT_INSTALLER "${SCRIBUS_ROOT}/win32/ci/install-qt.ps1")
set(BUILDER "${SCRIBUS_ROOT}/win32/ci/build-package.ps1")
set(VALIDATOR "${SCRIBUS_ROOT}/win32/ci/validate-package.ps1")
set(PORTABLE_ASSEMBLER "${SCRIBUS_ROOT}/win32/deploy/assemble.ps1")
set(INSTALLER_ASSEMBLER "${SCRIBUS_ROOT}/win32/installer/deploy.ps1")

foreach(REQUIRED_FILE IN ITEMS
		"${WORKFLOW}"
		"${QT_INSTALLER}"
		"${BUILDER}"
		"${VALIDATOR}"
		"${PORTABLE_ASSEMBLER}"
		"${INSTALLER_ASSEMBLER}")
	if(NOT EXISTS "${REQUIRED_FILE}")
		message(FATAL_ERROR "Required Windows packaging file is missing: ${REQUIRED_FILE}")
	endif()
endforeach()

function(require_text FILE_PATH REQUIRED_TEXT DESCRIPTION)
	file(READ "${FILE_PATH}" FILE_CONTENTS)
	string(FIND "${FILE_CONTENTS}" "${REQUIRED_TEXT}" TEXT_INDEX)
	if(TEXT_INDEX EQUAL -1)
		message(FATAL_ERROR "${DESCRIPTION} is missing from ${FILE_PATH}")
	endif()
endfunction()

require_text("${WORKFLOW}" "runs-on: windows-2022" "native Windows runner")
require_text("${WORKFLOW}" "install-qt.ps1" "verified Qt installer")
require_text("${WORKFLOW}" "qt-6.11.2-msvc2022-x64-20260813-v1" "pinned Qt cache")
require_text("${WORKFLOW}" "actions/cache/save@v4" "successful-stage cache persistence")
require_text("${WORKFLOW}" "steps.build-paths.outputs.builds-root" "normalized native build path")
require_text("${WORKFLOW}" "-DependencyBuildOnly" "separate dependency build stage")
require_text("${WORKFLOW}" "-ApplicationBuildOnly" "separate application build stage")
require_text("${WORKFLOW}" "-SkipApplicationBuild" "package-only stage")
require_text("${WORKFLOW}" "-TestInstaller" "installer smoke test")
require_text("${QT_INSTALLER}"
	"download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt6_6112"
	"official Qt 6.11.2 repository")
require_text("${QT_INSTALLER}" "Get-FileHash -LiteralPath $archivePath -Algorithm SHA1" "Qt archive checksum validation")
require_text("${QT_INSTALLER}" "Qt6Core5Compat.dll" "Qt 5 Core Compatibility validation")
require_text("${BUILDER}"
	"a6bd40450a22415d26cc9b0bf4aeaf9f295cfcff2f8bf64e9c62289aa69fdf3b"
	"pinned dependency archive checksum")
require_text("${BUILDER}" "scribus-libs-msvc2022.sln" "Visual Studio 2022 dependency solution")
require_text("${BUILDER}" "'Apscribe.exe'" "renamed Windows executable build output")
require_text("${BUILDER}" "/p:WindowsTargetPlatformVersion=10.0" "Windows 10 SDK retargeting")
require_text("${BUILDER}" "[System.IO.Directory]::GetParent($Sources).FullName" "sibling Visual Studio output directory")
require_text("${VALIDATOR}" "Assert-X64PE" "x64 PE validation")
require_text("${VALIDATOR}" "'Apscribe.exe'" "renamed Windows executable validation")
require_text("${VALIDATOR}" "[System.IO.Path]::GetDirectoryName($FileName)" "smoke-test working directory")
require_text("${VALIDATOR}" "Invoke-GuiStartupSmokeTest" "GUI startup smoke test")
require_text("${VALIDATOR}" "loaded $expectedPlatformPlugin" "packaged Qt platform plugin validation")
require_text("${VALIDATOR}" "python\\python313.dll" "bundled Python validation")
require_text("${VALIDATOR}" "qtplugins\\platforms\\qwindows.dll" "Qt Windows platform validation")
require_text("${VALIDATOR}" "'share\\icons'" "Windows icon resource validation")
require_text("${VALIDATOR}" "'share\\templates'" "Windows template resource validation")
require_text("${PORTABLE_ASSEMBLER}" "'python'" "Python portable staging")
require_text("${PORTABLE_ASSEMBLER}" "'Apscribe.exe'" "portable executable staging")
require_text("${PORTABLE_ASSEMBLER}" "New-Item -ItemType Directory -Path $AppDir" "portable output directory creation")
require_text("${PORTABLE_ASSEMBLER}" "win32\\qt.conf" "portable Qt plugin configuration")
require_text("${INSTALLER_ASSEMBLER}" "'python'" "Python installer staging")
require_text("${INSTALLER_ASSEMBLER}" "'Apscribe.exe'" "installer executable staging")
require_text("${INSTALLER_ASSEMBLER}" "New-Item -ItemType Directory -Path $StageDir" "installer staging directory creation")
require_text("${INSTALLER_ASSEMBLER}" "win32\\qt.conf" "installer Qt plugin configuration")
require_text("${SCRIBUS_ROOT}/win32/qt.conf" "Prefix=." "deployed Qt prefix")
require_text("${SCRIBUS_ROOT}/win32/qt.conf" "Plugins=qtplugins" "deployed Qt plugin path")
require_text("${SCRIBUS_ROOT}/win32/installer/Scribus.nsi" "MUI_LANGUAGE \"SimpChinese\"" "NSIS Simplified Chinese language identifier")
require_text("${SCRIBUS_ROOT}/win32/installer/Scribus.nsi" "!define PRODUCT_EXE \"Apscribe.exe\"" "NSIS executable identity")
foreach(MSVC_VERSION IN ITEMS 2019 2022 2026)
	require_text("${SCRIBUS_ROOT}/win32/msvc${MSVC_VERSION}/scribus-main/Scribus.vcxproj"
		"<TargetName>Apscribe</TargetName>" "MSVC ${MSVC_VERSION} executable target")
endforeach()
require_text("${SCRIBUS_ROOT}/win32/installer/Scribus.nsi" "MUI_LANGUAGE \"TradChinese\"" "NSIS Traditional Chinese language identifier")
require_text("${SCRIBUS_ROOT}/win32/installer/Scribus.nsi" "MUI_STARTMENU_GETFOLDER Application $StartMenuFolder" "NSIS start menu uninstall macro")

message(STATUS "Native Windows build and packaging contract is complete")
