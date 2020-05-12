add_executable (cube_3 main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(cube_3 OpenGL::GL glfw glm framework)