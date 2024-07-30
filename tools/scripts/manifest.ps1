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
