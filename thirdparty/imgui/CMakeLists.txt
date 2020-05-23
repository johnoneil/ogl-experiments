SET (PROJECT_NAME imgui)

SET( SRCS
    imgui.cpp
    imgui_impl_glfw.cpp
    imgui_widgets.cpp
    imgui_draw.cpp
    imgui_impl_opengl3.cpp)

add_library(${PROJECT_NAME} STATIC ${SRCS} )


#target_link_libraries(${PROJECT_NAME} glm assimp freetype easing)

target_link_libraries(${PROJECT_NAME} framework)

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=\<framework/gl.h\> )

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR})

INCLUDE (ogl)