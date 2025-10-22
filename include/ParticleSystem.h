#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Renderer.h"
#include "WeatherSystem.h"

enum class ParticleType {
    RAIN,
    SNOW,
    NONE
};

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float size;
    float lifetime;
    float maxLifetime;
    glm::vec4 color;
};

class ParticleSystem {
public:
    ParticleSystem(int maxParticles = 1000);
    
    void update(float deltaTime, const WeatherSystem& weather, int screenWidth, int screenHeight);
    void render(Renderer& renderer);
    
    void setParticleType(ParticleType type);
    ParticleType getParticleType() const { return currentType; }
    
    void setIntensity(float intensity) { this->intensity = intensity; }
    float getIntensity() const { return intensity; }
    
private:
    std::vector<Particle> particles;
    ParticleType currentType;
    int maxParticles;
    float intensity;  // 0.0 to 1.0
    
    // Spawn new particles
    void spawnParticle(int screenWidth, int screenHeight, const WeatherSystem& weather);
    
    // Update individual particle
    void updateParticle(Particle& particle, float deltaTime, const WeatherSystem& weather);
    
    // Remove dead particles
    void cleanupParticles();
};
