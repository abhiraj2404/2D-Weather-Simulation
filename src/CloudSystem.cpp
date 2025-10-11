#include "CloudSystem.h"
#include <cstdlib>
#include <algorithm>

CloudSystem::CloudSystem(int maxClouds)
    : maxClouds(maxClouds), cloudDensity(0.5f), screenWidth(1280), screenHeight(720) {
    clouds.reserve(maxClouds);
}

void CloudSystem::update(float deltaTime, const WeatherSystem& weather, int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    // Determine how many clouds based on cloud cover
    float targetClouds = weather.getCloudCover() * maxClouds;
    
    // Spawn clouds if below target
    while (clouds.size() < static_cast<size_t>(targetClouds)) {
        spawnCloud(screenWidth, screenHeight, weather);
    }
    
    // Remove excess clouds if above target
    while (clouds.size() > static_cast<size_t>(targetClouds)) {
        clouds.pop_back();
    }
    
    // Update cloud positions
    glm::vec2 wind = weather.getWindVector();
    for (auto& cloud : clouds) {
        // Move with wind
        cloud.position.x += (cloud.velocity.x + wind.x * 0.5f) * deltaTime;
        
        // Wrap around screen
        if (cloud.position.x > screenWidth + cloud.size) {
            cloud.position.x = -cloud.size;
        } else if (cloud.position.x < -cloud.size) {
            cloud.position.x = screenWidth + cloud.size;
        }
        
        // Adjust opacity based on weather state
        WeatherState state = weather.getState();
        if (state == WeatherState::THUNDERSTORM || state == WeatherState::RAINING) {
            cloud.opacity = 0.9f;  // Darker clouds
        } else if (state == WeatherState::CLOUDY) {
            cloud.opacity = 0.7f;
        } else {
            cloud.opacity = 0.4f;  // Light clouds
        }
    }
}

void CloudSystem::render(Renderer& renderer, const WeatherSystem& weather) {
    glm::vec4 baseColor = getCloudColor(weather);
    
    for (const auto& cloud : clouds) {
        // Draw each puff that makes up the cloud
        for (size_t i = 0; i < cloud.puffOffsets.size(); i++) {
            glm::vec2 puffPos = cloud.position + cloud.puffOffsets[i];
            float puffSize = cloud.puffSizes[i];
            
            glm::vec4 color = baseColor;
            color.a = cloud.opacity;
            
            renderer.drawCircle(puffPos, puffSize, color);
        }
    }
}

void CloudSystem::spawnCloud(int screenWidth, int screenHeight, const WeatherSystem& weather) {
    Cloud cloud;
    
    // Random position
    cloud.position.x = static_cast<float>(rand() % screenWidth);
    cloud.position.y = 50.0f + static_cast<float>(rand() % 200);  // Upper part of sky
    
    // Random size
    cloud.size = 40.0f + static_cast<float>(rand() % 80);
    
    // Slow horizontal movement
    cloud.velocity.x = 5.0f + static_cast<float>(rand() % 10);
    cloud.velocity.y = 0.0f;
    
    // Initial opacity
    cloud.opacity = 0.5f;
    
    // Generate cloud shape
    generateCloudShape(cloud);
    
    clouds.push_back(cloud);
}

void CloudSystem::generateCloudShape(Cloud& cloud) {
    // Create a fluffy cloud from multiple overlapping circles
    int numPuffs = 5 + rand() % 5;  // 5-9 puffs per cloud
    
    cloud.puffOffsets.clear();
    cloud.puffSizes.clear();
    
    for (int i = 0; i < numPuffs; i++) {
        // Random offset from cloud center
        float offsetX = (rand() % 100 - 50) * cloud.size / 100.0f;
        float offsetY = (rand() % 60 - 30) * cloud.size / 100.0f;
        
        cloud.puffOffsets.push_back(glm::vec2(offsetX, offsetY));
        
        // Random puff size
        float puffSize = cloud.size * (0.4f + static_cast<float>(rand() % 40) / 100.0f);
        cloud.puffSizes.push_back(puffSize);
    }
}

glm::vec4 CloudSystem::getCloudColor(const WeatherSystem& weather) const {
    WeatherState state = weather.getState();
    float timeOfDay = weather.getTimeOfDay();
    
    glm::vec4 color;
    
    switch (state) {
        case WeatherState::CLEAR:
        case WeatherState::CLOUDY:
            // White/light gray clouds
            color = glm::vec4(0.9f, 0.9f, 0.95f, 1.0f);
            break;
            
        case WeatherState::RAINING:
            // Dark gray clouds
            color = glm::vec4(0.5f, 0.5f, 0.6f, 1.0f);
            break;
            
        case WeatherState::THUNDERSTORM:
            // Very dark clouds
            color = glm::vec4(0.3f, 0.3f, 0.4f, 1.0f);
            break;
            
        case WeatherState::SNOWING:
            // Light gray clouds
            color = glm::vec4(0.75f, 0.75f, 0.8f, 1.0f);
            break;
    }
    
    // Adjust color based on time of day
    if (timeOfDay < 0.25f || timeOfDay > 0.75f) {
        // Night time - darker clouds
        color.r *= 0.4f;
        color.g *= 0.4f;
        color.b *= 0.5f;
    } else if (timeOfDay < 0.35f || timeOfDay > 0.65f) {
        // Dawn/dusk - orange tint
        color.r *= 1.1f;
        color.g *= 0.9f;
        color.b *= 0.8f;
    }
    
    return color;
}
