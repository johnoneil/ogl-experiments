find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

if (NOT EMSCRIPTEN)
    find_package(glfw3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)
endif()

if (EMSCRIPTEN)
    if (BUILD_HTML)
        set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    endif()
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--emrun -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 --preload-file ${PROJECT_SOURCE_DIR}/assets/brick.jpg@/assets/brick.jpg")
endif()
