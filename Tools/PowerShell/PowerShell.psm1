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

function Invoke-CMake
{
    Enter-DevShell64
    cmake --preset Release
    cmake --build --preset Release
}

function Export-Notes
{
    git --no-pager log -5 --oneline --no-decorate | Out-File "..\notes.txt"
}

function Compress-Repo
{
    Push-Location
    Set-Location ..\
    C:\msys64\usr\bin\bsdtar.exe --exclude-vcs --exclude Glow/build --exclude Glow/.vscode -cJf Glow.tar.xz Glow
    Pop-Location
}

function Get-Commit
{
    git rev-parse --short HEAD
}

function Get-Name
{
    (Get-Content .\Glow.json | ConvertFrom-Json).name
}

function Get-Version
{
    (Get-Content .\Glow.json | ConvertFrom-Json).version
}

function Get-Archive
{
    Get-Item "..\Glow.tar.xz"
}

function Get-Notes
{
    Get-Item "..\notes.txt"
}

function Invoke-StableRelease
{
    Export-Notes
    Compress-Repo
    $version = "v$(Get-Version)"
    $archive = Get-Archive
    $notes = Get-Notes
    gh release delete $version -y
    gh release create $version $archive -F $notes -t "$version"
}

function Invoke-NextRelease
{
    Export-Notes
    Compress-Repo
    $archive = Get-Archive
    $notes = Get-Notes
    gh release delete Next -y
    gh release create Next $archive -F $notes -t "Next" -p
}
