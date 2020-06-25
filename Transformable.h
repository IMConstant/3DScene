//
// Created by Koko on 20.06.2020.
//

#ifndef KR_EGOR_TRANSFORMABLE_H
#define KR_EGOR_TRANSFORMABLE_H

#include "Transform.h"


class Transformable {
    glm::mat4 m_translation;
    glm::mat4 m_rotation;
    glm::mat4 m_scale;

public:
    Transformable();

    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void scale(float x, float y, float z);

    void setTranslation(float x, float y, float z);
    void setRotation(float angle, float x, float y, float z);
    void setScale(float x, float y, float z);

    Transform transform() const;
};


#endif //KR_EGOR_TRANSFORMABLE_H
