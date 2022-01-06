#include <Renderer.h>
using namespace gl44;
Renderer::Renderer() {}
Renderer &Renderer::instance()
{
    static Renderer renderer;
    return renderer;
}
void Renderer::init(int height, int width)
{
    this->height = height;
    this->width = width;
    glbinding::initialize(nullptr);
    glGenTextures(1, &firstPassTex);
    glBindTexture(GL_TEXTURE_2D, firstPassTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    GLuint framebufferRBO;
    glGenRenderbuffers(1, &framebufferRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, framebufferRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glGenFramebuffers(1, &firstPassFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, firstPassFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, firstPassTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferRBO);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    assert(glGetError() == 0);
}