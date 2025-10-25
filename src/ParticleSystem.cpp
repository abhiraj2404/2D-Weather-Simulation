#include "ParticleSystem.h"
#include <cstdlib>
#include <algorithm>

ParticleSystem::ParticleSystem(int maxParticles)
    : currentType(ParticleType::NONE), maxParticles(maxParticles), intensity(1.0f) {
    particles.reserve(maxParticles);
}

void ParticleSystem::update(float deltaTime, const WeatherSystem& weather, int screenWidth, int screenHeight) {
    // Determine particle type based on weather
    WeatherState state = weather.getState();
    if (state == WeatherState::RAINING || state == WeatherState::THUNDERSTORM) {
        currentType = ParticleType::RAIN;
    } else if (state == WeatherState::SNOWING) {
        currentType = ParticleType::SNOW;
    } else {
        currentType = ParticleType::NONE;
    }
    
    // Spawn new particles based on intensity and weather
    if (currentType != ParticleType::NONE) {
        // Base spawn rate per frame (scaled for 60 FPS)
        float spawnRate = intensity * 600.0f * deltaTime;  // Particles per second
        
        if (state == WeatherState::THUNDERSTORM) {
            spawnRate *= 2.5f;  // More intense rain
        } else if (state == WeatherState::RAINING) {
            spawnRate *= 1.5f;  // Heavy rain
        } else if (state == WeatherState::SNOWING) {
            spawnRate *= 1.0f;  // Moderate snow (same as base)
        }
        
        int particlesToSpawn = static_cast<int>(spawnRate);
        
        // Fractional particles (for smooth spawning at low rates)
        static float fractionalParticles = 0.0f;
        fractionalParticles += spawnRate - particlesToSpawn;
        if (fractionalParticles >= 1.0f) {
            particlesToSpawn += 1;
            fractionalParticles -= 1.0f;
        }
        
        for (int i = 0; i < particlesToSpawn; i++) {
            if (particles.size() < static_cast<size_t>(maxParticles)) {
                spawnParticle(screenWidth, screenHeight, weather);
            }
        }
    }
    
    // Update all particles
    for (auto& particle : particles) {
        updateParticle(particle, deltaTime, weather);
    }
    
    // Remove dead particles
    cleanupParticles();
}

void ParticleSystem::render(Renderer& renderer) {
    for (const auto& particle : particles) {
        if (currentType == ParticleType::RAIN) {
            // Draw rain as a short line
            glm::vec2 end = particle.position + particle.velocity * 0.02f;
            renderer.drawLine(particle.position, end, particle.size, particle.color);
        } else if (currentType == ParticleType::SNOW) {
            // Draw snow as a circle
            renderer.drawCircle(particle.position, particle.size, particle.color);
        }
    }
}

void ParticleSystem::setParticleType(ParticleType type) {
    if (currentType != type) {
        currentType = type;
        particles.clear();  // Clear old particles when changing type
    }
}

void ParticleSystem::spawnParticle(int screenWidth, int screenHeight, const WeatherSystem& weather) {
    Particle particle;
    
    // Random position across top of screen (with some margin above)
    particle.position.x = static_cast<float>(rand() % screenWidth);
    particle.position.y = -20.0f - static_cast<float>(rand() % 50);
    
    glm::vec2 wind = weather.getWindVector();
    
    if (currentType == ParticleType::RAIN) {
        // Rain falls faster and is affected more by wind
        particle.velocity.x = wind.x * 3.0f + (rand() % 20 - 10);
        particle.velocity.y = 300.0f + (rand() % 200);  // Fast downward
        particle.size = 1.5f + static_cast<float>(rand() % 10) / 10.0f;
        particle.color = glm::vec4(0.6f, 0.6f, 0.8f, 0.6f);  // Light blue, semi-transparent
        particle.maxLifetime = 5.0f;
    } else if (currentType == ParticleType::SNOW) {
        // Snow falls slower and drifts more
        particle.velocity.x = wind.x * 5.0f + (rand() % 40 - 20);
        particle.velocity.y = 30.0f + (rand() % 50);  // Slow downward
        particle.size = 2.0f + static_cast<float>(rand() % 20) / 10.0f;
        particle.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);  // White, semi-transparent
        particle.maxLifetime = 10.0f;
    }
    
    particle.lifetime = particle.maxLifetime;
    
    particles.push_back(particle);
}

void ParticleSystem::updateParticle(Particle& particle, float deltaTime, const WeatherSystem& weather) {
    // Update position
    particle.position += particle.velocity * deltaTime;
    
    // Add some randomness to snow movement (swaying)
    if (currentType == ParticleType::SNOW) {
        particle.velocity.x += (rand() % 20 - 10) * deltaTime;
    }
    
    // Update lifetime
    particle.lifetime -= deltaTime;
    
    // Fade out as lifetime decreases
    float lifetimeRatio = particle.lifetime / particle.maxLifetime;
    particle.color.a = lifetimeRatio * 0.8f;
}

void ParticleSystem::cleanupParticles() {
    // Remove particles that have expired or fallen off screen
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) {
                return p.lifetime <= 0.0f || p.position.y > 1000.0f;
            }),
        particles.end()
    );
}

// Tweak note: particle params
