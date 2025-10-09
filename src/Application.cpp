#include "Application.h"
#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : window(nullptr), width(width), height(height), title(title),
      lastFrame(0.0f), deltaTime(0.0f), weatherSystem(), 
      particleSystem(1000), cloudSystem(15), lightningSystem(5),
      celestialSystem(100), fogSystem(), renderer() {
}

Application::~Application() {
    shutdownImGui();
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Application::initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Store 'this' pointer in window for callbacks
    glfwSetWindowUserPointer(window, this);

    std::cout << "GLFW initialized successfully" << std::endl;
    return true;
}

bool Application::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLAD initialized successfully" << std::endl;
    return true;
}

bool Application::initImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::cout << "ImGui initialized successfully" << std::endl;
    return true;
}

void Application::shutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::run() {
    if (!initGLFW() || !initGLAD() || !initImGui()) {
        return;
    }

    // Initialize renderer
    renderer.init();
    renderer.setProjection(width, height);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "==================================" << std::endl;
    std::cout << "2D Weather Simulation Started!" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;
    std::cout << "==================================" << std::endl;

    // Main render loop
    while (isRunning()) {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput();

        // Update simulation
        update(deltaTime);

        // Render
        render();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Application::update(float deltaTime) {
    // Update weather system
    weatherSystem.update(deltaTime);
    
    // Update particle system
    particleSystem.update(deltaTime, weatherSystem, width, height);
    
    // Update cloud system
    cloudSystem.update(deltaTime, weatherSystem, width, height);
    
    // Update lightning system
    lightningSystem.update(deltaTime);
    
    // Trigger lightning during thunderstorms
    if (weatherSystem.getState() == WeatherState::THUNDERSTORM) {
        static float lightningTimer = 0.0f;
        lightningTimer += deltaTime;
        if (lightningTimer > 2.0f) {  // Lightning every 2 seconds
            if (rand() % 100 < 30) {  // 30% chance
                lightningSystem.triggerLightning(width, height);
                weatherSystem.triggerLightning();  // Trigger flash
            }
            lightningTimer = 0.0f;
        }
    }
    
    // Update celestial system (sun/moon/stars)
    celestialSystem.update(deltaTime, weatherSystem);
    
    // Update fog system
    fogSystem.update(deltaTime, weatherSystem);
    
    // Log state changes
    static WeatherState lastState = weatherSystem.getState();
    if (weatherSystem.getState() != lastState) {
        std::cout << "Weather changed to: " << weatherSystem.getStateAsString() << std::endl;
        lastState = weatherSystem.getState();
    }
}

void Application::render() {
    // Get sky color from weather system (with day/night cycle)
    glm::vec3 skyColor = weatherSystem.getSkyColor();
    
    // Add lightning flash effect from both systems
    float flash = std::max(weatherSystem.getLightningFlash(), lightningSystem.getFlashIntensity());
    skyColor.r += flash * 0.5f;
    skyColor.g += flash * 0.5f;
    skyColor.b += flash * 0.5f;
    
    // Clear screen with weather-appropriate color
    glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Begin rendering
    renderer.begin();
    
    // Render in order: back to front
    
    // 1. Celestial bodies (sun/moon/stars) - furthest back
    celestialSystem.render(renderer, weatherSystem, width, height);
    
    // 2. Clouds
    cloudSystem.render(renderer, weatherSystem);
    
    // 3. Lightning bolts
    lightningSystem.render(renderer);
    
    // 4. Particles (rain/snow)
    particleSystem.render(renderer);
    
    // 5. Fog (foreground atmosphere)
    fogSystem.render(renderer, width, height);
    
    // End rendering (draws everything)
    renderer.end();

    // Render UI on top
    renderUI();
}

void Application::renderUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create Weather Controls window
    ImGui::Begin("Weather Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    // Display current weather state
    ImGui::Text("Current Weather: %s", weatherSystem.getStateAsString().c_str());
    ImGui::Separator();
    
    // ===== MANUAL WEATHER CONTROL BUTTONS =====
    ImGui::Text("Manual Weather Control:");
    
    if (ImGui::Button("Clear Skies", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::CLEAR);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cloudy", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::CLOUDY);
    }
    
    if (ImGui::Button("Raining", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::RAINING);
    }
    ImGui::SameLine();
    if (ImGui::Button("Thunderstorm", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::THUNDERSTORM);
    }
    
    if (ImGui::Button("Snowing", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::SNOWING);
    }
    ImGui::SameLine();
    if (ImGui::Button("Trigger Lightning", ImVec2(120, 0))) {
        lightningSystem.triggerLightning(width, height);
        weatherSystem.triggerLightning();
    }
    
    ImGui::Separator();
    
    // ===== WEATHER PARAMETERS =====
    ImGui::Text("Weather Parameters:");
    
    // Temperature slider (-20°C to 40°C)
    float temp = weatherSystem.getTemperature();
    if (ImGui::SliderFloat("Temperature (°C)", &temp, -20.0f, 40.0f)) {
        weatherSystem.setTemperature(temp);
    }
    
    // Pressure slider (980 hPa to 1050 hPa)
    float pressure = weatherSystem.getPressure();
    if (ImGui::SliderFloat("Pressure (hPa)", &pressure, 980.0f, 1050.0f)) {
        weatherSystem.setPressure(pressure);
    }
    
    // Humidity slider (0% to 100%)
    float humidity = weatherSystem.getHumidity();
    if (ImGui::SliderFloat("Humidity (%)", &humidity, 0.0f, 1.0f)) {
        weatherSystem.setHumidity(humidity);
    }
    
    // Cloud cover slider (0% to 100%)
    float cloudCover = weatherSystem.getCloudCover();
    if (ImGui::SliderFloat("Cloud Cover (%)", &cloudCover, 0.0f, 1.0f)) {
        weatherSystem.setCloudCover(cloudCover);
    }
    
    // Time of day slider (0 = midnight, 0.5 = noon, 1 = midnight)
    float timeOfDay = weatherSystem.getTimeOfDay();
    if (ImGui::SliderFloat("Time of Day", &timeOfDay, 0.0f, 1.0f)) {
        weatherSystem.setTimeOfDay(timeOfDay);
    }
    
    ImGui::Separator();
    
    // ===== SYSTEM INFO =====
    ImGui::Text("System Info:");
    
    // Wind display
    glm::vec2 wind = weatherSystem.getWindVector();
    ImGui::Text("Wind: (%.1f, %.1f) m/s", wind.x, wind.y);
    
    // Lightning intensity display
    float lightning = std::max(weatherSystem.getLightningFlash(), lightningSystem.getFlashIntensity());
    ImGui::Text("Lightning Intensity: %.2f", lightning);
    
    // Fog density
    ImGui::Text("Fog Density: %.2f", fogSystem.getDensity());
    
    ImGui::Separator();
    
    // ===== QUICK PRESETS =====
    ImGui::Text("Quick Presets:");
    
    if (ImGui::Button("Summer Day", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::CLEAR);
        weatherSystem.setTemperature(28.0f);
        weatherSystem.setTimeOfDay(0.5f);
        weatherSystem.setCloudCover(0.2f);
    }
    ImGui::SameLine();
    if (ImGui::Button("Winter Night", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::SNOWING);
        weatherSystem.setTemperature(-5.0f);
        weatherSystem.setTimeOfDay(0.0f);
        weatherSystem.setCloudCover(0.8f);
    }
    
    if (ImGui::Button("Stormy Evening", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::THUNDERSTORM);
        weatherSystem.setTemperature(18.0f);
        weatherSystem.setTimeOfDay(0.7f);
        weatherSystem.setCloudCover(0.95f);
    }
    ImGui::SameLine();
    if (ImGui::Button("Foggy Dawn", ImVec2(120, 0))) {
        weatherSystem.setState(WeatherState::CLOUDY);
        weatherSystem.setTemperature(12.0f);
        weatherSystem.setTimeOfDay(0.3f);
        weatherSystem.setHumidity(0.9f);
        weatherSystem.setCloudCover(0.7f);
    }
    
    ImGui::End();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Static callbacks
void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    // Update application's width and height
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->width = width;
        app->height = height;
        // Update renderer projection for new dimensions
        app->renderer.setProjection(width, height);
    }
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Handle key presses here if needed
    // This is separate from processInput for specific key events
}
