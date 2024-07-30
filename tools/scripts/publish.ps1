param (
    [ValidateNotNullOrEmpty()]
    [string]$Branch = 'main'
)

$version = "v$((Get-Content "build/manifest.json" | ConvertFrom-Json).version)"
$notes = Get-Item "build/notes/release_notes"
$archive = Get-Item "build/Glow.zip"

gh release delete $version -y
gh release create $version $archive --notes-file $notes -t $version
