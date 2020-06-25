//
// Created by Koko on 20.06.2020.
//

#ifndef KR_EGOR_APPLICATION_H
#define KR_EGOR_APPLICATION_H

#include <GL/glew.h>
#include "Shader.h"
#include "Texture.h"
#include "Shape.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


int main(int argc, char **argv);


class Application {
    friend int ::main(int argc, char **argv);

    static Application *s_instance;

    GLFWwindow *m_window;

public:
    Application();
    ~Application();

    static Application *getInstance();

    void imguiInit();

private:
    void run();
    void close();
};


#endif //KR_EGOR_APPLICATION_H
