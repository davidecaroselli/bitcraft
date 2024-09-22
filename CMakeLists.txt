cmake_minimum_required(VERSION 3.29)
project(bitcraft)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(${PROJECT_NAME} main.cpp
        src/utils/stb_image.h
        src/core/Shaders.h src/core/Shaders.cpp
        src/Window.h src/Window.cpp
        src/InputController.h src/InputController.cpp
        src/Model.h src/Model.cpp
)

find_package(GLEW 2.0 REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${GLM_LIBRARIES})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders)