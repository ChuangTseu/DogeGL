#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "material.h"
#include "shader.h"

#include "MathsTools/mat4.h"

#include <string>
#include <vector>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/config.h>

struct ModelNode {
    unsigned int m_numMeshes;
    unsigned int* m_meshIndex;

    std::vector<unsigned int> m_meshIndices;

//    unsigned int m_materialIndex;

    mat4 m_transformation;

    unsigned int m_numNodes;
    ModelNode* m_childNodes;
};

class Model
{
    std::vector<Mesh> m_meshes;
    std::vector<Material> m_materials;

    std::vector<ModelNode> m_nodes;

    ModelNode* m_rootNode;

public:
    enum class BasicType {
        CUBE, PYRAMID, TRIANGLE, PLAN
    };

public:
    Model();

//    bool loadBasicType(BasicType type);

    bool loadFromFile(const std::string& filename); // Later, loadFrom


    bool loadBasicType(BasicType type);
    bool loadFullscreenQuad();

    void draw(Shader* s = nullptr);
    void drawAsTriangles(Shader* s = nullptr);
    void drawAsPatch(Shader* s = nullptr);

    void draw(const mat4& projection, const mat4& view, const mat4& model, Shader* s = nullptr);
    void drawAsTriangles(const mat4& projection, const mat4& view, const mat4& model, Shader* s = nullptr);
    void drawAsPatch(const mat4& projection, const mat4& view, const mat4& model, Shader* s = nullptr);

    void handleNode(aiNode* ainode, const aiMatrix4x4& accumulatedTransformation = aiMatrix4x4(), int level = 0);
};

#endif // MODEL_H
