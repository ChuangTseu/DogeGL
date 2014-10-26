#include "model.h"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/config.h>

#include <cassert>

#define BUFFER_OFFSET(p) ((uint8_t*)0 + p)


Model::Model()
{
}

bool Model::loadBasicType(BasicType type) {
    m_vertices.clear();
    m_indices.clear();

    if (type == BasicType::CUBE) {
        m_vertices = {{{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},
                      {{1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, -1.0f}},
                      {{1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, -1.0f}},     // Face 1
                      {{-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, -1.0f}},
                      {{1.0f, -1.0f, 1.0f},   {1.0f, -1.0f, 1.0f}},
                      {{1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 1.0f}},
                      {{-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, 1.0f}},
                      {{-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, 1.0f}} };

        m_indices = {0,   1,   2,
                    0,   3,   2,
                    4,   1,   2,
                    4,   5,   2,
                    6,   4,   1,
                    6,   0,   1,
                    6,   4,   5,
                    6,   7,   5,
                    0,   6,   7,
                    0,   3,   7,
                    7,   5,   2,
                    7,   3,   2};
    }
    else {
        std::cerr << "BasicType (" << static_cast<int>(type) << ") not yet implemented" << '\n';
        return false;
    }

    m_vbo.submitData(m_vertices.data(), m_vertices.size());
    m_ibo.submitData(m_indices.data(), m_indices.size());


    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    m_vbo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    m_ibo.bind();

    glBindVertexArray(0);

    VBO::unbind();
    IBO::unbind();

    return true;
}

bool Model::load()
{
    std::string filename = "cube.obj";

    Assimp::Importer importer;

    // Component to be removed when importing file
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                                aiComponent_TANGENTS_AND_BITANGENTS
                                | aiComponent_COLORS
                                | aiComponent_TEXCOORDS
                                | aiComponent_BONEWEIGHTS
                                | aiComponent_ANIMATIONS
                                | aiComponent_TEXTURES
                                | aiComponent_LIGHTS
                                | aiComponent_CAMERAS
                                | aiComponent_MATERIALS);

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( filename,
        aiProcess_Triangulate
        | aiProcess_JoinIdenticalVertices
        | aiProcess_RemoveComponent
        | aiProcess_GenNormals
        //| aiProcess_MakeLeftHanded
    );
    // If the import failed, report it
    if( !scene)
    {
        std::cerr << importer.GetErrorString() << '\n';
        return false;
    }
    // Now we can access the file's contents.
    //DoTheSceneProcessing( scene);
//    std::cerr << "Model contains " << scene->mNumMeshes << " meshes" << '\n';

    const aiMesh* mesh = scene->mMeshes[0];

//    std::cerr << "Mesh HasFaces ? " << mesh->HasFaces() << '\n';
//    std::cerr << "Mesh HasNormals ? " << mesh->HasNormals() << '\n';
//    std::cerr << "Mesh HasPositions ? " << mesh->HasPositions() << '\n';
//    std::cerr << "Mesh HasTangentsAndBitangents ? " << mesh->HasTangentsAndBitangents() << '\n';
//    std::cerr << "Mesh HasBones ? " << mesh->HasBones() << '\n';
//    //std::cerr << "Mesh HasTextureCoords ? " << mesh->HasTextureCoords() << '\n';
//    //std::cerr << "Mesh HasVertexColors ? " << mesh->HasVertexColors() << '\n';
//    std::cerr << "Mesh GetNumUVChannels ? " << mesh->GetNumUVChannels() << '\n';
//    std::cerr << "Mesh GetNumColorChannels ? " << mesh->GetNumColorChannels() << '\n';
//    std::cerr << "Mesh mNumFaces ? " << mesh->mNumFaces << '\n';
//    std::cerr << "Mesh mNumVertices ? " << mesh->mNumVertices << '\n';
//    std::cerr << "Mesh mPrimitiveTypes ? " << mesh->mPrimitiveTypes << " and should be " << aiPrimitiveType_TRIANGLE << '\n';

    m_vertices.clear();
    m_indices.clear();

    assert( mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);

    for (unsigned int idFace = 0; idFace < mesh->mNumFaces; ++idFace) {
        const aiFace& triangle = mesh->mFaces[idFace];

        for (unsigned int idIndex = 0; idIndex < 3; ++idIndex) {
            m_indices.push_back(triangle.mIndices[idIndex]);
        }
    }

    assert( mesh->HasNormals() );

    for (unsigned int idVertex = 0; idVertex < mesh->mNumVertices; ++idVertex) {
        const aiVector3D& vertex = mesh->mVertices[idVertex];
        const aiVector3D& normal = mesh->mNormals[idVertex];

        Vertex v;
        v.position.x = vertex.x;
        v.position.y = vertex.y;
        v.position.z = vertex.z;

        v.normal.x = normal.x;
        v.normal.y = normal.y;
        v.normal.z = normal.z;

        m_vertices.push_back(v);
    }

    m_vbo.submitData(m_vertices.data(), m_vertices.size());
    m_ibo.submitData(m_indices.data(), m_indices.size());


    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    m_vbo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    m_ibo.bind();

    glBindVertexArray(0);

    VBO::unbind();
    IBO::unbind();

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

void Model::draw() {

    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}
