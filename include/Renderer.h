#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Initialize renderer (create shader, VAO, VBO)
    void init();
    void shutdown();

    // Drawing primitives
    void drawCircle(const glm::vec2& center, float radius, const glm::vec4& color);
    void drawRectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    void drawLine(const glm::vec2& start, const glm::vec2& end, float thickness, const glm::vec4& color);
    
    // Batch rendering
    void begin();
    void end();

    // Set viewport/projection
    void setProjection(int width, int height);

private:
    GLuint shaderProgram;
    GLuint VAO, VBO;
    
    struct Vertex {
        glm::vec2 position;
        glm::vec4 color;
    };
    
    std::vector<Vertex> vertices;
    
    // Shader compilation
    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc);
    
    // Circle generation
    void generateCircleVertices(const glm::vec2& center, float radius, const glm::vec4& color, int segments = 32);
};
