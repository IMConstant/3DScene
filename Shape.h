//
// Created by Koko on 20.06.2020.
//

#ifndef KR_EGOR_SHAPE_H
#define KR_EGOR_SHAPE_H

#include <GL/glew.h>

#include "Transformable.h"
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>


struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
    glm::vec2 UV;

    Vertex() = default;
    explicit Vertex(glm::vec3 _position,
            glm::vec4 _color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
            glm::vec3 _normal = glm::vec3(0.0f),
            glm::vec2 _uv = glm::vec2(0.0f)) :
        position(_position), color(_color),
        normal(_normal),
        UV(_uv) {}

    Vertex(glm::vec3 _position, glm::vec2 uv, glm::vec3 _normal = glm::vec3(0.0f)) :
        position(_position),
        color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
        normal(_normal),
        UV(uv) {}
};


struct Quad {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    Vertex v4;

    void calculateNormals() {
        glm::vec3 A = v3.position - v1.position;
        glm::vec3 B = v2.position - v1.position;
        glm::vec3 N = glm::cross(A, B);

        v1.normal = N;
        v2.normal = N;
        v3.normal = N;
        v4.normal = N;
    }
};

constexpr static const unsigned int QuadIndices[] = {
        0, 2, 1, 1, 2, 3
};


class Shape : public Transformable {
    std::vector<Quad> m_quads;

public:
    Shape() = default;

    void addQuad(Quad quad) {
        m_quads.push_back(quad);
    }

    void draw(Transform transform = Transform()) const {
        std::vector<unsigned int> indices;

        for (int i = 0; i < m_quads.size(); i++) {
            indices.push_back(0 + i * 4);
            indices.push_back(2 + i * 4);
            indices.push_back(1 + i * 4);
            indices.push_back(1 + i * 4);
            indices.push_back(2 + i * 4);
            indices.push_back(3 + i * 4);
        }

        GLuint quadVB, quadTB, quadNB, quadIB;

        glCreateBuffers(1, &quadVB);
        glBindBuffer(GL_ARRAY_BUFFER, quadVB);
        glBufferData(GL_ARRAY_BUFFER, m_quads.size() * 4 * sizeof(Vertex),
                reinterpret_cast<const unsigned char *>(m_quads.data()), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        glCreateBuffers(1, &quadTB);
        glBindBuffer(GL_ARRAY_BUFFER, quadTB);
        glBufferData(GL_ARRAY_BUFFER, m_quads.size() * 4 * sizeof(Vertex),
                reinterpret_cast<const unsigned char *>(m_quads.data()) + (3 + 4 + 3) * sizeof(float), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        glCreateBuffers(1, &quadNB);
        glBindBuffer(GL_ARRAY_BUFFER, quadNB);
        glBufferData(GL_ARRAY_BUFFER, m_quads.size() * 4 * sizeof(Vertex),
                     reinterpret_cast<const unsigned char *>(m_quads.data()) + (3 + 4) * sizeof(float), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        glCreateBuffers(1, &quadIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, 6 * m_quads.size(), GL_UNSIGNED_INT, nullptr);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glDeleteBuffers(1, &quadVB);
        glDeleteBuffers(1, &quadTB);
        glDeleteBuffers(1, &quadNB);
        glDeleteBuffers(1, &quadIB);
    }
};


class BarrelShape : public Shape {
public:
    BarrelShape(float r, int frag_x = 30, int frag_y = 30, float center_offset = 0.0f) : Shape() {
        static const double pi = std::acos(-1);
        float delta_phi = 2 * pi / (float)frag_x;
        float delta_i = 1.0f / (float)frag_y;

        for (float i = 0.0f; i < 1.0f;) {
            for (float phi = 0.0f; phi < 2.0f * pi - 0.0001f;) {
                float _r1 = r + center_offset * std::sin(i * pi);
                float _r2 = r + center_offset * std::sin((i + delta_i) * pi);

                glm::vec3 p1{ _r1 * std::cos(phi), i, _r1 * std::sin(phi) };
                glm::vec3 p2{ _r1 * std::cos(phi + delta_phi), i, _r1 * std::sin(phi + delta_phi) };
                glm::vec3 p3{ _r2 * std::cos(phi), i + delta_i, _r2 * std::sin(phi) };
                glm::vec3 p4{ _r2 * std::cos(phi + delta_phi), i + delta_i, _r2 * std::sin(phi + delta_phi) };

                float tex_x = phi / (float)(2 * pi);
                float tex_y = i;

                Quad quad{};
                quad.v1 = Vertex(p4, glm::vec2((phi + delta_phi) / (2 * pi), (i + delta_i)));
                quad.v2 = Vertex(p3, glm::vec2(phi / (2 * pi), i + delta_i));
                quad.v3 = Vertex(p2, glm::vec2((phi + delta_phi) / (2 * pi), i));
                quad.v4 = Vertex(p1, glm::vec2(phi / (2 * pi), i));

                quad.calculateNormals();
                addQuad(quad);

                phi += delta_phi;
            }

            i += delta_i;
        }

        for (float phi = 0.0f; phi < pi - 0.0001f;) {

            glm::vec3 p1{ r * std::cos(-phi - delta_phi), 1.0f, r * std::sin(-phi - delta_phi) };
            glm::vec3 p2{ r * std::cos(phi + delta_phi), 1.0f, r * std::sin(phi + delta_phi) };
            glm::vec3 p3{ r * std::cos(-phi), 1.0f, r * std::sin(-phi) };
            glm::vec3 p4{ r * std::cos(phi), 1.0f, r * std::sin(phi) };

            Quad quad{};
            quad.v3 = Vertex(p1, glm::vec2(p1.x * 0.5f + 0.5f, p1.z * 0.5f + 0.5f));
            quad.v4 = Vertex(p2, glm::vec2(p2.x * 0.5f + 0.5f, p2.z * 0.5f + 0.5f));
            quad.v1 = Vertex(p3, glm::vec2(p3.x * 0.5f + 0.5f, p3.z * 0.5f + 0.5f));
            quad.v2 = Vertex(p4, glm::vec2(p4.x * 0.5f + 0.5f, p4.z * 0.5f + 0.5f));

            quad.calculateNormals();
            addQuad(quad);

            phi += delta_phi;
        }

        for (float phi = 0.0f; phi < pi - 0.0001f;) {

            glm::vec3 p1{ r * std::cos(-phi - delta_phi), 0.0f, r * std::sin(-phi - delta_phi) };
            glm::vec3 p2{ r * std::cos(phi + delta_phi), 0.0f, r * std::sin(phi + delta_phi) };
            glm::vec3 p3{ r * std::cos(-phi), 0.0f, r * std::sin(-phi) };
            glm::vec3 p4{ r * std::cos(phi), 0.0f, r * std::sin(phi) };

            Quad quad{};
            quad.v4 = Vertex(p1, glm::vec2(p1.x * 0.5f + 0.5f, p1.z * 0.5f + 0.5f));
            quad.v3 = Vertex(p2, glm::vec2(p2.x * 0.5f + 0.5f, p2.z * 0.5f + 0.5f));
            quad.v2 = Vertex(p3, glm::vec2(p3.x * 0.5f + 0.5f, p3.z * 0.5f + 0.5f));
            quad.v1 = Vertex(p4, glm::vec2(p4.x * 0.5f + 0.5f, p4.z * 0.5f + 0.5f));

            quad.calculateNormals();
            addQuad(quad);

            phi += delta_phi;
        }
    }
};


#endif //KR_EGOR_SHAPE_H
