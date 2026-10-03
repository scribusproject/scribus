;----------------------------------------------------------------------------
;  Scribus - Open Source Desktop Publishing
;  Windows installer script (NSIS 3.x, Unicode)
;
;  This script packages the application tree staged in the "app" subfolder
;  (created by deploy.ps1 from an MSVC build output). You can also run
;  makensis directly after placing your Scribus.exe + DLLs/resources into
;  the "app" folder.
;----------------------------------------------------------------------------

Unicode true
SetCompressor /SOLID lzma
SetCompressorDictSize 64

!include "MUI2.nsh"
!include "x64.nsh"
!include "LogicLib.nsh"

;--------------------------------------------------------------------------
; Version / naming (can be overridden on the makensis command line with
; /DVERSION=..., e.g. from deploy.ps1)
;--------------------------------------------------------------------------
!ifndef VERSION
  !define VERSION "2.0.0"
!endif

!define PRODUCT_NAME "Apscribe ${VERSION}"
!define PRODUCT_SHORTNAME "Apscribe"
!define PRODUCT_PUBLISHER "Appaji Ambarisha Darbha and Scribus contributors"
!define PRODUCT_WEB_SITE "https://github.com/appajid/scribus"
; Keep the executable filename until the MSVC projects and deploy scripts can
; migrate together; the installer itself is independent of official Scribus.
!define PRODUCT_EXE "Scribus.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\Apscribe ${VERSION}"
!define PRODUCT_INSTALL_KEY "Software\Apscribe\Apscribe ${VERSION}"
!define PRODUCT_ASSOC_KEY "Software\Apscribe\Apscribe ${VERSION}\Associations"
!define PRODUCT_DOCUMENT_CLASS "Apscribe.Document.${VERSION}"
!define PRODUCT_STARTMENU_REGVAL "Start Menu Folder"

Name "${PRODUCT_NAME}"
OutFile "dist\Apscribe-${VERSION}-Setup.exe"
InstallDir "$PROGRAMFILES64\${PRODUCT_NAME}"
InstallDirRegKey HKLM "${PRODUCT_INSTALL_KEY}" "InstallDir"
RequestExecutionLevel admin

Icon "..\msvc2022\scribus-main\scribusicon.ico"
UninstallIcon "..\msvc2022\scribus-main\scribusicon.ico"

Var StartMenuFolder

;--------------------------------------------------------------------------
; MUI settings
;--------------------------------------------------------------------------
!define MUI_ABORTWARNING
!define MUI_ICON "..\msvc2022\scribus-main\scribusicon.ico"
!define MUI_UNICON "..\msvc2022\scribus-main\scribusicon.ico"
!define MUI_LANGDLL_ALLLANGUAGES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${PRODUCT_EXE}"
!define MUI_FINISHPAGE_RUN_TEXT "Launch ${PRODUCT_NAME}"

!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_INSTALL_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\COPYING"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"

;--------------------------------------------------------------------------
; Components
;--------------------------------------------------------------------------
Section "Apscribe ${VERSION} (required)" SecMain
  SectionIn RO
  SetShellVarContext all
  SetOutPath "$INSTDIR"
  File /r /x *.pdb /x *.ilk /x *.exp "app\*.*"

  WriteUninstaller "$INSTDIR\uninst.exe"

  ; install path + upgrade info
  WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "InstallDir" "$INSTDIR"
  WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "Version" "${VERSION}"

  ; Add / Remove Programs entry
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PRODUCT_EXE}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "QuietUninstallString" "$INSTDIR\uninst.exe /S"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegDWORD HKLM "${PRODUCT_UNINST_KEY}" "NoModify" 1
  WriteRegDWORD HKLM "${PRODUCT_UNINST_KEY}" "NoRepair" 1
SectionEnd

Section "Start Menu shortcuts" SecStartMenu
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_EXE}"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section /o "Desktop shortcut" SecDesktop
  CreateShortcut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_EXE}"
SectionEnd

Section /o "File associations (.sla / .sla.gz)" SecAssoc
  SetShellVarContext current
  ReadRegStr $R0 HKCU "Software\Classes\.sla" ""
  WriteRegStr HKCU "${PRODUCT_ASSOC_KEY}" "PreviousSla" "$R0"
  ReadRegStr $R0 HKCU "Software\Classes\.sla.gz" ""
  WriteRegStr HKCU "${PRODUCT_ASSOC_KEY}" "PreviousSlaGz" "$R0"
  WriteRegStr HKCU "Software\Classes\.sla" "" "${PRODUCT_DOCUMENT_CLASS}"
  WriteRegStr HKCU "Software\Classes\.sla.gz" "" "${PRODUCT_DOCUMENT_CLASS}"
  WriteRegStr HKCU "Software\Classes\${PRODUCT_DOCUMENT_CLASS}" "" "Apscribe Document (Scribus format)"
  WriteRegStr HKCU "Software\Classes\${PRODUCT_DOCUMENT_CLASS}\DefaultIcon" "" "$INSTDIR\${PRODUCT_EXE},0"
  WriteRegStr HKCU "Software\Classes\${PRODUCT_DOCUMENT_CLASS}\shell\open\command" "" '"$INSTDIR\${PRODUCT_EXE}" "%1"'
  WriteRegStr HKCU "Software\Classes\${PRODUCT_DOCUMENT_CLASS}\shell\edit\command" "" '"$INSTDIR\${PRODUCT_EXE}" "%1"'
SectionEnd

;--------------------------------------------------------------------------
; Descriptions
;--------------------------------------------------------------------------
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "Apscribe ${VERSION} application, plugins, translations and resources."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} "Add shortcuts to the Start Menu."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} "Add a shortcut on the desktop."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecAssoc} "Associate Scribus-format documents (.sla, .sla.gz) with Apscribe."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------------------------------------------------
; Installer
;--------------------------------------------------------------------------
Function .onInit
  ${If} ${RunningX64}
    SetRegView 64
  ${EndIf}
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;--------------------------------------------------------------------------
; Uninstaller
;--------------------------------------------------------------------------
Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

Section "Uninstall"
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall ${PRODUCT_NAME}.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"

  ReadRegStr $R0 HKCU "Software\Classes\.sla" ""
  ${If} $R0 == "${PRODUCT_DOCUMENT_CLASS}"
    ReadRegStr $R1 HKCU "${PRODUCT_ASSOC_KEY}" "PreviousSla"
    ${If} $R1 == ""
      DeleteRegValue HKCU "Software\Classes\.sla" ""
    ${Else}
      WriteRegStr HKCU "Software\Classes\.sla" "" "$R1"
    ${EndIf}
  ${EndIf}
  ReadRegStr $R0 HKCU "Software\Classes\.sla.gz" ""
  ${If} $R0 == "${PRODUCT_DOCUMENT_CLASS}"
    ReadRegStr $R1 HKCU "${PRODUCT_ASSOC_KEY}" "PreviousSlaGz"
    ${If} $R1 == ""
      DeleteRegValue HKCU "Software\Classes\.sla.gz" ""
    ${Else}
      WriteRegStr HKCU "Software\Classes\.sla.gz" "" "$R1"
    ${EndIf}
  ${EndIf}
  DeleteRegKey HKCU "Software\Classes\${PRODUCT_DOCUMENT_CLASS}"
  DeleteRegKey HKCU "${PRODUCT_ASSOC_KEY}"

  DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_INSTALL_KEY}"

  Delete "$INSTDIR\uninst.exe"
  RMDir /r "$INSTDIR"

  SetAutoClose true
SectionEnd
