& .\pwsh.ps1

Invoke-DevShell
Invoke-CMake -ConfigurePreset 'CI' -BuildPreset 'CI'
Compress-Glow
# Publish-Glow
