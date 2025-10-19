# 2D Weather Simulation# 2D Weather Simulation# 2D Weather Simulation

An advanced 2D weather simulation built with C++ and OpenGL featuring dynamic weather states, particle systems, and celestial rendering.An advanced 2D weather simulation built with C++ and OpenGL, featuring procedural generation, shader-based visual effects, and deep systemic interactions.A technically impressive 2D weather simulation built with C++ and OpenGL, featuring procedural generation, shader-based visual effects, and deep systemic interactions.

## 🚀 Quick Start## 🚀 Quick Start## Project Structure

### Build (MSYS2 MINGW64)### Build```

`````bash

cd /e/WORK/ColLegE/cgm/2````bash2/

./build_mingw64.sh

```build.bat├── main.cpp                  # Main application entry point



### Run```├── build.bat                 # Build script for Windows

```bash

./weather_sim.exe├── include/                  # Header files and external libraries

`````

### Run├── lib/ # Library files (.lib, .a)

Press **ESC** to exit.

`````bash├── src/                      # Source files (.cpp)

---

weather_sim.exe├── shaders/                  # GLSL shader files

## 🎨 Features

```└── WeatherSimulationPlan.md  # Comprehensive project plan

### Weather Systems

- ✅ **5 Weather States**: Clear, Cloudy, Raining, Thunderstorm, Snowing````

- ✅ **Dynamic Particles**: Rain and snow with wind effects

- ✅ **Animated Clouds**: Procedurally generated, wind-driven cloudsPress **ESC** to exit the application.

- ✅ **Branching Lightning**: Recursive lightning bolts during storms

- ✅ **Atmospheric Fog**: Humidity and weather-based fog layers## Prerequisites



### Celestial System---

- ✅ **Sun/Moon**: Realistic arc trajectories across the sky

- ✅ **100 Stars**: Twinkling stars visible at nightBefore building, you need to set up the following libraries:

- ✅ **Day/Night Cycle**: Dynamic sky colors based on time

## 📁 Project Structure

### Interactive Controls

- ✅ **Manual Weather Control**: Instant weather state changes### Required Libraries

- ✅ **Parameter Sliders**: Temperature, pressure, humidity, cloud cover, time

- ✅ **Quick Presets**: Summer Day, Winter Night, Stormy Evening, Foggy Dawn```

- ✅ **Trigger Effects**: Manual lightning and special effects

2/1. **GLFW** - Windowing and input

---

├── main.cpp                  # Entry point2. **GLAD** - OpenGL loader

## 📁 Project Structure

├── build.bat                 # Build script (requires MinGW-w64)3. **GLM** - Mathematics library

`````

2/├── weather_sim.exe # Compiled executable4. **Dear ImGui** - UI framework (to be added)

├── main.cpp # Entry point

├── build_mingw64.sh # Build script (MSYS2 MINGW64)├── include/5. **irrKlang** - Audio engine (optional, for polish)

├── build.bat # Build script (Windows)

├── BUILD_INSTRUCTIONS.md # Detailed build guide│ ├── Application.h # Main application class6. **libcurl** - HTTP requests (optional, for API integration)

├── include/ # Header files

│ ├── Application.h│ ├── WeatherSystem.h # Weather state machine7. **nlohmann/json** - JSON parsing (optional, for API integration)

│ ├── WeatherSystem.h

│ ├── ParticleSystem.h│ ├── GLFW/ # Window/input library headers

│ ├── CloudSystem.h

│ ├── LightningSystem.h│ ├── glad/ # OpenGL loader headers### Quick Setup for GLFW and GLAD

│ ├── CelestialSystem.h

│ ├── FogSystem.h│ ├── KHR/ # OpenGL platform headers

│ ├── Renderer.h

│ └── [GLFW, GLAD, GLM, ImGui]│ └── glm/ # Math library headers1. **Download GLFW:**

├── src/ # Implementation files

│ ├── Application.cpp├── lib/

│ ├── WeatherSystem.cpp

│ ├── ParticleSystem.cpp│ └── libglfw3.a # GLFW static library - Visit: https://www.glfw.org/download.html

│ ├── CloudSystem.cpp

│ ├── LightningSystem.cpp├── src/ - Download the Windows pre-compiled binaries (64-bit)

│ ├── CelestialSystem.cpp

│ ├── FogSystem.cpp│ ├── Application.cpp # Application implementation - Extract and copy:

│ ├── Renderer.cpp

│ ├── glad.c│ ├── WeatherSystem.cpp # Weather system logic - `include/GLFW/` folder → `include/`

│ └── imgui/

└── lib/ # Libraries│ └── glad.c # OpenGL loader implementation - `lib-mingw-w64/libglfw3.a` → `lib/`

    └── libglfw3dll.a

````├── shaders/                  # GLSL shaders (future)



---└── WeatherSimulationPlan.md  # Complete technical specification2. **Generate GLAD:**



## 🔧 Requirements```



- **MSYS2 MINGW64** (64-bit)- Visit: https://glad.dav1d.de/

- **GLFW 3.3+** ✅ Included

- **GLAD** (OpenGL 3.3 Core) ✅ Included--- - Set: Language=C/C++, gl=Version 3.3, Profile=Core

- **GLM** (Math library) ✅ Included

- **Dear ImGui** ✅ Included- Generate and download



---## 🔧 Requirements - Extract and copy:



## 🎮 Controls     - `include/glad/` and `include/KHR/` folders → `include/`



### Keyboard- **MinGW-w64** (64-bit GCC) - Required for compilation - `src/glad.c` → compile or add to your project

- **ESC** - Exit application

- **GLFW 3.3+** ✅ Included

### UI Panel

- **Weather Buttons** - Instantly change weather state- **GLAD** (OpenGL 3.3 Core) ✅ Included3. **Download GLM:**

- **Parameter Sliders** - Fine-tune weather variables

- **Quick Presets** - One-click weather scenarios- **GLM** (Math library) ✅ Included - Visit: https://github.com/g-truc/glm/releases

- **Manual Effects** - Trigger lightning and effects

  - Download and extract

---

--- - Copy `glm/` folder → `include/`

## 🎯 Technical Details

## 📊 Development Progress## Building the Project

### Technologies

- **Language**: C++17### ✅ Milestone 1: Canvas (COMPLETE)### Windows (MinGW)

- **Graphics**: OpenGL 3.3 Core

- **Windowing**: GLFW 3.4- GLFW window creation (800x600)

- **UI**: Dear ImGui

- **Math**: GLM- OpenGL 3.3 Core context```bash



### Rendering Systems- Main render loop with delta timebuild.bat

1. **Renderer** - Custom 2D shader-based rendering

2. **ParticleSystem** - Up to 1000 particles (rain/snow)- Input handling (ESC to exit)```

3. **CloudSystem** - Dynamic cloud generation and movement

4. **LightningSystem** - Recursive branching algorithm### ✅ Milestone 2: Core Architecture (COMPLETE)### Manual Build

5. **CelestialSystem** - Sun/moon trajectories and stars

6. **FogSystem** - Layered atmospheric effects- **Application Class**



### Performance  - GLFW/GLAD initialization```bash

- **Target**: 60 FPS

- **Particles**: Up to 1000  - Window managementg++ -o weather_sim.exe main.cpp -I./include -L./lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -std=c++17

- **Clouds**: Up to 15

- **Memory**: ~50 MB  - Event callbacks```



---  - Render loop



## 📖 Documentation  ## Running the Project



See **BUILD_INSTRUCTIONS.md** for detailed build steps and troubleshooting.- **WeatherSystem Class**



---  - State machine: CLEAR → CLOUDY → RAINING → THUNDERSTORM → SNOWING```bash



## 🎓 Course Project  - Atmospheric variables: temperature, pressure, wind, humidity, cloudCoverweather_sim.exe



Created for **Computer Graphics and Multimedia** course.  - Pressure-based state transitions (checked every 5 seconds)```



Demonstrates mastery of:  - Temperature determines rain vs snow (< 0°C = snow)

- Graphics primitives and OpenGL rendering pipeline

- 2D transformations and particle systems  - Lightning effect system with intensity decayPress **ESC** to exit.

- Color models and procedural generation

- Shader-based visual effects  - Time of day progression (0-24 hour cycle)

- System integration and OOP design

## Development Milestones

---

### ⏳ Next: Milestone 3 - UI Integration

**Author:** Rohan Duby

**Date:** October 2025  - Dear ImGui control panel- [x] **Milestone 1:** The Canvas - Basic GLFW window with blue sky

**Compiler:** MinGW-w64 GCC

- Real-time weather variable sliders- [ ] **Milestone 2:** The Control Panel - ImGui integration

- State visualization- [ ] **Milestone 3:** The Simulation Brain & First Raindrop - CPU particle system

- Time controls- [ ] **Milestone 4:** The Generative Sky - Procedural shaders

- [ ] **Milestone 5:** Polish & Atmosphere - Audio and effects

---- [ ] **Milestone 6:** The Killer Feature - Real-world API integration

- [ ] **Milestone 7:** Optimization - GPU particle system (optional)

## 🎯 Technical Details

## Current Status

### Libraries & Technologies

| Library | Purpose | Version |**Milestone 1-2 Architecture Complete!** ✓

|---------|---------|---------|

| **GLFW** | Window creation, input handling | 3.4 |### ✅ Completed

| **GLAD** | OpenGL function loading | GL 3.3 Core |

| **GLM** | Vector/matrix mathematics | Latest |- Application class with GLFW/GLAD initialization

| **OpenGL** | Graphics rendering | 3.3 Core Profile |- Main render loop with delta time calculation

| **C++17** | Modern C++ features | Standard |- Input handling and callback system

- WeatherSystem class with state machine

### Architecture Highlights- State transitions: CLEAR → CLOUDY → RAINING → THUNDERSTORM → SNOWING

- **OOP Design**: Clean separation between Application and WeatherSystem- Pressure-based probability transitions

- **State Machine Pattern**: Weather transitions based on atmospheric physics- Lightning effect system

- **Event-Driven**: GLFW callbacks for window/input events- Time of day progression

- **Delta Time**: Frame-rate independent updates

### ⚠️ CRITICAL: Setup Required Before Building

### Weather System Logic

```You **MUST** complete these steps before running `build.bat`:

Temperature: -20°C to 40°C

Pressure: 980 to 1030 hPa1. **Generate GLAD** (Most Important!)

Cloud Cover: 0.0 to 1.0

Wind: vec2 (direction + speed) - Visit: https://glad.dav1d.de/

Humidity: 0% to 100% - Settings: OpenGL 3.3 Core Profile

- Download and extract `src/glad.c` → place in `src/` folder

State Transitions: - Extract `include/glad/` and `include/KHR/` → place in `include/` folder

- Low pressure (< 1000 hPa) → Higher rain probability

- Temperature < 0°C → RAINING becomes SNOWING2. **Install GLFW**

- High pressure → Tends toward CLEAR

- Random lightning in THUNDERSTORM state - Extract `libglfw3.a` → place in `lib/` folder

```  - Extract`include/GLFW/`→ place in`include/GLFW/` folder

---3. **Install GLM**

- Extract `glm/` folder → place in `include/glm/` folder

## 📖 Documentation

### 📁 Current File Structure

- **WeatherSimulationPlan.md** - Complete technical specification

  - Detailed milestone breakdown```

  - Implementation strategies✅ include/Application.h - Main app class (complete)

  - Shader techniques (Perlin noise, fbm)✅ include/WeatherSystem.h - Weather state machine (complete)

  - Particle system design✅ src/Application.cpp - App implementation (complete)

  - Future features (API integration, GPU particles)✅ src/WeatherSystem.cpp - Weather logic (complete)

✅ main.cpp - Entry point (complete)

---✅ build.bat - Updated for multi-file build

⏳ src/glad.c - MUST ADD (critical blocker)

## 🎓 Course Alignment⏳ lib/libglfw3.a - MUST ADD (critical blocker)

⏳ include/glm/ - MUST ADD (critical blocker)

**Computer Graphics and Multimedia (L-T-P-C: 2-1-0-3)**```

Demonstrates mastery of:## Next Steps

- **Graphics Primitives** - OpenGL rendering pipeline

- **2D Transformations** - Particle movement, shader effects1. **COMPLETE GLAD SETUP** (critical blocker for compilation)

- **Color Models** - Procedural sky generation2. **Test Build** - Run `build.bat` to verify blue window appears

- **Advanced Rendering** - Shader-based visual effects3. **Add Dear ImGui** - Milestone 3 for control panel UI

- **System Integration** - Multiple interacting components4. **Implement ShaderManager** - For procedural sky rendering

### Why This Project Stands Out## Course Alignment

1. **Real Simulation** - Physics-based state transitions, not random effects

2. **Modern OpenGL** - Core Profile 3.3 with shaders, not legacy fixed pipelineThis project demonstrates mastery of key concepts from the Computer Graphics course:

3. **Procedural Generation** - Sky and effects generated algorithmically

4. **Professional Architecture** - Clean OOP design, industry patterns- **Unit 1:** Graphics primitives and OpenGL

5. **Technical Depth** - Goes beyond basic requirements- **Unit 2:** Attributes and rendering pipeline

- **Unit 3:** 2D transformations and viewing

---- **Unit 6:** Color models and advanced rendering

## 🔮 Future MilestonesFor detailed technical information, see `WeatherSimulationPlan.md`.

- [ ] **Milestone 3:** Dear ImGui UI - Control panel with sliders## Notes

- [ ] **Milestone 4:** Procedural Sky - Shader-based atmosphere rendering

- [ ] **Milestone 5:** Particle System - CPU-based rain/snow particles- This is a **simulation**, not just an animation

- [ ] **Milestone 6:** Polish - Audio, effects, optimization- Focus on procedural generation and systemic interaction

- [ ] **Milestone 7:** Advanced - GPU particles, API integration- Shader-based visual effects over simple 3D models

- Aim for technical depth and polish

---

---

## 👨‍💻 Development Notes

**Author:** Created for Computer Graphics and Multimedia course

**Current Status:** ✅ Foundation complete, ready for feature development**Date:** October 2025

**Build Time:** < 5 seconds
**Executable Size:** ~426 KB
**Memory Usage:** Minimal (< 50 MB)

**Compiler Warnings:** Non-critical initialization order warnings (can be fixed)

---

**Created for:** Computer Graphics and Multimedia Course
**Date:** October 2025
**Objective:** Create a standout technical project demonstrating advanced CG concepts
````

## Contributors & Work Division

- rohan099dubey (2025-10-08 → 2025-10-11)

  - Project bootstrap and core initialization
  - Files: `main.cpp`, `include/Application.h`, `src/Application.cpp`, `include/Renderer.h`, `src/Renderer.cpp`, `include/CloudSystem.h`, `src/CloudSystem.cpp`, `src/glad.c` (as needed)

- Abhiraj Singh Chauhan (2025-10-14 → 2025-10-19)

  - Orchestration and celestial/weather systems
  - Files: `include/WeatherSystem.h`, `src/WeatherSystem.cpp`, `include/CelestialSystem.h`, `src/CelestialSystem.cpp`
  - Light integrations (minor adjustments/comments where needed)

- arnav505 (2025-10-22 → 2025-10-26)
  - Particle and fog subsystems
  - Files: `include/ParticleSystem.h`, `src/ParticleSystem.cpp`, `include/FogSystem.h`, `src/FogSystem.cpp`
