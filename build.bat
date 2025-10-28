@echo off
echo =========================================
echo  2D Weather Simulation - Build Script
echo =========================================
echo.

REM Use MinGW-w64 from MSYS2
set PATH=C:\msys64\mingw64\bin;%PATH%

REM Check if MinGW-w64 is available
where g++.exe >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: MinGW-w64 g++ not found!
    echo Please install MSYS2 and mingw-w64-x86_64-toolchain
    pause
    exit /b 1
)

echo Compiling sources...
g++ -o weather_sim.exe ^
    main.cpp ^
    src/Application.cpp ^
    src/WeatherSystem.cpp ^
    src/ParticleSystem.cpp ^
    src/CloudSystem.cpp ^
    src/LightningSystem.cpp ^
    src/CelestialSystem.cpp ^
    src/FogSystem.cpp ^
    src/Renderer.cpp ^
    src/glad.c ^
    src/imgui/imgui.cpp ^
    src/imgui/imgui_demo.cpp ^
    src/imgui/imgui_draw.cpp ^
    src/imgui/imgui_tables.cpp ^
    src/imgui/imgui_widgets.cpp ^
    src/imgui/imgui_impl_glfw.cpp ^
    src/imgui/imgui_impl_opengl3.cpp ^
    -I./include ^
    -I./include/imgui ^
    -std=c++17 -Wall ^
    -lopengl32 lib/libglfw3.a -lgdi32 -luser32 -lshell32 -lkernel32

if %ERRORLEVEL% EQU 0 (
    echo.
    echo =========================================
    echo  Build successful!
    echo =========================================
    echo.
    echo Run: weather_sim.exe
    echo.
) else (
    echo.
    echo =========================================
    echo  Build failed! Check errors above.
    echo =========================================
    echo.
    exit /b 1
)

pause
