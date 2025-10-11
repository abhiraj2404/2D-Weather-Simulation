#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Renderer.h"
#include "WeatherSystem.h"

struct Cloud {
    glm::vec2 position;
    glm::vec2 velocity;
    float size;
    float opacity;
    std::vector<glm::vec2> puffOffsets;  // Multiple circles to form cloud shape
    std::vector<float> puffSizes;
};

class CloudSystem {
public:
    CloudSystem(int maxClouds = 15);
    
    void update(float deltaTime, const WeatherSystem& weather, int screenWidth, int screenHeight);
    void render(Renderer& renderer, const WeatherSystem& weather);
    
    void setCloudDensity(float density) { this->cloudDensity = density; }
    float getCloudDensity() const { return cloudDensity; }
    
private:
    std::vector<Cloud> clouds;
    int maxClouds;
    float cloudDensity;  // 0.0 to 1.0
    int screenWidth;
    int screenHeight;
    
    // Spawn new cloud
    void spawnCloud(int screenWidth, int screenHeight, const WeatherSystem& weather);
    
    // Generate cloud shape (multiple puffs)
    void generateCloudShape(Cloud& cloud);
    
    // Get cloud color based on weather
    glm::vec4 getCloudColor(const WeatherSystem& weather) const;
};
