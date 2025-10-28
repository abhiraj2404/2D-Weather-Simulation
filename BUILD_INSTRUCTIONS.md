# 🚨 BUILD INSTRUCTIONS

## ⚠️ Important: Use the Correct Terminal!

You **MUST** use **MSYS2 MINGW64** terminal to build, not Git Bash or regular command prompt.

---

## ✅ How to Build

### Step 1: Open MSYS2 MINGW64 Terminal

- Press **Windows Key**
- Type: **MSYS2 MINGW64**
- Click on **MSYS2 MINGW64** (purple icon)

### Step 2: Navigate to Project

```bash
cd /e/WORK/ColLegE/cgm/2
```

### Step 3: Build

```bash
./build_mingw64.sh
```

OR use the batch file:

```bash
./build.bat
```

### Step 4: Run

```bash
./weather_sim.exe
```

---

## 🎯 What to Expect

When you run `weather_sim.exe`, you should see:

**Console Output:**

```
GLFW initialized successfully
OpenGL Version: 4.x.x
GLAD initialized successfully
==================================
2D Weather Simulation Started!
Press ESC to exit
==================================
Weather changed to: CLOUDY
Weather changed to: RAINING
Weather changed to: THUNDERSTORM
...
```

**Visual Output:**

- Window appears (1280x720)
- **NEW: Dynamic day/night cycle** with beautiful sky color transitions!
- **NEW: Animated clouds** that drift across the sky
- **NEW: Rain particles** during rainy weather (diagonal streaks)
- **NEW: Snow particles** during snowy weather (gentle floating)
- Sky color **changes dynamically** based on weather AND time of day:
  - CLEAR: Bright blue (day), orange (dawn/dusk), dark blue (night)
  - CLOUDY: Grayer with visible clouds
  - RAINING: Dark gray with rain particles
  - THUNDERSTORM: Almost black with intense rain and lightning flashes!
  - SNOWING: Light gray with snow particles
- Weather states change every ~5 seconds
- Lightning flashes during thunderstorms
- Clouds change color and density based on weather

**Controls:**

- **ESC** = Exit
- **ImGui Panel** = Adjust weather parameters in real-time!

---

## ❌ If Build Fails

### Error: "undefined reference to glfwXXX"

**Problem:** You're using the wrong terminal (32-bit MinGW)

**Solution:**

1. Close current terminal
2. Open **MSYS2 MINGW64** (not Git Bash!)
3. Try again

### How to Check You're in the Right Terminal

```bash
gcc -dumpmachine
```

**Should show:** `x86_64-w64-mingw32`  
**Wrong if shows:** `mingw32` or `i686-w64-mingw32`

---

## 📊 Changes Made (Integration Complete!)

### **Phase 1: Basic Weather System** ✅

The following files were updated to integrate WeatherSystem:

#### `include/Application.h`

```cpp
+ #include "WeatherSystem.h"
+ WeatherSystem weatherSystem;  // Added member variable
```

#### `src/Application.cpp`

```cpp
+ weatherSystem()  // Initialize in constructor

+ // Update function now calls:
+ weatherSystem.update(deltaTime);
+ // Logs state changes to console

+ // Render function now:
+ // - Changes sky color based on weather state
+ // - Shows lightning flashes during thunderstorms
```

### **Phase 2: Visual Enhancements** ✅ **NEW!**

#### New Files Added:

- `include/Renderer.h` & `src/Renderer.cpp` - 2D rendering system
- `include/ParticleSystem.h` & `src/ParticleSystem.cpp` - Rain/snow particles
- `include/CloudSystem.h` & `src/CloudSystem.cpp` - Animated clouds

#### Enhanced Files:

- `include/WeatherSystem.h` - Added `getSkyColor()` for day/night cycle
- `src/WeatherSystem.cpp` - Dynamic sky color based on time + weather
- `include/Application.h` - Added ParticleSystem, CloudSystem, Renderer
- `src/Application.cpp` - Integrated all rendering systems
- `build_mingw64.sh` - Added new source files to compilation
- `build.bat` - Added new source files to compilation

#### What's New:

- 🌅 **Day/Night Cycle** - Sky changes from dawn to dusk to night
- ☁️ **Animated Clouds** - Fluffy clouds drift across the sky
- 🌧️ **Rain Particles** - Diagonal rain streaks during storms
- ❄️ **Snow Particles** - Gentle floating snowflakes
- ⚡ **Enhanced Lightning** - More dramatic flash effects
- 🎨 **Better Graphics** - OpenGL shader-based rendering

**Result:** Full 2D weather simulation with particles, clouds, and day/night cycle! 🎉

---

**Next:** Open MSYS2 MINGW64 and run `./build_mingw64.sh` to see it in action!

For detailed feature documentation, see **FEATURES.md**
