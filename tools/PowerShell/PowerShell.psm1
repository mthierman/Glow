function Invoke-DevShell
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Arch = 'amd64'
    )

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch $Arch -Arch $Arch -SkipAutomaticLocation
}

function Invoke-CMake
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$ConfigurePreset = ($env:CI) ? 'CI' : 'Default',
        [string]$BuildPreset = ($env:CI) ? 'CI' : 'Default'
    )

    cmake --preset $ConfigurePreset
    cmake --build --preset $BuildPreset
}

function Invoke-CTest
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$TestPreset = ($env:CI) ? 'CI' : 'Default',
        [switch]$JSON,
        [switch]$XML
    )

    $arguments = @("--preset", $TestPreset)

    if ($JSON)
    {
        $arguments += "--show-only=json-v1"
    }

    if ($XML)
    {
        $arguments += "--output-junit", "tests/log.xml"
    }

    &ctest @arguments
}

function Compress-Glow
{
    [CmdletBinding()]
    param ()

    if (Test-Path "build/Glow.zip") { Remove-Item "build/Glow.zip" -Force }
    7z a build/Glow.zip * -xr'!build' -xr'!node_modules' -xr'!.git' -xr'!.vscode' -xr'!CMakeUserPresets.json'
}

function Publish-Glow
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Branch = 'main'
    )

    # $hash = Get-Content "build/notes/short_hash"
    # $version = Get-Content "build/notes/version"
    # $notes = Get-Item "build/notes/release_notes"

    $version = "v$((Get-Content "build/manifest.json" | ConvertFrom-Json).version)"
    $notes = Get-Item "build/notes/release_notes"
    $archive = Get-Item "build/Glow.zip"

    gh release delete $version -y
    gh release create $version $archive --notes-file $notes -t $version
}
