#!/bin/bash
# Build script for MSYS2 MINGW64 terminal
# This ensures the correct compiler is used

echo "========================================="
echo " 2D Weather Simulation - Build Script"
echo "========================================="
echo

# Check if we're using the correct compiler
GCC_TARGET=$(gcc -dumpmachine)
echo "Compiler target: $GCC_TARGET"

if [[ "$GCC_TARGET" != *"x86_64-w64-mingw32"* ]]; then
    echo
    echo "⚠️  WARNING: You're not using MinGW-w64!"
    echo "Current: $GCC_TARGET"
    echo "Expected: x86_64-w64-mingw32"
    echo
    echo "Please open: MSYS2 MINGW64 terminal"
    echo "(NOT regular MSYS2 or Git Bash)"
    echo
    read -p "Press Enter to exit..."
    exit 1
fi

echo "✅ Correct compiler detected"
echo

# Build the project
echo "Compiling sources..."
g++ -o weather_sim.exe \
    main.cpp \
    src/Application.cpp \
    src/WeatherSystem.cpp \
    src/ParticleSystem.cpp \
    src/CloudSystem.cpp \
    src/LightningSystem.cpp \
    src/CelestialSystem.cpp \
    src/FogSystem.cpp \
    src/Renderer.cpp \
    src/glad.c \
    src/imgui/imgui.cpp \
    src/imgui/imgui_demo.cpp \
    src/imgui/imgui_draw.cpp \
    src/imgui/imgui_tables.cpp \
    src/imgui/imgui_widgets.cpp \
    src/imgui/imgui_impl_glfw.cpp \
    src/imgui/imgui_impl_opengl3.cpp \
    -I./include \
    -I./include/imgui \
    -std=c++17 -Wall \
    lib/libglfw3dll.a -lopengl32 -lgdi32 -luser32 -lshell32 -lkernel32

if [ $? -eq 0 ]; then
    echo
    echo "========================================="
    echo " ✅ Build successful!"
    echo "========================================="
    echo
    echo "Run: ./weather_sim.exe"
    echo
else
    echo
    echo "========================================="
    echo " ❌ Build failed! Check errors above."
    echo "========================================="
    echo
    exit 1
fi
