SET (PROJECT_NAME triangle_3)

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} framework)

INCLUDE (ogl)
