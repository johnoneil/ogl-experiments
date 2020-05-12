add_executable (triangle_2 main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(triangle_2 OpenGL::GL glfw framework)