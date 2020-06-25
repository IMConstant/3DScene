//
// Created by Koko on 20.06.2020.
//

#include "Texture.h"


Texture::Texture(const std::string &fileName) {
    stbi_set_flip_vertically_on_load(1);
    m_local_buffer = stbi_load(fileName.c_str(), &m_width, &m_height, &m_BPP, 4);

    std::cout << m_width << " " << m_height;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_local_buffer)
        stbi_image_free(m_local_buffer);
}

void Texture::bind(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}