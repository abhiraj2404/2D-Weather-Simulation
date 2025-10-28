#include "LightningSystem.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

LightningSystem::LightningSystem(int maxBolts)
    : maxBolts(maxBolts), enabled(true), flashIntensity(0.0f) {
    bolts.reserve(maxBolts);
}

void LightningSystem::update(float deltaTime) {
    // Update active bolts
    for (auto& bolt : bolts) {
        if (bolt.active) {
            bolt.lifetime -= deltaTime;
            if (bolt.lifetime <= 0.0f) {
                bolt.active = false;
            }
        }
    }
    
    // Calculate flash intensity based on active bolts
    flashIntensity = 0.0f;
    for (const auto& bolt : bolts) {
        if (bolt.active) {
            float ratio = bolt.lifetime / bolt.maxLifetime;
            flashIntensity = std::max(flashIntensity, ratio);
        }
    }
    
    // Remove inactive bolts if we have too many
    bolts.erase(
        std::remove_if(bolts.begin(), bolts.end(),
            [](const LightningBolt& b) { return !b.active; }),
        bolts.end()
    );
}

void LightningSystem::render(Renderer& renderer) {
    if (!enabled) return;
    
    for (const auto& bolt : bolts) {
        if (!bolt.active) continue;
        
        float lifetimeRatio = bolt.lifetime / bolt.maxLifetime;
        
        for (const auto& segment : bolt.segments) {
            // Lightning color: bright white/cyan
            float alpha = lifetimeRatio * segment.intensity;
            glm::vec4 color(0.9f, 0.95f, 1.0f, alpha);
            
            // Draw main bolt
            renderer.drawLine(segment.start, segment.end, 2.5f, color);
            
            // Draw glow effect (wider, more transparent)
            glm::vec4 glowColor(0.6f, 0.8f, 1.0f, alpha * 0.3f);
            renderer.drawLine(segment.start, segment.end, 6.0f, glowColor);
            
            // Draw bright core
            glm::vec4 coreColor(1.0f, 1.0f, 1.0f, alpha);
            renderer.drawLine(segment.start, segment.end, 1.0f, coreColor);
        }
    }
}

void LightningSystem::triggerLightning(int screenWidth, int screenHeight) {
    if (!enabled) return;
    if (bolts.size() >= static_cast<size_t>(maxBolts)) return;
    
    LightningBolt bolt;
    bolt.active = true;
    bolt.lifetime = 0.15f + random(0.0f, 0.1f);  // Very short duration
    bolt.maxLifetime = bolt.lifetime;
    
    // Random starting position at top of screen
    glm::vec2 start(random(screenWidth * 0.2f, screenWidth * 0.8f), 0.0f);
    
    // Random ending position in middle/lower screen
    glm::vec2 end(
        start.x + random(-200.0f, 200.0f),
        random(screenHeight * 0.5f, screenHeight * 0.9f)
    );
    
    // Generate the bolt with branches
    generateBolt(bolt, start, end);
    
    bolts.push_back(bolt);
}

void LightningSystem::generateBolt(LightningBolt& bolt, glm::vec2 start, glm::vec2 end, int depth) {
    const int maxDepth = 4;
    const int minSegments = 8;
    const int maxSegments = 15;
    
    glm::vec2 direction = end - start;
    float totalLength = glm::length(direction);
    direction = glm::normalize(direction);
    
    int numSegments = randomInt(minSegments, maxSegments);
    float segmentLength = totalLength / numSegments;
    
    glm::vec2 currentPos = start;
    
    for (int i = 0; i < numSegments; i++) {
        // Add some perpendicular offset for zigzag effect
        glm::vec2 perpendicular(-direction.y, direction.x);
        float offset = random(-20.0f, 20.0f) * (1.0f - (float)i / numSegments);
        
        glm::vec2 nextPos = currentPos + direction * segmentLength + perpendicular * offset;
        
        // Ensure last segment reaches the end
        if (i == numSegments - 1) {
            nextPos = end;
        }
        
        LightningSegment segment;
        segment.start = currentPos;
        segment.end = nextPos;
        segment.intensity = 1.0f - (float)depth / maxDepth;
        
        bolt.segments.push_back(segment);
        
        // Randomly add branches
        if (depth < maxDepth && random(0.0f, 1.0f) < 0.4f) {
            // Create a branch
            glm::vec2 branchDir = glm::normalize(nextPos - currentPos);
            float angle = random(-0.8f, 0.8f);  // Branch angle
            
            // Rotate branch direction
            glm::vec2 rotatedDir(
                branchDir.x * cos(angle) - branchDir.y * sin(angle),
                branchDir.x * sin(angle) + branchDir.y * cos(angle)
            );
            
            float branchLength = random(50.0f, 150.0f) * (1.0f - (float)depth / maxDepth);
            addBranch(bolt, currentPos, rotatedDir, branchLength, depth + 1);
        }
        
        currentPos = nextPos;
    }
}

void LightningSystem::addBranch(LightningBolt& bolt, glm::vec2 branchStart, glm::vec2 direction, float length, int depth) {
    const int maxDepth = 4;
    int numSegments = randomInt(3, 6);
    float segmentLength = length / numSegments;
    
    glm::vec2 currentPos = branchStart;
    
    for (int i = 0; i < numSegments; i++) {
        glm::vec2 perpendicular(-direction.y, direction.x);
        float offset = random(-10.0f, 10.0f);
        
        glm::vec2 nextPos = currentPos + direction * segmentLength + perpendicular * offset;
        
        LightningSegment segment;
        segment.start = currentPos;
        segment.end = nextPos;
        segment.intensity = 0.6f * (1.0f - (float)depth / maxDepth);
        
        bolt.segments.push_back(segment);
        
        // Smaller chance for sub-branches
        if (depth < maxDepth && random(0.0f, 1.0f) < 0.2f) {
            glm::vec2 branchDir = glm::normalize(nextPos - currentPos);
            float angle = random(-0.6f, 0.6f);
            
            glm::vec2 rotatedDir(
                branchDir.x * cos(angle) - branchDir.y * sin(angle),
                branchDir.x * sin(angle) + branchDir.y * cos(angle)
            );
            
            float subBranchLength = random(30.0f, 80.0f) * (1.0f - (float)depth / maxDepth);
            addBranch(bolt, currentPos, rotatedDir, subBranchLength, depth + 1);
        }
        
        currentPos = nextPos;
    }
}

float LightningSystem::getFlashIntensity() const {
    return flashIntensity;
}

float LightningSystem::random(float min, float max) const {
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return min + r * (max - min);
}

int LightningSystem::randomInt(int min, int max) const {
    return min + (rand() % (max - min + 1));
}
