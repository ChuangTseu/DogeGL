#include "model.h"

#include <cassert>

#define BUFFER_OFFSET(p) ((uint8_t*)0 + p)


Model::Model()
{
}

bool Model::loadBasicType(BasicType type) {
//    m_vertices.clear();
//    m_indices.clear();

//    if (type == BasicType::CUBE) {
//        m_vertices = {{{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {0,0}},
//                      {{1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, -1.0f}, {0,0}},
//                      {{1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, -1.0f}, {0,0}},     // Face 1
//                      {{-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, -1.0f}, {0,0}},
//                      {{1.0f, -1.0f, 1.0f},   {1.0f, -1.0f, 1.0f}, {0,0}},
//                      {{1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 1.0f}, {0,0}},
//                      {{-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, 1.0f}, {0,0}},
//                      {{-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, 1.0f}, {0,0}} };

//        m_indices = {0,   1,   2,
//                    0,   3,   2,
//                    4,   1,   2,
//                    4,   5,   2,
//                    6,   4,   1,
//                    6,   0,   1,
//                    6,   4,   5,
//                    6,   7,   5,
//                    0,   6,   7,
//                    0,   3,   7,
//                    7,   5,   2,
//                    7,   3,   2};
//    }
//    else if (type == BasicType::PLAN) {
//        m_vertices = {{{0.0f, -1.0f, 1.0f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//                      {{0.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//                      {{0.0f, 1.0f, 1.0f},   {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//                      {{0.0f, 1.0f, -1.0f},  {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}};

//        m_indices = {0,   1,   2,
//                     1,   2,   3};
//    }
//    else {
//        std::cerr << "BasicType (" << static_cast<int>(type) << ") not yet implemented" << '\n';
//        return false;
//    }

//    m_vbo.submitData(m_vertices.data(), m_vertices.size());
//    m_ibo.submitData(m_indices.data(), m_indices.size());


//    glGenVertexArrays(1, &m_vao);
//    glBindVertexArray(m_vao);

//    m_vbo.bind();
//    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
//    glEnableVertexAttribArray(0);

//    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
//    glEnableVertexAttribArray(1);

//    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), BUFFER_OFFSET(2*sizeof(vec3)));
//    glEnableVertexAttribArray(2);

//    m_ibo.bind();

//    glBindVertexArray(0);

//    VBO::unbind();
//    IBO::unbind();

    return true;
}

void aiMat4ToDogeMat4(const aiMatrix4x4& aiMat4, mat4& dogeMat4) {
    for (int i = 0 ; i < 4; ++i) {
        for (int j = 0 ; j < 4; ++j) {
            dogeMat4[j][i] = aiMat4[i][j];
        }
    }
}

void Model::handleNode(aiNode* ainode, const aiMatrix4x4& accumulatedTransformation, int level) {
//    std::cerr << "LVL " << level << " Node is: " << ainode->mName.C_Str() << " and has "
//              << ainode->mNumChildren << " children and " << ainode->mNumMeshes << " meshes." << "\n";

    aiMatrix4x4t<float> transformation =  accumulatedTransformation * ainode->mTransformation;

//    for (int i = 0 ; i < 4; ++i) {
//        for (int j = 0 ; j < 4; ++j) {
//            std::cerr << ainode->mTransformation[i][j] << " ";
//        }
//        std::cerr << '\n';
//    }

//    std::cerr << '\n';

    if (ainode->mNumMeshes > 0) {
        ModelNode current;

        for (unsigned int i = 0; i < ainode->mNumMeshes; ++i) {
            current.m_meshIndices.push_back(ainode->mMeshes[i]);
        }

        aiMat4ToDogeMat4(transformation, current.m_transformation);

        m_nodes.push_back(current);
    }


    for (unsigned int i = 0; i < ainode->mNumChildren; ++i) {
        handleNode(ainode->mChildren[i], transformation, level + 1);
    }

//    std::cerr << "END OF " << ainode->mName.C_Str() << "\n\n";

}

bool Model::loadFromFile(const std::string& filename)
{
    Assimp::Importer importer;

    // Component to be removed when importing file
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                                aiComponent_COLORS
//                                | aiComponent_TANGENTS_AND_BITANGENTS
                                | aiComponent_BONEWEIGHTS
                                | aiComponent_ANIMATIONS
                                | aiComponent_LIGHTS
                                | aiComponent_CAMERAS);

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( filename,
        aiProcess_Triangulate
        | aiProcess_JoinIdenticalVertices
        | aiProcess_RemoveComponent
        | aiProcess_GenNormals
        | aiProcess_CalcTangentSpace
        //| aiProcess_GenUVCoords
        //| aiProcess_MakeLeftHanded
    );


//    const aiScene* scene = importer.ReadFile( filename,
//                                              aiProcessPreset_TargetRealtime_MaxQuality
//    );

    // If the import failed, report it
    if( !scene)
    {
        std::cerr << importer.GetErrorString() << '\n';
        return false;
    }    

    aiNode* rootNode = scene->mRootNode;

    handleNode(rootNode, aiMatrix4x4());

    m_meshes.clear();
    m_meshes.resize(scene->mNumMeshes);

    m_materials.clear();
    m_materials.resize(scene->mNumMaterials);

    std::cerr << filename << '\n';
    std::cerr << "Model contains " << scene->mNumMeshes << " meshes" << '\n';
    std::cerr << "Model contains " << scene->mNumMaterials << " materials" << '\n';
//    std::cerr << '\n';

    std::string dir = filename;

    size_t lastSeparator = dir.find_last_of('/');

//    std::cerr << "Last / pos: " << lastSeparator << '\n';

    if (lastSeparator != std::string::npos) {
        dir.erase(lastSeparator, dir.size());
    }
    else {
        dir = std::string(".");
    }

//    std::cerr << "Dir is: " << dir << '\n';

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        m_meshes[i].loadFromAssimpMesh(scene->mMeshes[i]);
    }

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        m_materials[i].loadFromAssimpMaterial(scene->mMaterials[i], dir);
    }

//    const aiMesh* mesh = scene->mMeshes[0];

//    std::cerr << "Mesh HasFaces ? " << mesh->HasFaces() << '\n';
//    std::cerr << "Mesh HasNormals ? " << mesh->HasNormals() << '\n';
//    std::cerr << "Mesh HasPositions ? " << mesh->HasPositions() << '\n';
//    std::cerr << "Mesh HasTangentsAndBitangents ? " << mesh->HasTangentsAndBitangents() << '\n';
//    std::cerr << "Mesh HasBones ? " << mesh->HasBones() << '\n';
//    std::cerr << "Mesh HasTextureCoords ? " << mesh->HasTextureCoords(0) << '\n';
//    std::cerr << "mesh->mTextureCoords[0] != NULL ? " << (mesh->mTextureCoords[0] != NULL) << '\n';
//    std::cerr << "Mesh GetNumUVChannels ? " << mesh->GetNumUVChannels() << '\n';
//    std::cerr << "Mesh GetNumColorChannels ? " << mesh->GetNumColorChannels() << '\n';
//    std::cerr << "Mesh mNumFaces ? " << mesh->mNumFaces << '\n';
//    std::cerr << "Mesh mNumVertices ? " << mesh->mNumVertices << '\n';
//    std::cerr << "Mesh mPrimitiveTypes ? " << mesh->mPrimitiveTypes << " and should be " << aiPrimitiveType_TRIANGLE << '\n';

    return true;
}

void Model::draw(Shader *s) const {
    drawAsTriangles(s);
}

void Model::drawAsPatch(Shader *s) const {
    for (const ModelNode& node : m_nodes) {
        for (unsigned int meshIndex : node.m_meshIndices) {
            const Mesh& mesh = m_meshes[meshIndex];

            if (s) {
                s->sendMaterial(m_materials[mesh.m_materialIndex]);
            }
            mesh.drawAsPatch();
        }
    }
}

void Model::drawAsTriangles(Shader *s) const {
    for (const ModelNode& node : m_nodes) {
        for (unsigned int meshIndex : node.m_meshIndices) {
            const Mesh& mesh = m_meshes[meshIndex];

            if (s) {
                s->sendMaterial(m_materials[mesh.m_materialIndex]);
            }
            mesh.drawAsTriangles();
        }
    }

//    for (const Mesh& mesh : m_meshes) {
//        if (s) {
//            s->sendMaterial(m_materials[mesh.m_materialIndex]);
//        }
//        mesh.drawAsTriangles();
//    }
}


void Model::draw(const mat4 &projection, const mat4 &view, const mat4 &model, Shader *s) const {
    drawAsTriangles(projection, view, model, s);
}

void Model::drawAsPatch(const mat4 &projection, const mat4 &view, const mat4 &model, Shader *s) const {
    for (const ModelNode& node : m_nodes) {
        for (unsigned int meshIndex : node.m_meshIndices) {
            const Mesh& mesh = m_meshes[meshIndex];

            if (s) {
                s->sendMaterial(m_materials[mesh.m_materialIndex]);

                s->sendTransformations(projection, view, model * node.m_transformation);
            }
            mesh.drawAsPatch();
        }
    }
}

void Model::drawAsTriangles(const mat4 &projection, const mat4 &view, const mat4 &model, Shader *s) const {
    for (const ModelNode& node : m_nodes) {
        for (unsigned int meshIndex : node.m_meshIndices) {
            const Mesh& mesh = m_meshes[meshIndex];

            if (s) {
                s->sendMaterial(m_materials[mesh.m_materialIndex]);

                s->sendTransformations(projection, view, model * node.m_transformation);
            }
            mesh.drawAsTriangles();
        }
    }

//    for (const Mesh& mesh : m_meshes) {
//        if (s) {
//            s->sendMaterial(m_materials[mesh.m_materialIndex]);
//        }
//        mesh.drawAsTriangles();
//    }
}


