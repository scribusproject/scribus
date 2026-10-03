Native Windows test build
=========================

The scripts in this directory create and verify a real 64-bit Windows build.
They do not cross-compile or wrap a macOS/Linux binary.

Automated build (recommended)
-----------------------------
The manual GitHub Actions workflow is:

  .github/workflows/windows-native-package.yml

Run "Windows native package" from the repository's Actions page. It uses a
Windows Server 2022 runner with Visual Studio 2022, installs Qt 6.11.2 and
NSIS, builds the official Scribus dependency kit, builds Scribus, then uploads
one artifact containing:

  Apscribe-2.0.0-win64.zip
  Apscribe-2.0.0-Setup.exe

The verified Qt SDK, dependency build, and application build are cached after
their individual stages. The first run is therefore much slower than later
runs, and a packaging failure does not force a full recompile. Artifacts are
retained for 14 days.

Qt is downloaded from Qt's official online repository by install-qt.ps1. The
script pins the MSVC 2022 package revision and verifies each archive against
its published SHA-1 sidecar before extraction. This avoids relying on changing
third-party installer metadata. The extracted SDK is cached by GitHub Actions.

Local Windows build
-------------------
Requirements:

  * 64-bit Windows 10 or later
  * Visual Studio 2022 with "Desktop development with C++"
  * Qt 6.11.2 msvc2022_64 with Qt 5 Core Compatibility and Qt SVG
  * 7-Zip
  * NSIS 3.x
  * curl.exe (included with current Windows releases)

From a PowerShell prompt with MSBuild on PATH:

  powershell -ExecutionPolicy Bypass -File win32\ci\build-package.ps1 `
      -Sources C:\src\scribus `
      -QtDir C:\Qt\6.11.2\msvc2022_64 `
      -DependencyRoot C:\scribus-deps `
      -Version 2.0.0 `
      -TestInstaller

Keep the source and dependency paths short and free of unnecessary spaces.
The script pins the dependency archive and verifies its SHA-256 checksum
before extracting or compiling it. Its MSBuild invocation retargets legacy
dependency projects to the installed Windows 10 SDK while retaining the
Visual Studio 2022 v143 toolset.

Validation performed
--------------------
validate-package.ps1 rejects a package when it finds any of these problems:

  * Apscribe.exe is absent or is not an x64 PE executable
  * a required Qt, platform, or third-party runtime DLL is absent
  * qt.conf does not point Qt at the packaged qtplugins directory
  * the bundled Python runtime or Scripter standard library is absent
  * document resources, icons, templates, or plugins are absent
  * Debug runtime DLLs are mixed into the Release package
  * the portable ZIP or installer is missing or implausibly small
  * Scribus exits, crashes, or loads qwindows.dll outside the package during
    a bounded GUI startup smoke test
  * silent installation, installed-app startup, or uninstall fails

The installer smoke test uses a temporary directory and removes it after the
test. As expected by the Visual Studio projects, build output and logs are
written to the Scribus-builds directory beside the Scribus source directory.
