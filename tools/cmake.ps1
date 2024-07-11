param (
    [ValidateSet('Development', 'CI')]
    [string]$ConfigurePreset = ($env:CI) ? 'CI' : 'Development',
    [ValidateSet('Debug', 'Release', 'CI')]
    [string]$BuildPreset = ($env:CI) ? 'CI' : 'Debug`'
)

cmake --preset $ConfigurePreset
cmake --build --preset $BuildPreset
