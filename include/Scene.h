#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include<assimp/postprocess.h>
#include <glbinding/gl44/gl.h>
#include<glm/glm.hpp>
class Scene
{
private:
    class Mesh
    {
    public:
        gl::GLuint VAO;
        unsigned indicesCount;
    };
    std::vector<Mesh> meshes;
    void loadScene(const char *path);
    Mesh loadMesh(aiMesh *mesh);
    static Assimp::Importer importer;

public:
    Scene(const char *path);
    void draw();
};