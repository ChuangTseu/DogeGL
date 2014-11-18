#include "material.h"




bool Material::loadFromAssimpMaterial(const aiMaterial *mat) {
    //        aiString name;
    //        mat->Get(AI_MATKEY_NAME,name);


//    std::cerr << color.r << ", " << color.g << ", " << color.b << '\n';

    aiColor3D color (0.f,0.f,0.f);
    float value = 0.f;

    assert( AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, color) );
    m_ambientReflectance = color;

    std::cerr << color.r << ", " << color.g << ", " << color.b << '\n';

    assert( AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) );
    m_diffuseReflectance = color;

    assert( AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, color) );
    m_specularReflectance = color;

    assert( AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, value) );
    m_specularExponent = value / 4.f;

//    std::cerr << "Warning, dividing shininess by 4.f, Why Da Fuck ASSIMP ????? \n";

    return true;
}
