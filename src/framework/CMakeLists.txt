SET (PROJECT_NAME framework)

#find_package(Freetype REQUIRED)

SET( SRCS
    canvaselement.cpp
    shaders.cpp
    mesh.cpp
    model.cpp
    shader.cpp
    text.cpp
    color.cpp
    colorrect.cpp
    stage.cpp
    tween.cpp
    image.cpp)

add_library(${PROJECT_NAME} STATIC ${SRCS} )

target_link_libraries(${PROJECT_NAME} glm assimp freetype easing)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)

INCLUDE (ogl)