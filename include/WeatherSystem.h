#pragma once

#include <glm/glm.hpp>
#include <string>

enum class WeatherState {
    CLEAR,
    CLOUDY,
    RAINING,
    THUNDERSTORM,
    SNOWING
};

class WeatherSystem {
public:
    WeatherSystem();

    // Update the weather simulation
    void update(float deltaTime);

    // State management
    void setState(WeatherState newState);
    WeatherState getState() const { return currentState; }
    std::string getStateAsString() const;

    // Weather variable getters
    float getTemperature() const { return temperature; }
    float getPressure() const { return pressure; }
    float getHumidity() const { return humidity; }
    float getCloudCover() const { return cloudCover; }
    float getTimeOfDay() const { return timeOfDay; }
    glm::vec2 getWindVector() const { return windVector; }

    // Weather variable setters
    void setTemperature(float temp) { temperature = temp; }
    void setPressure(float pres) { pressure = pres; }
    void setHumidity(float hum) { humidity = hum; }
    void setCloudCover(float cover) { cloudCover = cover; }
    void setTimeOfDay(float time) { timeOfDay = time; }

    // Weather variables
    float temperature;        // In Celsius (-20 to 40)
    glm::vec2 windVector;     // Wind direction and speed
    float cloudCover;         // 0.0 to 1.0
    float pressure;           // 980.0 to 1050.0 hPa
    float humidity;           // 0.0 to 1.0

    // Special effects
    void triggerLightning();
    float getLightningFlash() const { return lightningFlash; }

    // Sky color calculation
    glm::vec3 getSkyColor() const;
    
    // Time of day (0.0 = midnight, 0.5 = noon, 1.0 = next midnight)
    float timeOfDay;
    float timeScale;          // Speed of day/night cycle

private:
    WeatherState currentState;
    
    // State transition logic
    void updateStateTransitions(float deltaTime);
    float stateTransitionTimer;
    
    // Lightning effect
    float lightningFlash;
    float lightningDecay;

    // Utility functions
    float normalize(float value, float min, float max) const;
    float random(float min, float max) const;
};
