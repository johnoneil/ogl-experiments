add_executable (cube_2 main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(cube_2 OpenGL::GL glfw glm framework)