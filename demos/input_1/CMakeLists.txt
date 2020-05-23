SET (PROJECT_NAME input_1)

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} framework imgui)

# custom ogl.cmake module include
INCLUDE (ogl)
