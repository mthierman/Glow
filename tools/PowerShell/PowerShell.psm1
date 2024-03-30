function Update-Dependencies
{
    if (!(Test-Path "libs")) { New-Item -ItemType Directory "libs" }
    
    if (Test-Path "libs/ada-url/ada") { Remove-Item -Path "libs/ada-url/ada" -Recurse -Force }
    if (Test-Path "libs/nlohmann/json") { Remove-Item -Path "libs/nlohmann/json" -Recurse -Force }

    gh release download -p "singleheader.zip" -R ada-url/ada --clobber
    [System.IO.Compression.ZipFile]::ExtractToDirectory("singleheader.zip", "libs/ada-url/ada")
    if (Test-Path "singleheader.zip") { Remove-Item -Path "singleheader.zip" -Recurse -Force }
    Invoke-WebRequest "https://raw.githubusercontent.com/ada-url/ada/main/LICENSE-MIT" -OutFile "libs/ada-url/ada/LICENSE-MIT"

    gh release download -p "include.zip" -R nlohmann/json --clobber
    [System.IO.Compression.ZipFile]::ExtractToDirectory("include.zip", "libs/nlohmann/json")
    if (Test-Path "include.zip") { Remove-Item -Path "include.zip" -Recurse -Force }

    $sqliteZip = ((Invoke-WebRequest "https://sqlite.org/download.html").ToString() | Select-String -Pattern 'sqlite-amalgamation-[0-9]+.zip' | Select-Object -First 1).Matches.Value
    $outdir = $sqliteZip.TrimEnd(".zip")
    Invoke-WebRequest "https://www.sqlite.org/2024/$sqliteZip" -OutFile "$sqliteZip"
    [System.IO.Compression.ZipFile]::ExtractToDirectory("$sqliteZip", "sqlite-temp")
    if (Test-Path "libs/sqlite") { Remove-Item "libs/sqlite" -Force -Recurse }
    Move-Item -Path "sqlite-temp/$outdir" -Destination "libs"
    Rename-Item -Path "libs/$outdir" -NewName "sqlite"
    if (Test-Path "sqlite-temp") { Remove-Item "sqlite-temp" -Force -Recurse }
    if (Test-Path "$sqliteZip") { Remove-Item "$sqliteZip" -Force -Recurse }
}

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

    if (Test-Path "build/Release/Glow.zip") { Remove-Item "build/Release/Glow.zip" -Force }
    7z a build/Release/Glow.zip * -xr'!build' -xr'!node_modules' -xr'!.git' -xr'!.vscode' -xr'!CMakeUserPresets.json'
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
