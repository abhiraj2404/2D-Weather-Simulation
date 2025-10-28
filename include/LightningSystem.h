#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Renderer.h"

struct LightningSegment {
    glm::vec2 start;
    glm::vec2 end;
    float intensity;  // 0.0 to 1.0
};

struct LightningBolt {
    std::vector<LightningSegment> segments;
    float lifetime;
    float maxLifetime;
    bool active;
};

class LightningSystem {
public:
    LightningSystem(int maxBolts = 5);
    
    void update(float deltaTime);
    void render(Renderer& renderer);
    
    // Trigger a new lightning bolt
    void triggerLightning(int screenWidth, int screenHeight);
    
    // Get flash intensity for screen flash effect
    float getFlashIntensity() const;
    
    void setEnabled(bool enabled) { this->enabled = enabled; }
    bool isEnabled() const { return enabled; }
    
private:
    std::vector<LightningBolt> bolts;
    int maxBolts;
    bool enabled;
    float flashIntensity;
    
    // Generate lightning bolt using recursive branching
    void generateBolt(LightningBolt& bolt, glm::vec2 start, glm::vec2 end, int depth = 0);
    
    // Add a branch to the lightning
    void addBranch(LightningBolt& bolt, glm::vec2 branchStart, glm::vec2 direction, float length, int depth);
    
    // Random utility
    float random(float min, float max) const;
    int randomInt(int min, int max) const;
};
