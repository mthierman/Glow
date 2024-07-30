if (Test-Path "build/Glow.zip") { Remove-Item "build/Glow.zip" -Force }
7z a build/Glow.zip * -xr'!build' -xr'!node_modules' -xr'!.git' -xr'!.vscode' -xr'!CMakeUserPresets.json'
