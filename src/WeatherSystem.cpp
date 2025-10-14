#include "WeatherSystem.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

WeatherSystem::WeatherSystem()
    : temperature(20.0f),
      windVector(0.0f, 0.0f),
      cloudCover(0.2f),
      pressure(1013.0f),
      humidity(0.5f),
      timeOfDay(0.5f),  // Start at noon
      timeScale(0.01f),  // Slow time progression
      currentState(WeatherState::CLEAR),
      stateTransitionTimer(0.0f),
      lightningFlash(0.0f),
      lightningDecay(5.0f) {
    
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void WeatherSystem::update(float deltaTime) {
    // Update time of day
    timeOfDay += deltaTime * timeScale;
    if (timeOfDay > 1.0f) {
        timeOfDay -= 1.0f;
    }

    // Update state transitions
    updateStateTransitions(deltaTime);

    // Decay lightning flash
    if (lightningFlash > 0.0f) {
        lightningFlash -= deltaTime * lightningDecay;
        if (lightningFlash < 0.0f) {
            lightningFlash = 0.0f;
        }
    }
}

void WeatherSystem::setState(WeatherState newState) {
    if (newState == currentState) return;
    
    currentState = newState;
    
    // Adjust weather variables based on new state
    switch (currentState) {
        case WeatherState::CLEAR:
            cloudCover = 0.1f;
            break;
        case WeatherState::CLOUDY:
            cloudCover = 0.6f;
            break;
        case WeatherState::RAINING:
            cloudCover = 0.8f;
            humidity = 0.9f;
            break;
        case WeatherState::THUNDERSTORM:
            cloudCover = 0.95f;
            humidity = 1.0f;
            windVector.x = random(-20.0f, 20.0f);
            break;
        case WeatherState::SNOWING:
            cloudCover = 0.7f;
            temperature = random(-10.0f, 0.0f);
            break;
    }
}

std::string WeatherSystem::getStateAsString() const {
    switch (currentState) {
        case WeatherState::CLEAR: return "CLEAR";
        case WeatherState::CLOUDY: return "CLOUDY";
        case WeatherState::RAINING: return "RAINING";
        case WeatherState::THUNDERSTORM: return "THUNDERSTORM";
        case WeatherState::SNOWING: return "SNOWING";
        default: return "UNKNOWN";
    }
}

void WeatherSystem::updateStateTransitions(float deltaTime) {
    stateTransitionTimer += deltaTime;
    
    // Check for state transitions every 5 seconds
    if (stateTransitionTimer < 5.0f) return;
    
    stateTransitionTimer = 0.0f;
    
    // Calculate probability of state change based on pressure and temperature
    float chanceOfRain = 1.0f - normalize(pressure, 980.0f, 1050.0f);
    float randomRoll = random(0.0f, 1.0f);
    
    // State transition logic
    switch (currentState) {
        case WeatherState::CLEAR:
            if (randomRoll < chanceOfRain * 0.3f) {
                setState(WeatherState::CLOUDY);
            }
            break;
            
        case WeatherState::CLOUDY:
            if (randomRoll < chanceOfRain * 0.5f) {
                if (temperature <= 0.0f) {
                    setState(WeatherState::SNOWING);
                } else {
                    setState(WeatherState::RAINING);
                }
            } else if (randomRoll > 0.8f) {
                setState(WeatherState::CLEAR);
            }
            break;
            
        case WeatherState::RAINING:
            if (randomRoll < chanceOfRain * 0.2f) {
                setState(WeatherState::THUNDERSTORM);
            } else if (randomRoll > 0.7f) {
                setState(WeatherState::CLOUDY);
            }
            break;
            
        case WeatherState::THUNDERSTORM:
            // Trigger occasional lightning
            if (random(0.0f, 1.0f) < 0.3f) {
                triggerLightning();
            }
            if (randomRoll > 0.6f) {
                setState(WeatherState::RAINING);
            }
            break;
            
        case WeatherState::SNOWING:
            if (temperature > 2.0f) {
                setState(WeatherState::RAINING);
            } else if (randomRoll > 0.7f) {
                setState(WeatherState::CLOUDY);
            }
            break;
    }
}

void WeatherSystem::triggerLightning() {
    lightningFlash = 1.0f;
    // TODO: Play thunder sound when audio is integrated
}

glm::vec3 WeatherSystem::getSkyColor() const {
    // Base sky colors for different times of day
    glm::vec3 nightColor(0.05f, 0.05f, 0.15f);      // Dark blue
    glm::vec3 dawnColor(1.0f, 0.5f, 0.3f);          // Orange/red
    glm::vec3 dayColor(0.3f, 0.6f, 1.0f);           // Bright blue
    glm::vec3 duskColor(1.0f, 0.4f, 0.2f);          // Deep orange
    
    glm::vec3 baseColor;
    
    // Interpolate based on time of day
    if (timeOfDay < 0.25f) {
        // Night (0.0 - 0.25)
        baseColor = nightColor;
    } else if (timeOfDay < 0.35f) {
        // Dawn (0.25 - 0.35)
        float t = (timeOfDay - 0.25f) / 0.1f;
        baseColor = glm::mix(nightColor, dawnColor, t);
    } else if (timeOfDay < 0.4f) {
        // Morning (0.35 - 0.4)
        float t = (timeOfDay - 0.35f) / 0.05f;
        baseColor = glm::mix(dawnColor, dayColor, t);
    } else if (timeOfDay < 0.6f) {
        // Day (0.4 - 0.6)
        baseColor = dayColor;
    } else if (timeOfDay < 0.65f) {
        // Evening (0.6 - 0.65)
        float t = (timeOfDay - 0.6f) / 0.05f;
        baseColor = glm::mix(dayColor, duskColor, t);
    } else if (timeOfDay < 0.75f) {
        // Dusk (0.65 - 0.75)
        float t = (timeOfDay - 0.65f) / 0.1f;
        baseColor = glm::mix(duskColor, nightColor, t);
    } else {
        // Night (0.75 - 1.0)
        baseColor = nightColor;
    }
    
    // Modify based on weather state
    switch (currentState) {
        case WeatherState::CLEAR:
            // No modification - bright sky
            break;
            
        case WeatherState::CLOUDY:
            // Slightly darker and more gray
            baseColor *= 0.8f;
            break;
            
        case WeatherState::RAINING:
            // Much darker and gray
            baseColor *= 0.5f;
            baseColor.r = baseColor.g = baseColor.b = (baseColor.r + baseColor.g + baseColor.b) / 3.0f;
            break;
            
        case WeatherState::THUNDERSTORM:
            // Very dark
            baseColor *= 0.3f;
            baseColor.r = baseColor.g = baseColor.b = (baseColor.r + baseColor.g + baseColor.b) / 3.0f;
            break;
            
        case WeatherState::SNOWING:
            // Lighter, more white
            baseColor = glm::mix(baseColor, glm::vec3(0.8f, 0.8f, 0.85f), 0.4f);
            break;
    }
    
    return baseColor;
}

float WeatherSystem::normalize(float value, float min, float max) const {
    return (value - min) / (max - min);
}

float WeatherSystem::random(float min, float max) const {
    float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return min + random * (max - min);
}
