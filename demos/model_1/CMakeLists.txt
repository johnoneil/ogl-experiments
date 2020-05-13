SET (PROJECT_NAME model_1)

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} framework assimp)

# custom ogl.cmake module include
INCLUDE (ogl)
