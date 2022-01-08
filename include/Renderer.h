#pragma once
#include <assert.h>
#include <vector>
#include <fstream>
#include <memory>
#include <glbinding/glbinding.h>
#include <glbinding/gl44/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Scene.h>
#define RENDERER Renderer::instance()
class Renderer
{
private:
    int height, width;
    gl::GLuint firstPassFBO, firstPassTex, firstPassDepthMap,
        secondPassShaderProgram, quadVAO,
        firstPassShaderProgram, MVPLocation;
    std::unique_ptr<Scene> scene;
    glm::mat4 VP;
    glm::vec3 lightPos = {0, 1, 0};

    Renderer();
    gl::GLuint loadShader(const char *vertexShader, const char *fragmentShader);
    gl::GLuint createSecondPassQuad();

public:
    static Renderer &instance();
    void init(int height, int width);
    void loadScene(const char *path);
    void drawFirstRenderPass();
    void drawSecondRenderPass();
    static std::vector<char> readBinaryFile(const char *path);
};