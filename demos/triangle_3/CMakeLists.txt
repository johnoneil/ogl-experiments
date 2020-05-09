add_executable (triangle_3 main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(triangle_3 OpenGL::GL glfw)