function Enter-DevShell64
{
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch amd64 -Arch amd64 -SkipAutomaticLocation
}

function Enter-DevShell32
{
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch x86 -Arch x86 -SkipAutomaticLocation
}

function Build-CMake
{
    cmake --preset Release
    cmake --build --preset Release
}

function Compress-Repo
{
    Push-Location
    Set-Location ..\
    tar -cJf ..\Glow.tar.xz Glow
    Pop-Location
}

function Get-CompressedRepo
{
    Get-Item "Glow.tar.xz"
}

function Get-Version
{
    Get-Content "build\Glow_version.txt"
}

function Get-ReleaseNotes
{
    Get-Item "build\Glow_release_notes.txt"
}

function Get-Commit
{
    git rev-parse --short HEAD
}
