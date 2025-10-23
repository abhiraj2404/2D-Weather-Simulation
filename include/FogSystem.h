#pragma once

#include <glm/glm.hpp>
#include "Renderer.h"
#include "WeatherSystem.h"

class FogSystem {
public:
    FogSystem();
    
    void update(float deltaTime, const WeatherSystem& weather);
    void render(Renderer& renderer, int screenWidth, int screenHeight);
    
    void setEnabled(bool enabled) { this->enabled = enabled; }
    bool isEnabled() const { return enabled; }
    
    float getDensity() const { return density; }
    
private:
    bool enabled;
    float density;  // 0.0 to 1.0
    float targetDensity;
    float transitionSpeed;
    
    // Calculate target fog density based on weather
    float calculateTargetDensity(const WeatherSystem& weather) const;
};
