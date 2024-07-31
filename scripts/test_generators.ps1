param(
    [switch]$MSBuild
)

if ($MSBuild)
{
    cmake --preset "MSBuild MSVC"
    Measure-Command { cmake --build --preset "MSBuild MSVC Release" }
}
else
{
    cmake --preset "Ninja MSVC"
    Measure-Command { cmake --build --preset "Ninja MSVC Release" }
}
