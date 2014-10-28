#ifndef ENTITY_H
#define ENTITY_H

#include "MathsTools/mat4.h"

#include "model.h"

class Entity
{
    mat4 m_transform;

    Model m_model;

public:
    Entity();


};

#endif // ENTITY_H
