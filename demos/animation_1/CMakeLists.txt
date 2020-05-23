SET (PROJECT_NAME animation_1)

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} framework imgui rlottie)

# custom ogl.cmake module include
INCLUDE (ogl)
