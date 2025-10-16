#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Renderer.h"
#include "WeatherSystem.h"

struct Star {
    glm::vec2 position;
    float brightness;
    float twinklePhase;
    float twinkleSpeed;
    float size;
};

class CelestialSystem {
public:
    CelestialSystem(int numStars = 100);
    
    void update(float deltaTime, const WeatherSystem& weather);
    void render(Renderer& renderer, const WeatherSystem& weather, int screenWidth, int screenHeight);
    
    void setEnabled(bool enabled) { this->enabled = enabled; }
    bool isEnabled() const { return enabled; }
    
private:
    std::vector<Star> stars;
    bool enabled;
    
    // Generate star field
    void generateStars(int screenWidth, int screenHeight);
    
    // Render individual celestial bodies
    void renderSun(Renderer& renderer, glm::vec2 position, float radius);
    void renderMoon(Renderer& renderer, glm::vec2 position, float radius, float phase, float alpha);
    void renderStars(Renderer& renderer, float visibility);
    
    // Calculate sun/moon position based on time of day
    glm::vec2 calculateCelestialPosition(float timeOfDay, int screenWidth, int screenHeight, bool isSun) const;
    
    // Random utility
    float random(float min, float max) const;
};
