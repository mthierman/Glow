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

function Export-ReleaseNotes
{
    git --no-pager log -5 --oneline --no-decorate | Out-File "..\notes.txt"
}

function Get-Commit
{
    git rev-parse --short HEAD
}

function Compress-Repo
{
    Set-Location ..\
    C:\msys64\usr\bin\bsdtar.exe --exclude-vcs -cJf Glow.tar.xz Glow
}

function Get-Archive
{
    Get-Item "Glow.tar.xz"
}

function Get-ReleaseNotes
{
    Get-Item "notes.txt"
}
