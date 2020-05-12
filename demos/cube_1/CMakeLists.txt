add_executable (cube_1 main.cpp)

find_package(OpenGL REQUIRED)
#find_package(glfw REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(cube_1 glm framework)