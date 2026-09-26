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

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$AppDir,
    [Parameter(Mandatory = $true)]
    [string]$PortableZip,
    [string]$Installer,
    [string]$Version = '2.0.0',
    [switch]$SkipSmokeTest,
    [switch]$TestInstaller
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 3.0

function Assert-File([string]$Root, [string]$RelativePath) {
    $path = Join-Path $Root $RelativePath
    if (-not (Test-Path -LiteralPath $path -PathType Leaf)) {
        throw "Required package file is missing: $RelativePath"
    }
}

function Assert-Directory([string]$Root, [string]$RelativePath) {
    $path = Join-Path $Root $RelativePath
    if (-not (Test-Path -LiteralPath $path -PathType Container)) {
        throw "Required package directory is missing: $RelativePath"
    }
}

function Assert-X64PE([string]$Path) {
    $stream = [System.IO.File]::OpenRead($Path)
    $reader = New-Object System.IO.BinaryReader($stream)
    try {
        if ($reader.ReadUInt16() -ne 0x5A4D) { throw "$Path is not a PE executable." }
        $stream.Position = 0x3C
        $peOffset = $reader.ReadInt32()
        $stream.Position = $peOffset
        if ($reader.ReadUInt32() -ne 0x00004550) { throw "$Path has an invalid PE header." }
        $machine = $reader.ReadUInt16()
        if ($machine -ne 0x8664) {
            throw ("$Path is not an x64 executable (PE machine 0x{0:X4})." -f $machine)
        }
    }
    finally {
        $reader.Dispose()
        $stream.Dispose()
    }
}

function Invoke-ProcessWithTimeout([string]$FileName, [string]$Arguments, [int]$TimeoutSeconds = 60) {
    $startInfo = New-Object System.Diagnostics.ProcessStartInfo
    $startInfo.FileName = $FileName
    $startInfo.Arguments = $Arguments
    $startInfo.WorkingDirectory = [System.IO.Path]::GetDirectoryName($FileName)
    $startInfo.UseShellExecute = $false
    $startInfo.CreateNoWindow = $true
    $startInfo.RedirectStandardOutput = $true
    $startInfo.RedirectStandardError = $true

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $startInfo
    if (-not $process.Start()) { throw "Could not start $FileName" }
    if (-not $process.WaitForExit($TimeoutSeconds * 1000)) {
        try { $process.Kill() } catch {}
        throw "$FileName did not exit within $TimeoutSeconds seconds."
    }
    $stdout = $process.StandardOutput.ReadToEnd()
    $stderr = $process.StandardError.ReadToEnd()
    if ($stdout) { Write-Host $stdout.Trim() }
    if ($stderr) { Write-Host $stderr.Trim() }
    if ($process.ExitCode -ne 0) {
        throw "$FileName exited with code $($process.ExitCode)."
    }
}

function Invoke-GuiStartupSmokeTest([string]$FileName, [int]$StartupSeconds = 10) {
    $appRoot = [System.IO.Path]::GetDirectoryName($FileName)
    $expectedPlatformPlugin = [System.IO.Path]::GetFullPath((Join-Path $appRoot 'qtplugins\platforms\qwindows.dll'))
    $startInfo = New-Object System.Diagnostics.ProcessStartInfo
    $startInfo.FileName = $FileName
    $startInfo.Arguments = '--no-splash'
    $startInfo.WorkingDirectory = $appRoot
    $startInfo.UseShellExecute = $false
    $startInfo.CreateNoWindow = $true
    $startInfo.EnvironmentVariables.Remove('QT_PLUGIN_PATH')
    $startInfo.EnvironmentVariables.Remove('QT_QPA_PLATFORM_PLUGIN_PATH')
    $startInfo.EnvironmentVariables['QT_QPA_PLATFORM'] = 'windows'

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $startInfo
    if (-not $process.Start()) { throw "Could not start $FileName" }
    try {
        if ($process.WaitForExit($StartupSeconds * 1000)) {
            throw "$FileName exited during the GUI startup smoke test with code $($process.ExitCode)."
        }
        $loadedPlatformPlugins = @($process.Modules | Where-Object { $_.ModuleName -ieq 'qwindows.dll' })
        $packagedPluginLoaded = @($loadedPlatformPlugins | Where-Object {
            [string]::Equals(
                [System.IO.Path]::GetFullPath($_.FileName),
                $expectedPlatformPlugin,
                [System.StringComparison]::OrdinalIgnoreCase
            )
        }).Count -gt 0
        if (-not $packagedPluginLoaded) {
            $loadedPaths = @($loadedPlatformPlugins | ForEach-Object { $_.FileName }) -join ', '
            throw "Scribus did not load its packaged Qt platform plugin. Loaded qwindows.dll paths: $loadedPaths"
        }
        Write-Host "  GUI process remained healthy for $StartupSeconds seconds and loaded $expectedPlatformPlugin."
    }
    finally {
        if (-not $process.HasExited) {
            try {
                $process.Kill()
                $process.WaitForExit()
            }
            catch {}
        }
        $process.Dispose()
    }
}

if (-not (Test-Path -LiteralPath $AppDir -PathType Container)) {
    throw "Portable application directory not found: $AppDir"
}
$AppDir = (Resolve-Path -LiteralPath $AppDir).Path

$requiredFiles = @(
    'Scribus.exe',
    'Qt6Core.dll',
    'Qt6Core5Compat.dll',
    'Qt6Gui.dll',
    'Qt6Network.dll',
    'Qt6PrintSupport.dll',
    'Qt6Svg.dll',
    'Qt6Widgets.dll',
    'Qt6Xml.dll',
    'qt.conf',
    'qtplugins\platforms\qwindows.dll',
    'cairo2.dll',
    'freetype.dll',
    'harfbuzz.dll',
    'iconv.dll',
    'icudt78.dll',
    'icuin78.dll',
    'icuuc78.dll',
    'libjpeg9f.dll',
    'liblzma.dll',
    'libpng16.dll',
    'libtiff5.dll',
    'libxml2.dll',
    'libcrypto-3-x64.dll',
    'libssl-3-x64.dll',
    'podofo.dll',
    'zlib1.dll',
    'python\python313.dll',
    'plugins\importpm.dll',
    'share\doc\COPYING'
)
foreach ($file in $requiredFiles) { Assert-File $AppDir $file }

$qtConfig = Get-Content -LiteralPath (Join-Path $AppDir 'qt.conf') -Raw
if ($qtConfig -notmatch '(?mi)^\s*Prefix\s*=\s*\.\s*$') {
    throw 'qt.conf does not use the application directory as its Qt prefix.'
}
if ($qtConfig -notmatch '(?mi)^\s*Plugins\s*=\s*qtplugins\s*$') {
    throw 'qt.conf does not direct Qt to the packaged qtplugins directory.'
}

foreach ($directory in @(
    'plugins',
    'python\lib',
    'share\poppler',
    'share\icons',
    'share\templates'
)) {
    Assert-Directory $AppDir $directory
}

$pluginCount = @(Get-ChildItem -LiteralPath (Join-Path $AppDir 'plugins') -Filter '*.dll' -File -Recurse).Count
if ($pluginCount -lt 10) {
    throw "Only $pluginCount Scribus plugin DLL(s) were packaged; at least 10 were expected."
}

$debugRuntimePatterns = @(
    '^Qt6.+d\.dll$',
    '^(freetype|harfbuzz|iconv|libjpeg9f|liblzma|libpng16|libtiff5|libxml2|podofo|zlib1)_d\.dll$'
)
$debugDlls = Get-ChildItem -LiteralPath $AppDir -Filter '*.dll' -File -Recurse | Where-Object {
    $name = $_.Name
    @($debugRuntimePatterns | Where-Object { $name -match $_ }).Count -gt 0
}
if ($debugDlls) {
    throw "Debug DLLs were mixed into the Release package: $($debugDlls.Name -join ', ')"
}

$scribusExe = Join-Path $AppDir 'Scribus.exe'
Assert-X64PE $scribusExe

if (-not (Test-Path -LiteralPath $PortableZip -PathType Leaf)) {
    throw "Portable ZIP not found: $PortableZip"
}
if ((Get-Item -LiteralPath $PortableZip).Length -lt 1MB) {
    throw "Portable ZIP is unexpectedly small: $PortableZip"
}

if (-not $SkipSmokeTest) {
    Write-Host '  Running portable GUI startup smoke test...'
    Invoke-GuiStartupSmokeTest $scribusExe
}

if ($Installer) {
    if (-not (Test-Path -LiteralPath $Installer -PathType Leaf)) {
        throw "Installer not found: $Installer"
    }
    if ((Get-Item -LiteralPath $Installer).Length -lt 1MB) {
        throw "Installer is unexpectedly small: $Installer"
    }
}

if ($TestInstaller) {
    if (-not $Installer) { throw '-TestInstaller requires -Installer.' }
    $testBase = if ($env:RUNNER_TEMP) { $env:RUNNER_TEMP } else { [System.IO.Path]::GetTempPath() }
    $installRoot = Join-Path $testBase "scribus-$Version-installer-smoke"
    if (Test-Path -LiteralPath $installRoot) {
        Remove-Item -LiteralPath $installRoot -Recurse -Force
    }
    Write-Host "  Silently installing into $installRoot..."
    try {
        Invoke-ProcessWithTimeout $Installer "/S /D=$installRoot" 180
        $installedExe = Join-Path $installRoot 'Scribus.exe'
        Assert-File $installRoot 'Scribus.exe'
        Assert-File $installRoot 'python\python313.dll'
        if (-not $SkipSmokeTest) {
            Write-Host '  Running installed GUI startup smoke test...'
            Invoke-GuiStartupSmokeTest $installedExe
        }
        $uninstaller = Join-Path $installRoot 'uninst.exe'
        Assert-File $installRoot 'uninst.exe'
        Write-Host '  Running silent uninstall...'
        Invoke-ProcessWithTimeout $uninstaller '/S' 180
        for ($attempt = 0; $attempt -lt 20 -and (Test-Path -LiteralPath $installRoot); $attempt++) {
            Start-Sleep -Milliseconds 500
        }
    }
    finally {
        if (Test-Path -LiteralPath $installRoot) {
            try {
                Remove-Item -LiteralPath $installRoot -Recurse -Force
            }
            catch {
                Write-Warning "Could not remove installer smoke-test directory '$installRoot': $($_.Exception.Message)"
            }
        }
    }
}

Write-Host "SUCCESS: validated x64 package with $pluginCount Scribus plugins and the bundled Python runtime." -ForegroundColor Green
