//
// Created by Koko on 20.06.2020.
//

#ifndef KR_EGOR_TRANSFORM_H
#define KR_EGOR_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Transform {
    glm::mat4 m_transform;

public:
    Transform();
    Transform(glm::mat4 t);
    glm::mat4 native() const;

    glm::mat4 translate(float x, float y, float z);
    glm::mat4 rotate(float angle, float x, float y, float z);
    glm::mat4 scale(float x, float y, float z);
};


#endif //KR_EGOR_TRANSFORM_H
