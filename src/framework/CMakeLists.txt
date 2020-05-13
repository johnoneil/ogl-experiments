SET (PROJECT_NAME framework)

add_library(${PROJECT_NAME} STATIC shaders.cpp mesh.cpp model.cpp shader.cpp)

target_link_libraries(${PROJECT_NAME} glm assimp)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)

INCLUDE (ogl)