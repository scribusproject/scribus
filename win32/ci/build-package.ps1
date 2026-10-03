#----------------------------------------------------------------------------
#  Scribus - Open Source Desktop Publishing
#  Copyright (C) 2026 The Scribus Team
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the Free
#  Software Foundation; either version 2 of the License, or (at your option)
#  any later version.
#----------------------------------------------------------------------------
#Requires -Version 5.1

<#
.SYNOPSIS
Builds and packages a native 64-bit Windows test release of Scribus.

.DESCRIPTION
This script is intended for a Visual Studio 2022 developer prompt or a
Windows CI runner. It downloads and verifies the official Scribus dependency
kit, builds Release|x64 dependencies and Scribus, deploys the runtime, creates
both the portable ZIP and NSIS installer, and validates the resulting package.
#>

[CmdletBinding()]
param(
    [string]$Sources,
    [string]$QtDir = $env:QT_ROOT_DIR,
    [string]$DependencyRoot = 'C:\scribus-deps',
    [string]$Version = '2.0.0',
    [ValidateRange(1, 8)]
    [int]$MaximumCpuCount = 2,
    [switch]$SkipDependencyBuild,
    [switch]$DependencyBuildOnly,
    [switch]$SkipApplicationBuild,
    [switch]$ApplicationBuildOnly,
    [switch]$SkipInstaller,
    [switch]$SkipSmokeTest,
    [switch]$TestInstaller
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 3.0

$DependencyArchiveName = 'scribus-1.7.x-libs-msvc-20260412.7z'
$DependencyArchiveUrl = 'https://downloads.sourceforge.net/project/scribus/scribus-libs/scribus-1.7.x/scribus-1.7.x-libs-msvc-20260412.7z'
$DependencyArchiveSha256 = 'a6bd40450a22415d26cc9b0bf4aeaf9f295cfcff2f8bf64e9c62289aa69fdf3b'
$DependencyKitName = 'scribus-1.7.x-libs-msvc'
$Toolset = 'v143'
$Configuration = 'Release'
$Platform = 'x64'

function Assert-LastExitCode([string]$Operation) {
    if ($LASTEXITCODE -ne 0) {
        throw "$Operation failed with exit code $LASTEXITCODE."
    }
}

function Find-Executable([string]$Name, [string[]]$Candidates = @()) {
    $command = Get-Command $Name -ErrorAction SilentlyContinue
    if ($command) { return $command.Source }
    foreach ($candidate in $Candidates) {
        if ($candidate -and (Test-Path -LiteralPath $candidate -PathType Leaf)) {
            return (Resolve-Path -LiteralPath $candidate).Path
        }
    }
    return $null
}

function Invoke-MSBuild(
    [string]$MSBuild,
    [string]$Solution,
    [string]$LogFile,
    [string[]]$AdditionalProperties = @()
) {
    $arguments = @(
        $Solution,
        '/t:Build',
        "/p:Configuration=$Configuration",
        "/p:Platform=$Platform",
        "/p:PlatformToolset=$Toolset",
        '/p:WindowsTargetPlatformVersion=10.0',
        "/p:CL_MPCount=$MaximumCpuCount",
        "/m:$MaximumCpuCount",
        '/nologo',
        '/verbosity:minimal',
        '/fl',
        "/flp:LogFile=$LogFile;Verbosity=normal"
    ) + $AdditionalProperties

    & $MSBuild @arguments
    Assert-LastExitCode "MSBuild ($Solution)"
}

if ([Environment]::OSVersion.Platform -ne [PlatformID]::Win32NT) {
    throw 'This script creates a native Windows application and must run on Windows.'
}

if (-not $Sources) {
    $Sources = Split-Path (Split-Path (Split-Path -Path $PSScriptRoot -Parent) -Parent) -Parent
}
if (-not (Test-Path -LiteralPath $Sources -PathType Container)) {
    throw "Source directory not found: $Sources"
}
$Sources = (Resolve-Path -LiteralPath $Sources).Path

if (-not $QtDir -or -not (Test-Path -LiteralPath $QtDir -PathType Container)) {
    throw 'Qt was not found. Pass -QtDir or set QT_ROOT_DIR to a Qt 6 MSVC 2022 x64 installation.'
}
$QtDir = (Resolve-Path -LiteralPath $QtDir).Path
if (-not (Test-Path -LiteralPath (Join-Path $QtDir 'bin\windeployqt.exe') -PathType Leaf)) {
    throw "windeployqt.exe is missing from Qt directory: $QtDir"
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$msbuild = Find-Executable 'MSBuild.exe' @(
    if (Test-Path -LiteralPath $vswhere -PathType Leaf) {
        & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
    }
)
if (-not $msbuild) {
    throw 'MSBuild.exe was not found. Install Visual Studio 2022 with Desktop development with C++.'
}

$sevenZip = Find-Executable '7z.exe' @(
    "$env:ProgramFiles\7-Zip\7z.exe",
    "${env:ProgramFiles(x86)}\7-Zip\7z.exe"
)
if (-not $sevenZip) {
    throw '7z.exe was not found. Install 7-Zip or add it to PATH.'
}

New-Item -ItemType Directory -Path $DependencyRoot -Force | Out-Null
$DependencyRoot = (Resolve-Path -LiteralPath $DependencyRoot).Path
$archive = Join-Path $DependencyRoot $DependencyArchiveName
$kitRoot = Join-Path $DependencyRoot $DependencyKitName
$dependencySolution = Join-Path $kitRoot 'scribus-libs-msvc2022.sln'
$dependencyStamp = Join-Path $kitRoot ".release-x64-$Toolset-$DependencyArchiveSha256.complete"
$sourceParent = [System.IO.Directory]::GetParent($Sources).FullName
$buildsRoot = Join-Path $sourceParent 'Scribus-builds'
$buildRoot = Join-Path $buildsRoot "Scribus-$Configuration-$Platform-$Toolset"
$logsRoot = Join-Path $buildsRoot 'logs'
New-Item -ItemType Directory -Path $logsRoot -Force | Out-Null

Write-Host '== Native Windows package build ==' -ForegroundColor Cyan
Write-Host "  Sources      : $Sources"
Write-Host "  Qt           : $QtDir"
Write-Host "  Dependencies : $kitRoot"
Write-Host "  MSBuild      : $msbuild"
Write-Host "  Output       : $buildRoot"

if (-not (Test-Path -LiteralPath $archive -PathType Leaf)) {
    Write-Host '== Downloading the official Scribus dependency kit ==' -ForegroundColor Cyan
    $curl = Find-Executable 'curl.exe'
    if (-not $curl) { throw 'curl.exe was not found.' }
    & $curl --fail --location --retry 5 --retry-delay 5 --output $archive $DependencyArchiveUrl
    Assert-LastExitCode 'Dependency kit download'
}

$actualHash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
if ($actualHash -ne $DependencyArchiveSha256) {
    Remove-Item -LiteralPath $archive -Force
    throw "Dependency kit checksum mismatch. Expected $DependencyArchiveSha256, received $actualHash. The invalid download was removed."
}
Write-Host "  Dependency archive SHA-256 verified: $actualHash" -ForegroundColor Green

if (-not (Test-Path -LiteralPath $dependencySolution -PathType Leaf)) {
    Write-Host '== Extracting dependencies ==' -ForegroundColor Cyan
    if (Test-Path -LiteralPath $kitRoot) {
        Remove-Item -LiteralPath $kitRoot -Recurse -Force
    }
    & $sevenZip x $archive "-o$DependencyRoot" -y
    Assert-LastExitCode 'Dependency kit extraction'
}
if (-not (Test-Path -LiteralPath $dependencySolution -PathType Leaf)) {
    throw "Dependency solution is missing after extraction: $dependencySolution"
}

if (-not $SkipDependencyBuild -and -not (Test-Path -LiteralPath $dependencyStamp -PathType Leaf)) {
    Write-Host '== Building Scribus dependencies (Release|x64, v143) ==' -ForegroundColor Cyan
    Invoke-MSBuild $msbuild $dependencySolution (Join-Path $logsRoot 'dependencies-release-x64.log')
    Set-Content -LiteralPath $dependencyStamp -Value @(
        "archive=$DependencyArchiveName",
        "sha256=$DependencyArchiveSha256",
        "configuration=$Configuration",
        "platform=$Platform",
        "toolset=$Toolset"
    ) -Encoding ASCII
} elseif ($SkipDependencyBuild) {
    Write-Host '  Dependency build skipped by request.' -ForegroundColor Yellow
} else {
    Write-Host '  Reusing the verified cached dependency build.' -ForegroundColor Green
}

if ($DependencyBuildOnly) {
    Write-Host 'Dependency build is complete.' -ForegroundColor Green
    exit 0
}

$appSolution = Join-Path $Sources 'win32\msvc2022\Scribus.sln'
$scribusExe = Join-Path $buildRoot 'Apscribe.exe'
if (-not $SkipApplicationBuild) {
    Write-Host '== Building Scribus (Release|x64, v143) ==' -ForegroundColor Cyan
    Invoke-MSBuild $msbuild $appSolution (Join-Path $logsRoot 'scribus-release-x64.log') @(
        "/p:SCRIBUS_LIB_ROOT=$kitRoot",
        "/p:QT6_DIR=$QtDir"
    )
} else {
    Write-Host '  Scribus application build skipped by request.' -ForegroundColor Yellow
}

if (-not (Test-Path -LiteralPath $scribusExe -PathType Leaf)) {
    throw "MSBuild completed but did not produce $scribusExe"
}

if ($ApplicationBuildOnly) {
    Write-Host "Scribus application build is complete: $scribusExe" -ForegroundColor Green
    exit 0
}

Write-Host '== Installing third-party runtimes into the build tree ==' -ForegroundColor Cyan
$copyDllScript = Join-Path $kitRoot 'copy-dlls-to-build-dir.bat'
Push-Location $kitRoot
try {
    & $copyDllScript $buildsRoot
    if ($LASTEXITCODE -ne 0) {
        Write-Warning "The dependency copy helper returned $LASTEXITCODE; package validation will determine whether required files are present."
    }
}
finally {
    Pop-Location
}

Write-Host '== Creating portable Windows package ==' -ForegroundColor Cyan
$portableScript = Join-Path $Sources 'win32\deploy\assemble.ps1'
& $portableScript -Sources $Sources -BuildRoot $buildRoot -QtDir $QtDir -LibsKitRoot $kitRoot -Version $Version -MakeZip
Assert-LastExitCode 'Portable package assembly'

$portableDir = Join-Path $Sources "win32\deploy\dist\Apscribe-$Version-win64"
$portableZip = Join-Path $Sources "win32\deploy\dist\Apscribe-$Version-win64.zip"
$installer = Join-Path $Sources "win32\installer\dist\Apscribe-$Version-Setup.exe"

if (-not $SkipInstaller) {
    Write-Host '== Creating NSIS installer ==' -ForegroundColor Cyan
    $installerScript = Join-Path $Sources 'win32\installer\deploy.ps1'
    & $installerScript -Sources $Sources -BuildRoot $buildRoot -QtDir $QtDir -LibsKitRoot $kitRoot -Version $Version
    Assert-LastExitCode 'Installer assembly'
}

Write-Host '== Validating Windows package ==' -ForegroundColor Cyan
$validator = Join-Path $PSScriptRoot 'validate-package.ps1'
$validationArguments = @{
    AppDir = $portableDir
    PortableZip = $portableZip
    Version = $Version
    SkipSmokeTest = $SkipSmokeTest
}
if (-not $SkipInstaller) {
    $validationArguments.Installer = $installer
    $validationArguments.TestInstaller = $TestInstaller
}
& $validator @validationArguments

Write-Host ''
Write-Host 'Native Windows test packages are ready:' -ForegroundColor Green
Write-Host "  $portableZip"
if (-not $SkipInstaller) { Write-Host "  $installer" }
