add_executable (cube_2 main.cpp)

find_package(OpenGL REQUIRED)
#find_package(glfw REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(cube_2 glm framework)