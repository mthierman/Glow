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
        [string]$TestPreset = 'CI'
    )

    ctest --preset $TestPreset
}

function Compress-Repo
{
    Push-Location
    Set-Location ..\
    C:\msys64\usr\bin\bsdtar.exe --exclude-vcs --exclude Glow/build --exclude Glow/.vscode -cJf Glow.tar.xz Glow
    Pop-Location
}

function Get-Archive
{
    Get-Item "..\Glow.tar.xz"
}
