param (
    [ValidateNotNullOrEmpty()]
    [string]$Branch = 'main'
)

$version = "v$((Get-Content "build/manifest.json" | ConvertFrom-Json).version)"
$notes = git --no-pager log -5 --oneline --no-decorate
$archive = Get-Item "build/Glow.zip"

gh release delete $version -y
gh release create $version $archive --notes-file $notes -t $version
