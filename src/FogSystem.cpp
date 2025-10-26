#include "FogSystem.h"
#include <algorithm>

FogSystem::FogSystem()
    : enabled(true), density(0.0f), targetDensity(0.0f), transitionSpeed(0.2f) {
}

void FogSystem::update(float deltaTime, const WeatherSystem& weather) {
    if (!enabled) return;
    
    // Calculate target fog density
    targetDensity = calculateTargetDensity(weather);
    
    // Smoothly transition to target density
    if (density < targetDensity) {
        density += deltaTime * transitionSpeed;
        if (density > targetDensity) density = targetDensity;
    } else if (density > targetDensity) {
        density -= deltaTime * transitionSpeed;
        if (density < targetDensity) density = targetDensity;
    }
}

void FogSystem::render(Renderer& renderer, int screenWidth, int screenHeight) {
    if (!enabled || density < 0.01f) return;
    
    // Render fog as gradient layers from bottom to top
    int numLayers = 5;
    float layerHeight = screenHeight / static_cast<float>(numLayers);
    
    for (int i = 0; i < numLayers; i++) {
        float y = screenHeight - (i + 1) * layerHeight;
        
        // Fog is denser at bottom, lighter at top
        float layerDensity = density * (1.0f - static_cast<float>(i) / numLayers);
        
        // Fog color: light gray/white
        glm::vec4 fogColor(0.8f, 0.8f, 0.85f, layerDensity * 0.4f);
        
        // Draw horizontal fog band
        renderer.drawRectangle(
            glm::vec2(0.0f, y), 
            glm::vec2(static_cast<float>(screenWidth), layerHeight), 
            fogColor
        );
    }
}

float FogSystem::calculateTargetDensity(const WeatherSystem& weather) const {
    float baseDensity = 0.0f;
    
    // Humidity affects fog
    float humidity = weather.getHumidity();
    baseDensity += humidity * 0.4f;
    
    // Weather state affects fog
    WeatherState state = weather.getState();
    switch (state) {
        case WeatherState::CLEAR:
            baseDensity *= 0.3f;  // Less fog in clear weather
            break;
        case WeatherState::CLOUDY:
            baseDensity *= 0.8f;
            break;
        case WeatherState::RAINING:
            baseDensity += 0.3f;  // More fog during rain
            break;
        case WeatherState::THUNDERSTORM:
            baseDensity += 0.2f;
            break;
        case WeatherState::SNOWING:
            baseDensity += 0.4f;  // Significant fog during snow
            break;
    }
    
    // Time of day - more fog at dawn/dusk
    float timeOfDay = weather.getTimeOfDay();
    if ((timeOfDay > 0.2f && timeOfDay < 0.35f) || (timeOfDay > 0.65f && timeOfDay < 0.8f)) {
        baseDensity += 0.2f;
    }
    
    // Clamp to valid range
    return std::min(baseDensity, 1.0f);
}

// Tweak note: fog params
