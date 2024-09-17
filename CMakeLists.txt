cmake_minimum_required(VERSION 3.29)
project(bitcraft)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(${PROJECT_NAME} main.cpp
        src/Engine3D.h src/Engine3D.cpp
        src/Screen.h src/Screen.cpp
        src/geometry/mesh.h src/geometry/mesh.cpp
        src/geometry/vertex_t.h
        src/geometry/matrix_t.h src/geometry/matrix_t.cpp
        src/geometry/face_t.h src/geometry/face_t.cpp)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
