Scribus Windows Installer
=========================

This directory contains everything needed to build a Windows installer
(Apscribe-<version>-Setup.exe) from a native MSVC build of this Scribus fork.

Contents
--------
  Scribus.nsi        NSIS installer script (NSIS 3.x, Unicode)
  deploy.ps1         PowerShell script: stages the MSVC output, deploys the
                     Qt runtime and the scribus-libs DLLs, then runs makensis
  app/               staging area created by deploy.ps1 (removed/rebuilt each run)

Requirements (on the Windows build machine)
-------------------------------------------
  * The Scribus source tree (this folder is win32\installer inside it)
  * Visual Studio 2022 build of Scribus, Release|x64
      - produced as <sources>\Scribus-builds\Scribus-Release-x64-v143\
      - see ..\..\BUILDING_win32_msvc.txt
  * Qt 6 (>= 6.2), matching the build, e.g. F:\Libraries\Qt\6.11.2\msvc2022_64
  * The scribus-libs kit, e.g. F:\Scribus Libs\scribus-1.7.x-libs-msvc
      - https://sourceforge.net/projects/scribus/files/scribus-libs/
  * NSIS 3.x  ->  https://nsis.sourceforge.io/Download

Build steps
-----------
  1. Build Scribus (Release | x64) in Visual Studio 2022, following
     BUILDING_win32_msvc.txt. The build output must contain Apscribe.exe.

  2. Open a PowerShell window and run, from anywhere:

       powershell -ExecutionPolicy Bypass -File .\win32\installer\deploy.ps1

     The script auto-detects:
       - the build output (Scribus-builds\Scribus-Release-x64-*)
       - Qt (F:\Libraries\Qt\6.11.2\msvc2022_64) from win32\msvc2022\
         Scribus-build-props.props or the QT6_DIR environment variable
       - the scribus-libs kit (SCRIBUS_LIB_ROOT env var, Scribus-build-props.props,
         or a sibling folder named scribus-1.7.x-libs-msvc)
       - makensis.exe on PATH or in "C:\Program Files\NSIS"

     If auto-detection does not match your machine, pass explicit values:

       powershell -ExecutionPolicy Bypass -File .\win32\installer\deploy.ps1 `
           -BuildRoot   "D:\Scribus\Scribus-builds\Scribus-Release-x64-v143" `
           -QtDir       "D:\Qt\6.11.2\msvc2022_64" `
           -LibsKitRoot "D:\Scribus Libs\scribus-1.7.x-libs-msvc" `
           -Makensis    "C:\Program Files (x86)\NSIS\makensis.exe"

  3. Result: win32\installer\dist\Apscribe-<version>-Setup.exe

     Copy this file to the target PC and run it as Administrator.

What the installer does
-----------------------
  * Installs to C:\Program Files\Apscribe <version> (by default)
  * Installs Apscribe.exe, all Qt runtime DLLs + Qt plugins (qtplugins\),
    the Scribus plugins (plugins\), bundled Python runtime (python\), support
    files (libs\) and all resources, translations, fonts, colour profiles,
    templates under share\
  * Creates Start Menu and (optional) desktop shortcuts
  * Optionally registers the .sla / .sla.gz file associations
  * Registers an Add/Remove Programs entry with a full uninstaller
  * Supports several installer languages (English, French, German, Italian,
    Spanish, Russian, Polish, Simplified & Traditional Chinese)

Troubleshooting
---------------
  * "Apscribe.exe not found"          -> build the Release|x64 configuration,
     the output directory must contain Apscribe.exe.
  * "Could not locate Qt"            -> install Qt or pass -QtDir.
  * "Could not locate scribus-libs"  -> download the kit or pass -LibsKitRoot.
  * makensis warning / missing       -> install NSIS 3.x or pass -Makensis.
  * windeployqt missing              -> ensure the Qt installation contains
     tools\windeployqt.exe (bin\windeployqt.exe in the Qt dir).
  * Debug vs Release mix             -> both Scribus and all dependencies must
     be the same configuration/architecture (see BUILDING_win32_msvc.txt).

Manual staging (optional)
-------------------------
For advanced use, place a ready-made application tree into app\ (Apscribe.exe,
runtime DLLs, qtplugins\, plugins\, python\, libs\, share\...) and run directly:

    makensis /DVERSION=2.0.0 Scribus.nsi
