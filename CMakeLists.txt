cmake_minimum_required(VERSION 3.29)
project(bitcraft)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(${PROJECT_NAME} main.cpp
        src/algebra3d.h src/algebra3d.cpp
        src/Engine3D.h src/Engine3D.cpp
        src/Screen.cpp
        src/Screen.h)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
