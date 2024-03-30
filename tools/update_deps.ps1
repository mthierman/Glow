Push-Location
$libs = $PSScriptRoot | Split-Path | Join-Path -ChildPath "libs"
Set-Location $libs

gh release download -p "singleheader.zip" -R ada-url/ada --clobber
if (Test-Path "$libs/ada-url/ada") { Remove-Item -Path "$libs/ada-url/ada" -Recurse -Force }
[System.IO.Compression.ZipFile]::ExtractToDirectory("$libs/singleheader.zip", "$libs/ada-url/ada")
if (Test-Path "$libs/singleheader.zip") { Remove-Item -Path "$libs/singleheader.zip" -Recurse -Force }

gh release download -p "include.zip" -R nlohmann/json --clobber
if (Test-Path "$libs/nlohmann/json") { Remove-Item -Path "$libs/nlohmann/json" -Recurse -Force }
[System.IO.Compression.ZipFile]::ExtractToDirectory("$libs/include.zip", "$libs/nlohmann/json")
if (Test-Path "$libs/include.zip") { Remove-Item -Path "$libs/include.zip" -Recurse -Force }

$sqliteZip = ((Invoke-WebRequest "https://sqlite.org/download.html").ToString() | Select-String -Pattern 'sqlite-amalgamation-[0-9]+.zip' | Select-Object -First 1).Matches.Value
$outdir = $sqliteZip.TrimEnd(".zip")
Invoke-WebRequest "https://www.sqlite.org/2024/$sqliteZip" -OutFile "$libs/$sqliteZip"
[System.IO.Compression.ZipFile]::ExtractToDirectory("$libs/$sqliteZip", "$libs/sqlite-amalgamation")
Move-Item -Path "$libs/sqlite-amalgamation/$outdir" -Destination "$libs/$outdir"
if (Test-Path "$libs/sqlite-amalgamation") { Remove-Item "$libs/sqlite-amalgamation" -Force -Recurse }
if (Test-Path "$libs/$sqliteZip") { Remove-Item "$libs/$sqliteZip" -Force -Recurse }

Pop-Location
