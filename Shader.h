//
// Created by Koko on 20.06.2020.
//

#ifndef KR_EGOR_SHADER_H
#define KR_EGOR_SHADER_H

#include <GL/glew.h>

#include <iostream>
#include <fstream>


class Shader {
    int m_shader_program;

public:
    Shader(const std::string &vertex_source, const std::string &fragment_source);
    Shader(std::pair<std::string, std::string> sources);

    int getId() const;
    void bind();

    static std::pair<std::string, std::string> fromSourceFiles(const std::string &vertex_source_file, const std::string &fragment_source_file);
};


#endif //KR_EGOR_SHADER_H
