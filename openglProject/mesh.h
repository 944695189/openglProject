#pragma once
#include "shader.h"
#define MAX_BONE_INFLUENCE 4

struct Vertex {//定义顶点变量
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};
struct Texture {//定义纹理变量
    unsigned int id=0;
    string type;
    string path;
};
class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader shader);
private:
    unsigned int VBO, EBO;
    void setupMesh();
};