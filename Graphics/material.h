#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/config.h>

#include <iostream>

#include "MathsTools/vec3.h"

#include "texture.h"

#include <string>


struct Material
{
    Color3f m_ambientReflectance;
    Color3f m_diffuseReflectance;
    Color3f m_specularReflectance;

    float m_specularExponent; // Shininess

    Texture m_diffuseTexture;
    Texture m_normalTexture;

public:
    Material() {
    }

    bool loadFromAssimpMaterial(const aiMaterial* mat, std::string matBaseDir);
};

#endif // MATERIAL_H
