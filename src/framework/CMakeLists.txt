SET (PROJECT_NAME framework)

#find_package(Freetype REQUIRED)

SET( SRCS
    shaders.cpp
    mesh.cpp
    model.cpp
    shader.cpp
    text.cpp
    color.cpp)

add_library(${PROJECT_NAME} STATIC ${SRCS} )

target_link_libraries(${PROJECT_NAME} glm assimp freetype)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)

INCLUDE (ogl)