#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "WeatherSystem.h"
#include "ParticleSystem.h"
#include "CloudSystem.h"
#include "LightningSystem.h"
#include "CelestialSystem.h"
#include "FogSystem.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Application {
public:
    Application(int width, int height, const std::string& title);
    ~Application();

    // Main application loop
    void run();

    // Getters
    bool isRunning() const { return !glfwWindowShouldClose(window); }
    GLFWwindow* getWindow() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    // Initialization
    bool initGLFW();
    bool initGLAD();
    bool initImGui();

    // Cleanup
    void shutdownImGui();

    // Main loop functions
    void processInput();
    void update(float deltaTime);
    void render();
    void renderUI();

    // Callbacks (static for GLFW)
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Window properties
    GLFWwindow* window;
    int width;
    int height;
    std::string title;

    // Timing
    float lastFrame;
    float deltaTime;

    // Weather simulation systems
    WeatherSystem weatherSystem;
    ParticleSystem particleSystem;
    CloudSystem cloudSystem;
    LightningSystem lightningSystem;
    CelestialSystem celestialSystem;
    FogSystem fogSystem;
    Renderer renderer;
};
