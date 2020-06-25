//
// Created by Koko on 20.06.2020.
//

#include "Transformable.h"


Transformable::Transformable() :
    m_translation(glm::mat4(1.0f)), m_rotation(glm::mat4(1.0f)), m_scale(glm::mat4(1.0f)) {

}

void Transformable::translate(float x, float y, float z) {
    m_translation = glm::translate(m_translation, glm::vec3(x, y, z));
}

void Transformable::rotate(float angle, float x, float y, float z) {
    m_rotation = glm::rotate(m_rotation, glm::radians(angle), glm::vec3(x, y, z));
}

void Transformable::scale(float x, float y, float z) {
    m_scale = glm::scale(m_scale, glm::vec3(x, y, z));
}

void Transformable::setTranslation(float x, float y, float z) {
    m_translation = glm::mat4(1.0f);
    translate(x, y, z);
}

void Transformable::setRotation(float angle, float x, float y, float z) {
    m_rotation = glm::mat4(1.0f);
    rotate(angle, x, y, z);
}

void Transformable::setScale(float x, float y, float z) {
    m_scale = glm::mat4(1.0f);
    scale(x, y, z);
}

Transform Transformable::transform() const {
    return m_translation * m_rotation * m_scale;
}
