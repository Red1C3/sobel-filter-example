#include <Renderer.h>
using namespace gl44;
using namespace std;
using namespace glm;
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
    // Setup first render pass, which is used for rendering the scene
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
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    // Create a quad to draw the first pass texture on with Sobel applied
    quadVAO = createSecondPassQuad();
    // Setup shaders
    secondPassShaderProgram = loadShader("shaders/secondPass.vert", "shaders/secondPass.frag");
    firstPassShaderProgram = loadShader("shaders/firstPass.vert", "shaders/firstPass.frag");
    glUseProgram(firstPassShaderProgram);
    MVPLocation = glGetUniformLocation(firstPassShaderProgram, "VP");
    VP = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f) *
         lookAt(vec3{0, 0, 0}, vec3{1, 0, 0}, vec3{0, 1, 0});
    GLuint lightPosLocation = glGetUniformLocation(firstPassShaderProgram, "light_world");
    glUniform3fv(lightPosLocation, 1, &lightPos.x);
    assert(glGetError() == 0);
}
void Renderer::drawFirstRenderPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, firstPassFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(firstPassShaderProgram);
    glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &VP[0][0]);
    scene->draw();
    assert(glGetError() == 0);
}
void Renderer::drawSecondRenderPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(secondPassShaderProgram);
    glBindTexture(GL_TEXTURE_2D, firstPassTex);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    assert(glGetError() == 0);
}
GLuint Renderer::createSecondPassQuad()
{
    float vertices[] = {-1, -1,
                        1, -1,
                        1, 1,
                        -1, 1};
    unsigned indices[] = {0, 1, 2, 0, 2, 3};
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * 6, indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    assert(glGetError() == 0);
    return VAO;
}
GLuint Renderer::loadShader(const char *vertexShader, const char *fragmentShader)
{
    GLuint id;
    vector<char> vertexShaderArray, fragmentShaderArray;
    const char *vertexShaderCode, *fragmentShaderCode;

    vertexShaderArray = readBinaryFile(vertexShader);
    fragmentShaderArray = readBinaryFile(fragmentShader);
    vertexShaderCode = vertexShaderArray.data();
    fragmentShaderCode = fragmentShaderArray.data();

    {
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        int shaderLength = vertexShaderArray.size();
        glShaderSource(vertexShader, 1, &vertexShaderCode, &shaderLength);
        glCompileShader(vertexShader);
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
        printf("%s \n", infoLog);
        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        shaderLength = fragmentShaderArray.size();
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, &shaderLength);
        glCompileShader(fragmentShader);
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
        printf("%s \n", infoLog);
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        glGetProgramInfoLog(id, 1024, nullptr, infoLog);
        printf("%s \n", infoLog);
        glDetachShader(id, vertexShader);
        glDetachShader(id, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    assert(glGetError() == 0);
    return id;
}
void Renderer::loadScene(const char *path)
{
    scene = make_unique<Scene>(path);
}
vector<char> Renderer::readBinaryFile(const char *path)
{
    fstream fileStream(path, ios::in | ios::binary | ios::ate);
    vector<char> data;
    assert(fileStream.is_open());
    int size = fileStream.tellg();
    fileStream.seekg(0, ios::beg);
    data.resize(size);
    fileStream.read(data.data(), data.size());
    fileStream.flush();
    fileStream.close();
    return data;
}