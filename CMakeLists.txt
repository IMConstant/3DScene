cmake_minimum_required(VERSION 3.15)
project(KR_Egor)

set(CMAKE_CXX_STANDARD)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_subdirectory(./dependencies/glew/build/cmake)
add_subdirectory(./dependencies/glfw)
add_subdirectory(./dependencies/imgui/imgui-master)

add_executable(KR_Egor main.cpp
        Application.cpp
        Application.h
        Shader.cpp
        Shader.h
        Texture.cpp
        Texture.h
        dependencies/stb_image/stb_image.h
        dependencies/stb_image/stb_image.cpp Transform.cpp Transform.h Transformable.cpp Transformable.h Shape.cpp Shape.h)
target_include_directories(KR_Egor PUBLIC ./dependencies/glew/include
        ./dependencies/glfw/include
        ./dependencies/glm
        ./dependencies/imgui/imgui-master
        ./dependencies/stb_image)
target_link_libraries(KR_Egor PUBLIC glew_s glfw imgui)