#pragma once
#include <assert.h>
#include <vector>
#include <fstream>
#include <glbinding/glbinding.h>
#include <glbinding/gl44/gl.h>
#define RENDERER Renderer::instance()
class Renderer
{
private:
    int height, width;
    gl::GLuint firstPassFBO, firstPassTex, secondPassShaderProgram, quadVAO;

    Renderer();
    gl::GLuint loadShader(const char *vertexShader, const char *fragmentShader);
    gl::GLuint createSecondPassQuad();

public:
    static Renderer &instance();
    void init(int height, int width);
    void drawFirstRenderPass();
    void drawSecondRenderPass();
    static std::vector<char> readBinaryFile(const char *path);
};