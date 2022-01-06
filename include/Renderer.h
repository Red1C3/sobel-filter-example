#pragma once
#include <assert.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl44/gl.h>
#define RENDERER Renderer::instance()
class Renderer
{
private:
    Renderer();

    int height, width;
    gl::GLuint firstPassFBO, firstPassTex;

public:
    static Renderer &instance();
    void init(int height, int width);
};