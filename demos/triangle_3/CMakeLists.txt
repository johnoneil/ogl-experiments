add_executable (triangle_3 main.cpp)

find_package(OpenGL REQUIRED)
#find_package(glfw REQUIRED)

target_link_libraries(triangle_3 framework)