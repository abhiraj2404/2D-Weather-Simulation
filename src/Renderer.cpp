#include "Renderer.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

// Simple vertex shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    vertexColor = aColor;
}
)";

// Simple fragment shader
const char* fragmentShaderSource = R"(
#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}
)";

Renderer::Renderer() : shaderProgram(0), VAO(0), VBO(0) {
}

Renderer::~Renderer() {
    shutdown();
}

void Renderer::init() {
    // Create shader program
    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    
    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    std::cout << "Renderer initialized" << std::endl;
}

void Renderer::shutdown() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}

void Renderer::setProjection(int width, int height) {
    // Orthographic projection (0,0) at top-left
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    
    glUseProgram(shaderProgram);
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
}

void Renderer::begin() {
    vertices.clear();
}

void Renderer::end() {
    if (vertices.empty()) return;
    
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
    glBindVertexArray(0);
}

void Renderer::drawCircle(const glm::vec2& center, float radius, const glm::vec4& color) {
    generateCircleVertices(center, radius, color);
}

void Renderer::drawRectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    // Two triangles to form a rectangle
    vertices.push_back({position, color});
    vertices.push_back({{position.x + size.x, position.y}, color});
    vertices.push_back({{position.x + size.x, position.y + size.y}, color});
    
    vertices.push_back({position, color});
    vertices.push_back({{position.x + size.x, position.y + size.y}, color});
    vertices.push_back({{position.x, position.y + size.y}, color});
}

void Renderer::drawLine(const glm::vec2& start, const glm::vec2& end, float thickness, const glm::vec4& color) {
    // Draw line as a thin rectangle
    glm::vec2 dir = end - start;
    float length = glm::length(dir);
    if (length < 0.001f) return;
    
    glm::vec2 norm = glm::normalize(dir);
    glm::vec2 perpendicular(-norm.y, norm.x);
    glm::vec2 offset = perpendicular * (thickness * 0.5f);
    
    vertices.push_back({start + offset, color});
    vertices.push_back({end + offset, color});
    vertices.push_back({end - offset, color});
    
    vertices.push_back({start + offset, color});
    vertices.push_back({end - offset, color});
    vertices.push_back({start - offset, color});
}

void Renderer::generateCircleVertices(const glm::vec2& center, float radius, const glm::vec4& color, int segments) {
    float angleStep = 2.0f * 3.14159f / segments;
    
    for (int i = 0; i < segments; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        glm::vec2 p1 = center + glm::vec2(std::cos(angle1) * radius, std::sin(angle1) * radius);
        glm::vec2 p2 = center + glm::vec2(std::cos(angle2) * radius, std::sin(angle2) * radius);
        
        vertices.push_back({center, color});
        vertices.push_back({p1, color});
        vertices.push_back({p2, color});
    }
}

GLuint Renderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }
    
    return shader;
}

GLuint Renderer::createShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}
