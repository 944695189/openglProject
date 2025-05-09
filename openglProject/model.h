#include "mesh.h"

#include <SOIL2/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    vector<Texture> texturesLoaded;	
    vector<Mesh>  meshes;
    string directory;
    bool gammaCorrection;
    void model(string const& path, bool gamma = false) 
    {
        gammaCorrection = gamma;
        loadModel(path);
    }

    void draw(Shader& shader);

private:  
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

