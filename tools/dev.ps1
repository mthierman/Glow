param (
    [ValidateSet('x86', 'amd64', 'arm', 'arm64')]
    [string]$Arch = 'amd64',
    [ValidateSet('x86', 'amd64')]
    [string]$HostArch = 'amd64'
)

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vspath = & $vswhere -products * -latest -property installationPath
& "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -Arch $Arch -HostArch $HostArch -SkipAutomaticLocation
