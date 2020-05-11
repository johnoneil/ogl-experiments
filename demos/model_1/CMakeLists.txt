add_executable (model_1 main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(model_1 OpenGL::GL glfw glm framework assimp)