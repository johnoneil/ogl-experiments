add_executable (triangle_3 main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

#include_directories( ${OPENGL_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})
#target_add_include_directories(my_project ${JPEG_INCLUDE_DIR})

target_link_libraries(triangle_3 OpenGL::GL glfw soil)