#include <Scene.h>
using namespace std;
using namespace gl;
using namespace glm;
using namespace Assimp;
Importer Scene::importer;
Scene::Scene(const char *path)
{
    loadScene(path);
}
void Scene::draw()
{
    for (unsigned i = 0; i < meshes.size(); ++i)
    {
        glBindVertexArray(meshes[i].VAO);
        glDrawElements(GL_TRIANGLES, meshes[i].indicesCount, GL_UNSIGNED_INT, 0);
    }
}
void Scene::loadScene(const char *path)
{
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);
    int meshesCount = scene->mNumMeshes;
    for (unsigned i = 0; i < meshesCount; ++i)
    {
        Mesh mesh = loadMesh(scene->mMeshes[i]);
        meshes.push_back(mesh);
    }
}
Scene::Mesh Scene::loadMesh(aiMesh *mesh)
{
    vector<vec3> vertices;
    vector<unsigned> indices;
    for (unsigned i = 0; i < mesh->mNumVertices; ++i)
    {
        vertices.push_back({mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
        vertices.push_back({mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
    }
    for (unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, (void *)sizeof(vec3));
    glBindVertexArray(0);
    assert(glGetError() == 0);
    return Mesh{VAO, (unsigned)indices.size()};
}