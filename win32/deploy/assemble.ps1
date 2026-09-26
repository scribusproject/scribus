#----------------------------------------------------------------------------
#  Scribus 2.0 - Windows portable deployment assembler (PowerShell 5.1+)
#
#  Run ON THE WINDOWS MACHINE that holds the MSVC build of Scribus. Builds a
#  self-contained, copy-and-run application tree into win32\deploy\dist:
#
#      Scribus-2.0.0-win64\
#          Scribus.exe
#          *.dll                        (Qt6 + 3rd-party runtime DLLs)
#          qtplugins\                   (Qt platform/plugin DLLs, windeployqt)
#          plugins\                     (Scribus import/export + tool plugins)
#          libs\                        (scribus support dlls/lib)
#          share\                       (resources, icons, translations, fonts,
#                                        colour profiles, templates)
#
#  The whole folder can be zipped and copied verbatim to any 64-bit PC.
#
#  Usage:
#      powershell -ExecutionPolicy Bypass -File win32\deploy\assemble.ps1
#      powershell -ExecutionPolicy Bypass -File win32\deploy\assemble.ps1 `
#          -BuildRoot "D:\Scribus\Scribus-builds\Scribus-Release-x64-v143" `
#          -QtDir     "D:\Qt\6.11.2\msvc2022_64" `
#          -LibsKitRoot "D:\Scribus Libs\scribus-1.7.x-libs-msvc"
#----------------------------------------------------------------------------
[CmdletBinding()]
param(
    [string]$Sources,                   # Scribus source root (auto-detected)
    [string]$BuildRoot,                 # MSVC app output dir with Scribus.exe (auto-detected)
    [string]$Configuration = 'Release',
    [string]$Platform = 'x64',
    [string]$PlatformToolset,           # v143 = VS2022 (auto-detected)
    [string]$QtDir,                     # e.g. F:\Libraries\Qt\6.11.2\msvc2022_64
    [string]$LibsKitRoot,               # scribus-libs kit root (auto-detected)
    [string]$Version,                   # defaults to Scribus-version-infos.h
    [switch]$SkipWindeploy,             # skip Qt runtime deployment
    [switch]$MakeZip,                   # also create Scribus-<Version>-win64.zip
    [switch]$ZipOnly                    # refresh staged dist from existing app tree only
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
$DistDir     = Join-Path $ScriptDir 'dist'
$AppDir      = Join-Path $DistDir "Scribus-$Version-win64"

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
function Find-BuildRoot {
    $base = Join-Path $Sources 'Scribus-builds'
    if (-not (Test-Path -LiteralPath $base)) { return $null }
    $pattern = "Scribus-$Configuration-$Platform-*"
    $dirs = Get-ChildItem -LiteralPath $base -Directory -Filter $pattern -ErrorAction SilentlyContinue |
            Sort-Object LastWriteTime -Descending
    foreach ($d in $dirs) {
        if (Test-Path -LiteralPath (Join-Path $d.FullName 'Scribus.exe')) { return $d.FullName }
    }
    return $null
}
function Find-QtDir {
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
    $candidate = Join-Path (Split-Path -Parent $Sources) 'scribus-1.7.x-libs-msvc'
    if (Test-Path -LiteralPath $candidate) { return $candidate }
    return $null
}

#----------------------------------------------------------------------------
# Resolve configuration
#----------------------------------------------------------------------------
Write-Host "== Scribus portable Windows deployment ==" -ForegroundColor Cyan

if (-not $PlatformToolset) { $PlatformToolset = 'v143' }

if (-not $BuildRoot) {
    $BuildRoot = Find-BuildRoot
    if ($BuildRoot) { Write-Host "  Build output auto-detected: $BuildRoot" }
}
if (-not $BuildRoot) {
    throw "Could not locate the Scribus build output. Use -BuildRoot to specify the directory that contains Scribus.exe."
}
if (-not (Test-Path -LiteralPath (Join-Path $BuildRoot 'Scribus.exe'))) {
    throw "Scribus.exe not found in '$BuildRoot'. Build the Release|x64 configuration first."
}
$BuildRoot = (Resolve-Path -LiteralPath $BuildRoot).Path

if (-not $Version) {
    $vc = Read-TextFile $VersionFile
    $vm = [regex]::Match([string]$vc, 'SCRIBUS_FULL_VERSION_STRING\s+"([^"]+)"')
    if ($vm.Success) { $Version = $vm.Groups[1].Value }
}
if (-not $Version) { $Version = '2.0.0' }

$AppDir = Join-Path $DistDir "Scribus-$Version-win64"

if (-not $QtDir) { $QtDir = Find-QtDir }
if (-not $QtDir) {
    Write-Warning "Could not locate Qt; -SkipWindeploy implied. Set -QtDir (e.g. F:\Libraries\Qt\6.11.2\msvc2022_64)."
    $SkipWindeploy = $true
}
$WindeployAvailable = $false
if (-not $SkipWindeploy) {
    if (-not (Test-Path -LiteralPath (Join-Path $QtDir 'bin\windeployqt.exe'))) {
        Write-Warning "windeployqt.exe not found under '$QtDir\bin'. Qt deployment will be skipped."
        $SkipWindeploy = $true
    } else {
        $WindeployAvailable = $true
    }
}

if (-not $LibsKitRoot) { $LibsKitRoot = Find-LibsKitRoot }

Write-Host "  Sources        : $Sources"
Write-Host "  Build output   : $BuildRoot"
Write-Host "  Qt             : $QtDir"
Write-Host "  Scribus-libs   : $LibsKitRoot"
Write-Host "  Version        : $Version"
Write-Host "  Output         : $AppDir"

#----------------------------------------------------------------------------
# Stage the application tree
#----------------------------------------------------------------------------
if (-not $ZipOnly) {
    Write-Host "== Staging application tree ==" -ForegroundColor Cyan
    if (Test-Path -LiteralPath $AppDir) { Remove-Item -LiteralPath $AppDir -Recurse -Force }
    New-Item -ItemType Directory -Path $AppDir -Force | Out-Null

    Copy-Item -LiteralPath (Join-Path $BuildRoot 'Scribus.exe') -Destination $AppDir

    Get-ChildItem -LiteralPath $BuildRoot -Filter '*.dll' -File -ErrorAction SilentlyContinue |
        Copy-Item -Destination $AppDir

    foreach ($sub in @('libs', 'plugins', 'python', 'share')) {
        $src = Join-Path $BuildRoot $sub
        if (Test-Path -LiteralPath $src) {
            Copy-Item -LiteralPath $src -Destination $AppDir -Recurse
        }
    }

    # MSVC projects do not run CMake's install(FILES) for Scripter scripts.
    $scriptsDir = Join-Path $AppDir 'share\scripts'
    New-Item -ItemType Directory -Path $scriptsDir -Force | Out-Null
    Copy-Item -LiteralPath (Join-Path $Sources 'scribus\plugins\scriptplugin\scripts\DataPublish.py') -Destination $scriptsDir

    # Third-party DLLs from the scribus-libs kit.
    if ($LibsKitRoot) {
        $dllSourceDirs = New-Object System.Collections.Generic.List[string]
        $pathsProps = Join-Path $LibsKitRoot 'scribus-lib-paths.props'
        if (Test-Path -LiteralPath $pathsProps) {
            $props = Read-TextFile $pathsProps
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
                Copy-Item -LiteralPath $dll.FullName -Destination $AppDir -Force
                $copied++
            }
        }
        Write-Host "  Copied $copied third-party DLL(s) from the scribus-libs kit."
    } else {
        Write-Warning "scribus-libs kit not found; 3rd-party DLLs (cairo, harfbuzz, poppler, lcms2 ...) will be MISSING."
    }
}

#----------------------------------------------------------------------------
# Qt runtime deployment
#----------------------------------------------------------------------------
if (-not $SkipWindeploy) {
    Write-Host "== Deploying Qt runtime (windeployqt) ==" -ForegroundColor Cyan
    $windeploy = Join-Path $QtDir 'bin\windeployqt.exe'
    $exe = Join-Path $AppDir 'Scribus.exe'
    $windeployArgs = @(
        '--release'
        '--no-system-d3d-compiler'
        '--dir', $AppDir
        '--plugindir', (Join-Path $AppDir 'qtplugins')
        '--compiler-runtime'
        $exe
    )
    & $windeploy $windeployArgs 2>&1 | ForEach-Object { Write-Host "  $_" }
    if ($LASTEXITCODE -ne 0) {
        throw "windeployqt failed with exit code $LASTEXITCODE."
    }
} else {
    Write-Host "  Skipped Qt deployment." -ForegroundColor Yellow
}

# Qt reads qt.conf before Scribus can adjust QApplication's library paths.
# Keep the platform plugins in our private qtplugins directory so a deployed
# build does not depend on a Qt SDK being installed on the target machine.
Copy-Item -LiteralPath (Join-Path $Sources 'win32\qt.conf') -Destination (Join-Path $AppDir 'qt.conf') -Force

#----------------------------------------------------------------------------
# Zip
#----------------------------------------------------------------------------
if ($MakeZip) {
    Write-Host "== Creating zip archive ==" -ForegroundColor Cyan
    $zip = Join-Path $DistDir "Scribus-$Version-win64.zip"
    if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip -Force }
    Compress-Archive -Path $AppDir -DestinationPath $zip -CompressionLevel Optimal
    $size = (Get-Item -LiteralPath $zip).Length
    Write-Host "SUCCESS: $zip ($([math]::Round($size/1MB,1)) MB)" -ForegroundColor Green
} else {
    Write-Host "SUCCESS: portable deployment ready at $AppDir" -ForegroundColor Green
    Write-Host "Copy this folder to any 64-bit Windows PC and run Scribus.exe"
    if (-not $SkipWindeploy) {
        Write-Host "Re-run with -MakeZip to also create Scribus-$Version-win64.zip"
    }
}
