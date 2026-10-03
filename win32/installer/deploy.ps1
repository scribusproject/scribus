#----------------------------------------------------------------------------
#  Scribus - Open Source Desktop Publishing
#  Windows installer build script (PowerShell 5.1+)
#
#  Run on the Windows build machine AFTER building Scribus with the MSVC
#  solution (win32\msvc2022\Scribus.sln). This script:
#    1. stages the MSVC Release|x64 output into win32\installer\app
#    2. deploys the Qt6 runtime (windeployqt) so Scribus runs on a PC
#    3. copies the third-party DLLs provided by the scribus-libs kit
#    4. runs NSIS (makensis) to produce Apscribe-<version>-Setup.exe
#
#  Usage:
#    powershell -ExecutionPolicy Bypass -File deploy.ps1
#    powershell -ExecutionPolicy Bypass -File deploy.ps1 `
#        -QtDir "F:\Libraries\Qt\6.11.2\msvc2022_64" `
#        -LibsKitRoot "F:\Scribus Libs\scribus-1.7.x-libs-msvc" `
#        -BuildRoot "C:\scribus\Scribus-builds\Scribus-Release-x64-v143"
#----------------------------------------------------------------------------
[CmdletBinding()]
param(
    [string]$Sources,                   # Scribus source root (default: this script's parent\..\..)
    [string]$BuildRoot,                 # MSVC application output dir, e.g. ...\Scribus-Release-x64-v143
    [string]$Configuration = 'Release',
    [string]$Platform = 'x64',
    [string]$PlatformToolset,           # e.g. v143 (auto-detected if omitted)
    [string]$QtDir,                     # Qt install dir, e.g. F:\Libraries\Qt\6.11.2\msvc2022_64
    [string]$LibsKitRoot,               # scribus-libs kit root (auto-detected if omitted)
    [string]$Makensis,                  # full path to makensis.exe (searched if omitted)
    [string]$Version,                   # version string; defaults to Scribus-version-infos.h (auto-detected)
    [switch]$SkipWindeploy,             # skip Qt runtime deployment
    [switch]$SkipNsis                   # only stage files, do not build the installer
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 3.0

#----------------------------------------------------------------------------
# Paths
#----------------------------------------------------------------------------
$ScriptDir   = $PSScriptRoot
if (-not $Sources -or -not (Test-Path -LiteralPath $Sources)) {
    $Sources = Split-Path (Split-Path -LiteralPath $ScriptDir) -Parent
}
$Sources = (Resolve-Path -LiteralPath $Sources).Path
$PropsFile   = Join-Path $Sources "win32\msvc2022\Scribus-build-props.props"
$VersionFile = Join-Path $Sources "win32\msvc2022\Scribus-version-infos.h"
$StageDir    = Join-Path $ScriptDir 'app'
$DistDir     = Join-Path $ScriptDir 'dist'

#----------------------------------------------------------------------------
# Helpers
#----------------------------------------------------------------------------
function Read-TextFile([string]$Path) {
    if (Test-Path -LiteralPath $Path) { return (Get-Content -LiteralPath $Path -Raw) }
    return $null
}

function Get-XmlValue($Content, [string]$Tag) {
    if ($null -eq $Content) { return $null }
    $m = [regex]::Match($Content, "<$Tag(?:\s[^>]*)?>([^<]+)</$Tag>")
    if ($m.Success) { return $m.Groups[1].Value.Trim() }
    return $null
}

function Get-DefaultToolset {
    # v143 = VS2022 (the currently supported toolchain in win32/msvc2022)
    return 'v143'
}

function Find-BuildRoot {
    $base = Join-Path $Sources 'Scribus-builds'
    if (-not (Test-Path -LiteralPath $base)) { return $null }

    $pattern = "Scribus-$Configuration-$Platform-*"
    $dirs = Get-ChildItem -LiteralPath $base -Directory -Filter $pattern -ErrorAction SilentlyContinue |
            Sort-Object LastWriteTime -Descending
    foreach ($d in $dirs) {
        if (Test-Path -LiteralPath (Join-Path $d.FullName 'Apscribe.exe')) {
            return $d.FullName
        }
    }
    return $null
}

function Find-QtDir {
    # Prefer a Qt matching the selected toolset (v143 -> msvc2022/64), then
    # fall back to any x64 entry that actually exists on this machine.
    $content = Read-TextFile $PropsFile
    if ($content) {
        $matches = [regex]::Matches($content, "<QT6_DIR[^>]*>[^<]+</QT6_DIR>")
        if ($PlatformToolset) {
            foreach ($m in $matches) {
                if ($m.Value -match "x64" -and $m.Value -match "$PlatformToolset") {
                    $inner = Get-XmlValue $m.Value 'QT6_DIR'
                    if ($inner -and (Test-Path -LiteralPath $inner)) { return $inner }
                }
            }
        }
        foreach ($m in $matches) {
            if ($m.Value -match "x64") {
                $inner = Get-XmlValue $m.Value 'QT6_DIR'
                if ($inner -and (Test-Path -LiteralPath $inner)) { return $inner }
            }
        }
    }
    return $null
}

function Find-LibsKitRoot {
    $envVal = $env:SCRIBUS_LIB_ROOT
    if ($envVal -and (Test-Path -LiteralPath $envVal)) { return $envVal }

    $content = Read-TextFile $PropsFile
    $val = Get-XmlValue $content 'SCRIBUS_LIB_ROOT'
    if ($val -and (Test-Path -LiteralPath $val)) { return $val }

    # Sibling folder of the sources, as documented
    $candidate = Join-Path (Split-Path -Parent $Sources) 'scribus-1.7.x-libs-msvc'
    if (Test-Path -LiteralPath $candidate) { return $candidate }
    return $null
}

function Find-Makensis {
    $custom = Get-Command 'makensis.exe' -ErrorAction SilentlyContinue
    if ($custom) { return $custom.Source }
    foreach ($p in @(
        "$env:ProgramFiles\NSIS\makensis.exe",
        "${env:ProgramFiles(x86)}\NSIS\makensis.exe",
        "$env:LOCALAPPDATA\Programs\NSIS\makensis.exe"
    )) {
        if ($p -and (Test-Path -LiteralPath $p)) { return $p }
    }
    return $null
}

#----------------------------------------------------------------------------
# Resolve configuration
#----------------------------------------------------------------------------
Write-Host "== Scribus Windows installer build ==" -ForegroundColor Cyan

if (-not $PlatformToolset) { $PlatformToolset = Get-DefaultToolset }

if (-not $BuildRoot) {
    $BuildRoot = Find-BuildRoot
    if ($BuildRoot) { Write-Host "  Build output auto-detected: $BuildRoot" }
}
if (-not $BuildRoot) {
    throw "Could not locate the Scribus build output ('Apscribe.exe' under '$Sources\Scribus-builds\Scribus-$Configuration-$Platform-*'). Use -BuildRoot to specify it."
}
if (-not (Test-Path -LiteralPath (Join-Path $BuildRoot 'Apscribe.exe'))) {
    throw "Apscribe.exe not found in '$BuildRoot'. Build the Release|x64 (or $Platform) configuration first."
}
$BuildRoot = (Resolve-Path -LiteralPath $BuildRoot).Path

if (-not $QtDir) { $QtDir = Find-QtDir }
if (-not $QtDir) {
    throw "Could not locate Qt. Set -QtDir (or F:\Libraries\Qt\6.11.2\msvc2022_64)."
}
if (-not (Test-Path -LiteralPath (Join-Path $QtDir 'bin\windeployqt.exe'))) {
    Write-Warning "windeployqt.exe not found under '$QtDir\bin'. Qt deployment will be skipped."
    $Script:WindeployAvailable = $false
} else {
    $Script:WindeployAvailable = $true
}

if (-not $LibsKitRoot) { $LibsKitRoot = Find-LibsKitRoot }
if (-not $LibsKitRoot) {
    throw "Could not locate the scribus-libs kit. Set -LibsKitRoot (or F:\Scribus Libs\scribus-1.7.x-libs-msvc)."
}
$LibsKitRoot = (Resolve-Path -LiteralPath $LibsKitRoot).Path

if (-not $Version) {
    $vc = Read-TextFile $VersionFile
    $vm = [regex]::Match([string]$vc, 'SCRIBUS_FULL_VERSION_STRING\s+"([^"]+)"')
    if ($vm.Success) { $Version = $vm.Groups[1].Value }
}
if (-not $Version) { $Version = '2.0.0' }

if (-not $Makensis) { $Makensis = Find-Makensis }
if ((-not $SkipNsis) -and -not $Makensis) {
    Write-Warning "makensis.exe not found. Install NSIS (https://nsis.sourceforge.io) or set -Makensis. Installer will not be built."
}

Write-Host "  Sources        : $Sources"
Write-Host "  Build output   : $BuildRoot"
Write-Host "  Qt             : $QtDir"
Write-Host "  Scribus-libs   : $LibsKitRoot"
Write-Host "  Version        : $Version"

#----------------------------------------------------------------------------
# Stage the application tree
#----------------------------------------------------------------------------
Write-Host "== Staging application tree -> $StageDir ==" -ForegroundColor Cyan
if (Test-Path -LiteralPath $StageDir) { Remove-Item -LiteralPath $StageDir -Recurse -Force }
New-Item -ItemType Directory -Path $StageDir -Force | Out-Null

# Application files produced by the MSVC build
Copy-Item -LiteralPath (Join-Path $BuildRoot 'Apscribe.exe') -Destination $StageDir

Get-ChildItem -LiteralPath $BuildRoot -Filter '*.dll' -File -ErrorAction SilentlyContinue |
    Copy-Item -Destination $StageDir

foreach ($sub in @('libs', 'plugins', 'python', 'share')) {
    $src = Join-Path $BuildRoot $sub
    if (Test-Path -LiteralPath $src) {
        Copy-Item -LiteralPath $src -Destination $StageDir -Recurse
    }
}

# Third-party DLLs from the scribus-libs kit. They are usually referenced by
# <XXX>_LIB_DIR variables in scribus-lib-paths.props; look in every such dir
# (plus the kit's bin folder) and copy all DLLs found.
$dllSourceDirs = New-Object System.Collections.Generic.List[string]
if (Test-Path -LiteralPath (Join-Path $LibsKitRoot 'scribus-lib-paths.props')) {
    $props = Read-TextFile (Join-Path $LibsKitRoot 'scribus-lib-paths.props')
    if ($props) {
        foreach ($m in [regex]::Matches($props, '<[A-Z0-9_]+_LIB_DIR>([^<]+)</[A-Z0-9_]+_LIB_DIR>')) {
            $dir = $m.Groups[1].Value
            if (Test-Path -LiteralPath $dir) { $dllSourceDirs.Add($dir) }
        }
    }
}
foreach ($p in @("$LibsKitRoot\bin", "$LibsKitRoot\lib")) {
    if (Test-Path -LiteralPath $p) { $dllSourceDirs.Add($p) }
}

$copied = 0
foreach ($dir in ($dllSourceDirs | Select-Object -Unique)) {
    $dlls = Get-ChildItem -LiteralPath $dir -Filter '*.dll' -File -ErrorAction SilentlyContinue
    foreach ($dll in $dlls) {
        Copy-Item -LiteralPath $dll.FullName -Destination $StageDir -Force
        $copied++
    }
}
Write-Host "  Copied $copied third-party DLL(s) from the scribus-libs kit."

#----------------------------------------------------------------------------
# Qt runtime deployment
#----------------------------------------------------------------------------
if (-not $SkipWindeploy -and $Script:WindeployAvailable) {
    Write-Host "== Deploying Qt runtime (windeployqt) ==" -ForegroundColor Cyan
    $windeploy = Join-Path $QtDir 'bin\windeployqt.exe'
    $exe = Join-Path $StageDir 'Apscribe.exe'
    $windeployArgs = @(
        '--release'
        '--no-system-d3d-compiler'
        '--dir', $StageDir
        '--plugindir', (Join-Path $StageDir 'qtplugins')
        "--compiler-runtime"
        $exe
    )
    & $windeploy $windeployArgs 2>&1 | ForEach-Object { Write-Host "  $_" }
    if ($LASTEXITCODE -ne 0) {
        throw "windeployqt failed with exit code $LASTEXITCODE."
    }
} else {
    Write-Host "  Skipped Qt deployment." -ForegroundColor Yellow
}

# Qt must know the private plugin directory before QApplication is created.
# Without this configuration a clean PC cannot locate qwindows.dll.
Copy-Item -LiteralPath (Join-Path $Sources 'win32\qt.conf') -Destination (Join-Path $StageDir 'qt.conf') -Force

#----------------------------------------------------------------------------
# Build the installer with NSIS
#----------------------------------------------------------------------------
if (-not $SkipNsis -and $Makensis) {
    Write-Host "== Building NSIS installer ==" -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $DistDir -Force | Out-Null
    Push-Location $ScriptDir
    try {
        & $Makensis "/DVERSION=$Version" 'Scribus.nsi'
        if ($LASTEXITCODE -ne 0) {
            throw "makensis failed with exit code $LASTEXITCODE."
        }
    }
    finally {
        Pop-Location
    }
    $installer = Join-Path $DistDir "Apscribe-$Version-Setup.exe"
    if (Test-Path -LiteralPath $installer) {
        $size = (Get-Item -LiteralPath $installer).Length
        Write-Host ""
        Write-Host "SUCCESS: $installer ($([math]::Round($size/1MB,1)) MB)" -ForegroundColor Green
    }
} else {
    Write-Host ""
    Write-Host "Staged application tree ready at: $StageDir" -ForegroundColor Green
    Write-Host "Run makensis Scribus.nsi in this directory to build the installer."
}
