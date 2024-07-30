function New-GlowManifest
{
    if (!(Test-Path "build")) { New-Item -ItemType Directory "build" }
    $manifest = Get-Content "package.json" | ConvertFrom-Json

([PSCustomObject]@{
        name        = $manifest.name
        description = $manifest.description
        version     = $manifest.version
        date        = Get-Date -Format "yyyy'/'MM'/'dd HH:mm:ss"
        hash        = $(git rev-parse HEAD)
        symbol      = "🌟"
        github      = "https://github.com/mthierman/Glow"
        download    = "https://github.com/mthierman/Glow/releases/download/v$($manifest.version)/Glow.zip"
    }) | ConvertTo-Json | Out-File -FilePath "build/manifest.json"
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

    $version = "v$((Get-Content "build/manifest.json" | ConvertFrom-Json).version)"
    $notes = Get-Item "build/notes/release_notes"
    $archive = Get-Item "build/Glow.zip"

    gh release delete $version -y
    gh release create $version $archive --notes-file $notes -t $version
}
