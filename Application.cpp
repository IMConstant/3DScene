//
// Created by Koko on 20.06.2020.
//

#include "Application.h"

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


Application *Application::s_instance = nullptr;

Application::Application() {
    if (s_instance) {
        std::cerr << "Application is already created!";
        throw;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    s_instance = this;
    m_window = glfwCreateWindow(800, 600, "Application", nullptr, nullptr);

    glfwMakeContextCurrent(m_window);
    glewInit();
    glfwSwapInterval(1);
    glfwWindowHint(GLFW_SAMPLES, 4);

    imguiInit();
}

Application::~Application() {

}

void Application::imguiInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;

    static const ImWchar ranges[] = {
                    0x0020, 0x00FF, // Basic Latin + Latin Supplement
                    0x0400, 0x044F, // Cyrillic
                    0,
            };

    io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\Tahoma.ttf)", 18.0f, &font_config, ranges);
}

Application *Application::getInstance() {
    return s_instance;
}



void Application::run() {
    //std::filesystem::path cwd = std::filesystem::current_path();
    //std::cout << cwd.string();

    Shader shader(Shader::fromSourceFiles(
        "./shaders/vertex.vsh",
        "./shaders/fragment.fsh"
    ));

    Shader depth_shader(Shader::fromSourceFiles(
            "./shaders/depth.vsh.glsl",
            "./shaders/depth.fsh.glsl"
    ));

    Shader shadow_shader(Shader::fromSourceFiles(
            "./shaders/shadow.vsh.glsl",
            "./shaders/shadow.fsh.glsl"
    ));

    Shader default_shader(Shader::fromSourceFiles(
            "./shaders/default.vsh.glsl",
            "./shaders/default.fsh.glsl"
    ));

    glEnable (GL_TEXTURE_2D);

    Texture texture("./res/barrel.jpg");
    Texture floorTexture("./res/floor4.jpg");
    Texture wood("./res/wall.jpg");

    BarrelShape barrel(0.4f, 100, 100, 0.15f);
    barrel.scale(0.5f, 0.5f, 0.5f);

    Shape shape;
    float delta = 100.0f / 100.0f;

    for (float z = -50.0f; z < 50.0f;) {
        for (float x = -50.0f; x < 50.0f;) {
            Quad quad{};
            quad.v1 = Vertex(glm::vec3(x, 0.0f, z),
                             glm::vec2(x * 0.5f + 0.5f, 1.0f - z * 0.5f + 0.5f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
            quad.v2 = Vertex(glm::vec3(x + delta, 0.0f, z),
                             glm::vec2((x + delta) * 0.5f + 0.5f, 1.0f - z * 0.5f + 0.5f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
            quad.v3 = Vertex(glm::vec3(x, 0.0f, z + delta),
                             glm::vec2(x * 0.5f + 0.5f, 1.0f - (z + delta) * 0.5f + 0.5f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
            quad.v4 = Vertex(glm::vec3(x + delta, 0.0f, z + delta),
                             glm::vec2((x + delta) * 0.5f + 0.5f, 1.0f - (z + delta) * 0.5f + 0.5f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
            shape.addQuad(quad);

            x += delta;
        }

        z += delta;
    }

    BarrelShape light(0.0f, 41, 14, 2.0f);
    light.setScale(0.02f, 0.07f, 0.02f);

    glm::vec3 light_position(1.0f, 2.0f, -0.5f);
    glm::vec3 camera_position(2.0f, 1.25f, 2.0f);
    glm::vec3 look_position(0.0f, 0.0f, 0.0f);
    glm::vec4 light_color(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Once);

    auto renderScene = [&](Shader &shader) mutable {
        texture.bind(0);
        barrel.setTranslation(0.0f, 0.0f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(barrel.transform().native()));
        barrel.draw();

        barrel.setTranslation(0.0f, 0.5f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(barrel.transform().native()));
        barrel.draw();

        barrel.setTranslation(0.8f, 0.0f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(barrel.transform().native()));
        barrel.draw();

        barrel.setTranslation(0.0f, 0.0f, 0.8f);
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(barrel.transform().native()));
        barrel.draw();

        floorTexture.bind(0);
        shape.setTranslation(0.0f, 0.0f, 0.0f);
        shape.setRotation(0.0f, 0.0f, 0.0f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(depth_shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(glm::mat4(1.0f)));
        shape.draw();

        wood.bind(0);
        shape.setTranslation(0.0f, 0.0f, -1.0f);
        shape.setRotation(90.0f, 1.0f, 0.0f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(depth_shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(shape.transform().native()));
        shape.draw();
        shape.setTranslation(-1.0f, 0.0f, 0.0f);
        shape.setRotation(-90.0f, 0.0f, 0.0f, 1.0f);
        shape.rotate(90, 0.0f, 1.0f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(depth_shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(shape.transform().native()));
        shape.draw();
    };

    float angle = 0.0f;

    while (!glfwWindowShouldClose(m_window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        angle += 0.01;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove);

        {
            ImGui::Text("Light position:");
            ImGui::SliderFloat("X", &light_position.x, -2.0f, 2.0f);
            ImGui::SliderFloat("Y", &light_position.y, -2.0f, 2.0f);
            ImGui::SliderFloat("Z", &light_position.z, -2.0f, 2.0f);
            ImGui::Text("Look position:");
            ImGui::SliderFloat("LX", &look_position.x, -2.0f, 2.0f);
            ImGui::SliderFloat("LY", &look_position.y, -2.0f, 2.0f);
            ImGui::SliderFloat("LZ", &look_position.z, -2.0f, 2.0f);
            ImGui::Text("Camera position:");
            ImGui::SliderFloat("CX", &camera_position.x, -2.0f, 5.0f);
            ImGui::SliderFloat("CY", &camera_position.y, -2.0f, 5.0f);
            ImGui::SliderFloat("CZ", &camera_position.z, -2.0f, 5.0f);
            ImGui::ColorEdit4("light color", reinterpret_cast<float *>(&light_color));
        }

        ImGui::End();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);

        const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //barrel.rotate(0.01f, 1.0f, 0.0f, 0.0f);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), display_w / (float)display_h, 0.1f, 100.f);
        glm::mat4 view = glm::lookAt(camera_position,
                look_position,
                glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 modelViewProjection = projection * view * model;

        glm::mat4 light_projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, -1.0f, 7.5f);
        light_projection = glm::perspective(glm::radians(150.0f), display_w / (float)display_h, 0.2f, 10000.0f);
        glm::mat4 light_view = glm::lookAt(light_position,
                                     glm::vec3(0, 0.0f, 0),
                                     glm::vec3(0, 1, 0));
        glm::mat4 light_space_matrix = light_projection * light_view;

        depth_shader.bind();

        glUniformMatrix4fv(glGetUniformLocation(depth_shader.getId(), "u_light_space_matrix"),
                1,
                GL_FALSE,
                glm::value_ptr(light_space_matrix));
        glUniformMatrix4fv(glGetUniformLocation(depth_shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(barrel.transform().native()));

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

        renderScene(depth_shader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.bind();

        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_modelViewProjectionMatrix"),
                1,
                false,
                glm::value_ptr(modelViewProjection));
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "u_light_space_matrix"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(light_space_matrix));
        glUniform4fv(glGetUniformLocation(shader.getId(), "u_light_color"), 1, glm::value_ptr(light_color));
        glUniform3fv(glGetUniformLocation(shader.getId(), "u_light_position"), 1, glm::value_ptr(light_position));
        glUniform3fv(glGetUniformLocation(shader.getId(), "u_camera_position"), 1, glm::value_ptr(camera_position));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        renderScene(shader);

        default_shader.bind();
        glUniformMatrix4fv(glGetUniformLocation(default_shader.getId(), "u_modelViewProjectionMatrix"),
                           1,
                           false,
                           glm::value_ptr(modelViewProjection));
        light.setTranslation(light_position.x, light_position.y, light_position.z);
        glUniform4fv(glGetUniformLocation(default_shader.getId(), "u_color"), 1, glm::value_ptr(light_color));
        glUniformMatrix4fv(glGetUniformLocation(default_shader.getId(), "u_transform"),
                           1,
                           false,
                           glm::value_ptr(light.transform().native()));
        light.draw();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);

        glDeleteFramebuffers(1, &depthMapFBO);
        glDeleteTextures(1, &depthMap);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}
