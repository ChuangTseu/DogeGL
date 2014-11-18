#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/config.h>

#include <iostream>

struct Color {
    union {
        float data[3];
        struct {
            float r, g, b;
        };
    };

    Color& operator=(const aiColor3D& color) {
        r = color.r;
        g = color.g;
        b = color.b;

        return *this;
    }
};

struct Material
{
    Color m_ambientReflectance;
    Color m_diffuseReflectance;
    Color m_specularReflectance;

    float m_specularExponent; // Shininess

public:
    Material() {
    }

    bool loadFromAssimpMaterial(const aiMaterial* mat);
};

#endif // MATERIAL_H
