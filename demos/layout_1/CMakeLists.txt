SET (PROJECT_NAME layout_1)

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} framework)

# custom ogl.cmake module include
INCLUDE (ogl)
