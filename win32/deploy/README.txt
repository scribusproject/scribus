Scribus 2.0 - portable Windows deployment
==========================================

This folder produces a self-contained, copy-and-run portable installation
of Scribus 2.0 for 64-bit Windows.

Layout
------
  win32/deploy/
      assemble.ps1                      <- run this on the Windows machine
      Sketch.txt                        <- target layout reference (this README's sibling)
      dist\                            <- created on build; NOT committed
          Apscribe-2.0.0-win64\         <- the portable app folder
              Apscribe.exe
              *.dll                     Qt6 + 3rd-party runtime DLLs
              qtplugins\                Qt platform/plugin DLLs (from windeployqt)
              plugins\                  Scribus import/export + tool plugins
              python\                   Bundled Python runtime for Scripter
              libs\                     scribus support dlls/lib
              share\                    resources, icons, translations, fonts,
                                        colour profiles, templates
          Apscribe-2.0.0-win64.zip      (only with -MakeZip)

The namespace folders (qtplugins\, plugins\, libs\, share\) under
Apscribe-2.0.0-win64\ contain only placeholder files in the repository;
assemble.ps1 rebuilds the real tree from the MSVC build output on Windows.

Requirements (Windows machine)
------------------------------
  * The Scribus source tree (this folder)
  * Visual Studio 2022 Release|x64 build -> Scribus-builds\Scribus-Release-x64-v143\Apscribe.exe
  * Qt 6 (>= 6.2) matching the build, e.g. F:\Libraries\Qt\6.11.2\msvc2022_64
  * scribus-libs kit (or explicit -LibsKitRoot)
      https://sourceforge.net/projects/scribus/files/scribus-libs/

Build
-----
  powershell -ExecutionPolicy Bypass -File win32\deploy\assemble.ps1
  powershell -ExecutionPolicy Bypass -File win32\deploy\assemble.ps1 `
      -BuildRoot "D:\Scribus\Scribus-builds\Scribus-Release-x64-v143" `
      -QtDir     "D:\Qt\6.11.2\msvc2022_64" `
      -LibsKitRoot "D:\Scribus Libs\scribus-1.7.x-libs-msvc" `
      -MakeZip

Result
------
  dist\Apscribe-2.0.0-win64\  (or the matching .zip) - copy it verbatim to any
  64-bit Windows PC and run Apscribe.exe. A test on the build machine is
  recommended before shipping.

Tip: to verify everything is present, run Apscribe.exe from the staged folder
and open File > New Document - if share\ or qtplugins\ is missing the app
either fails to start or has no icons/translations.
