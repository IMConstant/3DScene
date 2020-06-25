//
// Created by Koko on 20.06.2020.
//

#include "Transform.h"


Transform::Transform() : m_transform(glm::mat4(1.0f)) {}

Transform::Transform(glm::mat4 t) : m_transform(t) {}

glm::mat4 Transform::native() const {
    return m_transform;
}

glm::mat4 Transform::translate(float x, float y, float z) {
    m_transform = glm::translate(m_transform, glm::vec3(x, y, z));

    return m_transform;
}

glm::mat4 Transform::rotate(float angle, float x, float y, float z) {
    m_transform = glm::rotate(m_transform, angle, glm::vec3(x, y, z));

    return m_transform;
}

glm::mat4 Transform::scale(float x, float y, float z) {
    m_transform = glm::scale(m_transform, glm::vec3(x, y, z));

    return m_transform;
}

