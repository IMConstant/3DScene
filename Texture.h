//
// Created by Koko on 20.06.2020.
//

#ifndef KR_EGOR_TEXTURE_H
#define KR_EGOR_TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>

#include <stb_image.h>


class Texture {
    unsigned int m_id;

    unsigned char *m_local_buffer;
    int m_width;
    int m_height;
    int m_BPP;
public:
    explicit Texture(const std::string &fileName);

    void bind(int index = 0);
    void unbind();
};


#endif //KR_EGOR_TEXTURE_H
