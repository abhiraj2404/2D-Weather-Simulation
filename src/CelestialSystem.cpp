#include "CelestialSystem.h"
#include <cstdlib>
#include <cmath>

CelestialSystem::CelestialSystem(int numStars)
    : enabled(true) {
    stars.reserve(numStars);
}

void CelestialSystem::update(float deltaTime, const WeatherSystem& weather) {
    // Update star twinkling
    for (auto& star : stars) {
        star.twinklePhase += deltaTime * star.twinkleSpeed;
        if (star.twinklePhase > 2.0f * 3.14159f) {
            star.twinklePhase -= 2.0f * 3.14159f;
        }
    }
}

void CelestialSystem::render(Renderer& renderer, const WeatherSystem& weather, int screenWidth, int screenHeight) {
    if (!enabled) return;
    
    // Generate stars if not already done
    if (stars.empty()) {
        generateStars(screenWidth, screenHeight);
    }
    
    float timeOfDay = weather.getTimeOfDay();
    
    // Determine visibility based on time and weather
    bool isNight = (timeOfDay < 0.25f || timeOfDay > 0.75f);
    bool isDay = (timeOfDay >= 0.4f && timeOfDay <= 0.6f);
    bool isDawnDusk = !isNight && !isDay;
    
    // Stars visible at night (reduced by cloud cover)
    if (isNight) {
        float starVisibility = 1.0f - weather.getCloudCover() * 0.8f;
        if (weather.getState() == WeatherState::CLEAR) {
            starVisibility = 1.0f;
        }
        renderStars(renderer, starVisibility);
    }
    
    // Calculate celestial body positions
    glm::vec2 sunPosition = calculateCelestialPosition(timeOfDay, screenWidth, screenHeight, true);
    glm::vec2 moonPosition = calculateCelestialPosition(timeOfDay, screenWidth, screenHeight, false);
    float radius = 40.0f;
    
    // Render sun during day
    if (timeOfDay >= 0.25f && timeOfDay <= 0.75f) {
        // Sun visible from dawn to dusk
        float sunAlpha = 1.0f;
        
        // Fade during dawn/dusk
        if (timeOfDay < 0.35f) {
            sunAlpha = (timeOfDay - 0.25f) / 0.1f;
        } else if (timeOfDay > 0.65f) {
            sunAlpha = (0.75f - timeOfDay) / 0.1f;
        }
        
        // Reduce visibility in bad weather
        if (weather.getState() == WeatherState::RAINING || 
            weather.getState() == WeatherState::THUNDERSTORM) {
            sunAlpha *= 0.3f;
        } else if (weather.getState() == WeatherState::CLOUDY) {
            sunAlpha *= 0.6f;
        }
        
        if (sunAlpha > 0.01f) {
            renderSun(renderer, sunPosition, radius);
        }
    }
    
    // Render moon during night (opposite times from sun)
    if (timeOfDay < 0.25f || timeOfDay > 0.75f) {
        float moonAlpha = 1.0f;
        
        // Fade during dawn/dusk
        if (timeOfDay > 0.15f && timeOfDay < 0.25f) {
            moonAlpha = (0.25f - timeOfDay) / 0.1f;
        } else if (timeOfDay > 0.75f && timeOfDay < 0.85f) {
            moonAlpha = (timeOfDay - 0.75f) / 0.1f;
        }
        
        // Reduce visibility in bad weather
        if (weather.getState() == WeatherState::RAINING || 
            weather.getState() == WeatherState::THUNDERSTORM) {
            moonAlpha *= 0.3f;
        } else if (weather.getState() == WeatherState::CLOUDY) {
            moonAlpha *= 0.7f;
        }
        
        if (moonAlpha > 0.01f) {
            // Moon phase (simplified - could be tied to actual date)
            float phase = fmod(timeOfDay * 30.0f, 1.0f);  // Cycle through phases
            renderMoon(renderer, moonPosition, radius, phase, moonAlpha);
        }
    }
}

void CelestialSystem::generateStars(int screenWidth, int screenHeight) {
    stars.clear();
    
    for (int i = 0; i < 100; i++) {
        Star star;
        star.position.x = random(0.0f, static_cast<float>(screenWidth));
        star.position.y = random(0.0f, static_cast<float>(screenHeight) * 0.6f);  // Upper portion
        star.brightness = random(0.3f, 1.0f);
        star.twinklePhase = random(0.0f, 6.28f);
        star.twinkleSpeed = random(1.0f, 3.0f);
        star.size = random(1.0f, 2.5f);
        
        stars.push_back(star);
    }
}

void CelestialSystem::renderSun(Renderer& renderer, glm::vec2 position, float radius) {
    // Outer glow (large, faint)
    glm::vec4 outerGlow(1.0f, 0.9f, 0.5f, 0.1f);
    renderer.drawCircle(position, radius * 2.5f, outerGlow);
    
    // Middle glow
    glm::vec4 middleGlow(1.0f, 0.95f, 0.6f, 0.3f);
    renderer.drawCircle(position, radius * 1.5f, middleGlow);
    
    // Sun body
    glm::vec4 sunColor(1.0f, 0.95f, 0.7f, 1.0f);
    renderer.drawCircle(position, radius, sunColor);
    
    // Bright core
    glm::vec4 coreColor(1.0f, 1.0f, 0.95f, 1.0f);
    renderer.drawCircle(position, radius * 0.7f, coreColor);
}

void CelestialSystem::renderMoon(Renderer& renderer, glm::vec2 position, float radius, float phase, float alpha) {
    // Moon glow (behind)
    glm::vec4 glowColor(0.8f, 0.8f, 0.9f, 0.2f * alpha);
    renderer.drawCircle(position, radius * 1.4f, glowColor);
    
    // Moon body (pale white/gray)
    glm::vec4 moonColor(0.9f, 0.9f, 0.95f, 0.9f * alpha);
    renderer.drawCircle(position, radius, moonColor);
}

void CelestialSystem::renderStars(Renderer& renderer, float visibility) {
    for (const auto& star : stars) {
        // Twinkling effect
        float twinkle = 0.5f + 0.5f * sin(star.twinklePhase);
        float brightness = star.brightness * twinkle * visibility;
        
        glm::vec4 starColor(1.0f, 1.0f, 1.0f, brightness);
        renderer.drawCircle(star.position, star.size, starColor);
        
        // Some stars have a slight glow
        if (star.brightness > 0.7f) {
            glm::vec4 glowColor(0.9f, 0.9f, 1.0f, brightness * 0.3f);
            renderer.drawCircle(star.position, star.size * 2.0f, glowColor);
        }
    }
}

glm::vec2 CelestialSystem::calculateCelestialPosition(float timeOfDay, int screenWidth, int screenHeight, bool isSun) const {
    if (isSun) {
        // Sun: Arc from left (east) to right (west) during day (0.25 to 0.75)
        // Map 0.25-0.75 to 0-PI for arc
        float normalizedTime = (timeOfDay - 0.25f) / 0.5f;  // 0 to 1
        float angle = normalizedTime * 3.14159f;  // 0 to PI
        
        // X: Move from left to right
        float x = screenWidth * 0.1f + normalizedTime * screenWidth * 0.8f;
        
        // Y: Arc upward - starts at top area, peaks higher, stays visible
        // sin(0) = 0, sin(PI/2) = 1, sin(PI) = 0
        float arcHeight = screenHeight * 0.25f;  // Arc height
        float baseY = screenHeight * 0.45f;  // Starting position (higher up)
        float y = baseY - sin(angle) * arcHeight;  // Subtract because Y increases downward
        
        return glm::vec2(x, y);
    } else {
        // Moon: Arc from right (west) to left (east) during night
        // For night time (0.75-1.0 and 0.0-0.25), create continuous arc
        float moonTime;
        if (timeOfDay > 0.75f) {
            moonTime = (timeOfDay - 0.75f) / 0.5f;  // 0.75-1.0 maps to 0-0.5
        } else {
            moonTime = (timeOfDay + 0.25f) / 0.5f;  // 0.0-0.25 maps to 0.5-1.0
        }
        
        float angle = moonTime * 3.14159f;  // 0 to PI
        
        // X: Move from right to left
        float x = screenWidth * 0.9f - moonTime * screenWidth * 0.8f;
        
        // Y: Arc upward - starts at top area, peaks higher, stays visible
        float arcHeight = screenHeight * 0.25f;
        float baseY = screenHeight * 0.45f;  // Starting position (higher up)
        float y = baseY - sin(angle) * arcHeight;
        
        return glm::vec2(x, y);
    }
}

float CelestialSystem::random(float min, float max) const {
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return min + r * (max - min);
}
