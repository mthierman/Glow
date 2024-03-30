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
        [string]$ConfigurePreset = 'CI',
        [string]$BuildPreset = 'CI'
    )

    cmake --preset $ConfigurePreset
    cmake --build --preset $BuildPreset
}

function Invoke-CTest
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$TestPreset = 'CI',
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
    param (
        [ValidateNotNullOrEmpty()]
        [switch]$CI
    )

    if ($CI)
    {
        C:\msys64\usr\bin\bsdtar.exe --exclude-vcs --exclude=.vscode --exclude=build --exclude=node_modules --exclude=CMakeUserPresets.json -cJf build/Release/Glow.tar.xz .
    }

    else { bsdtar --exclude-vcs --exclude=.vscode --exclude=build --exclude=node_modules --exclude=CMakeUserPresets.json -cJf build/Release/Glow.tar.xz . }
}

function Publish-Glow
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Branch = 'main'
    )

    # $hash = Get-Content "build/notes/short_hash"
    $version = Get-Content "build/notes/version"
    $notes = Get-Item "build/notes/release_notes"
    $archive = Get-Item "build/Release/Glow.tar.xz"

    gh release delete $version -y
    gh release create $version $archive --notes-file $notes -t $version
}
