#----------------------------------------------------------------------------
#  Scribus - Open Source Desktop Publishing
#  Copyright (C) 2026 The Scribus Team
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#----------------------------------------------------------------------------
#Requires -Version 5.1

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$AppDir,
    [Parameter(Mandatory = $true)][string]$Sources
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 3.0
$app = Join-Path $AppDir 'Scribus.exe'
$publishScript = Join-Path $AppDir 'share\scripts\DataPublish.py'
if (-not (Test-Path -LiteralPath $app -PathType Leaf)) { throw "Missing packaged application: $app" }
if (-not (Test-Path -LiteralPath $publishScript -PathType Leaf)) { throw "Missing packaged publishing script: $publishScript" }

$outputRoot = Join-Path $env:RUNNER_TEMP 'scribus-phase5-tests'
New-Item -ItemType Directory -Path $outputRoot -Force | Out-Null

foreach ($test in @(
    @{ File = 'test_data_merge.py'; Marker = 'DATA_MERGE_TEST_PASSED'; ExitCode = 0 },
    @{ File = 'test_data_publish.py'; Marker = 'DATA_PUBLISH_TEST_PASSED'; ExitCode = 0 },
    @{ File = 'test_headless_script_failure.py'; Marker = 'HEADLESS_SCRIPT_FAILURE_EXPECTED'; ExitCode = 1 }
)) {
    $script = Join-Path $Sources "scribus\tests\scripts\$($test.File)"
    if (-not (Test-Path -LiteralPath $script -PathType Leaf)) { throw "Missing test: $script" }
    $startInfo = New-Object System.Diagnostics.ProcessStartInfo
    $startInfo.FileName = $app
    $startInfo.Arguments = '--no-splash --no-gui --python-script "' + $script + '"'
    $startInfo.WorkingDirectory = $AppDir
    $startInfo.UseShellExecute = $false
    $startInfo.CreateNoWindow = $true
    $startInfo.RedirectStandardOutput = $true
    $startInfo.RedirectStandardError = $true
    $startInfo.EnvironmentVariables.Remove('QT_PLUGIN_PATH')
    $startInfo.EnvironmentVariables.Remove('QT_QPA_PLATFORM_PLUGIN_PATH')
    $startInfo.EnvironmentVariables['QT_QPA_PLATFORM'] = 'windows'
    $startInfo.EnvironmentVariables['SCRIBUS_TEST_OUTPUT_DIR'] = $outputRoot

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $startInfo
    if (-not $process.Start()) { throw "Could not start $app" }
    try {
        $stdoutTask = $process.StandardOutput.ReadToEndAsync()
        $stderrTask = $process.StandardError.ReadToEndAsync()
        if (-not $process.WaitForExit(180000)) {
            $process.Kill()
            throw "$($test.File) timed out after 180 seconds"
        }
        $output = $stdoutTask.Result + [Environment]::NewLine + $stderrTask.Result
        Write-Host $output.Trim()
        if ($process.ExitCode -ne $test.ExitCode -or -not $output.Contains($test.Marker)) {
            throw "$($test.File) failed (exit code $($process.ExitCode); missing $($test.Marker))"
        }
    }
    finally {
        if (-not $process.HasExited) { $process.Kill() }
        $process.Dispose()
    }
}

Write-Host 'Phase 5 Windows Scripter runtime tests passed.' -ForegroundColor Green
