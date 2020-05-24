ADD_SUBDIRECTORY(glm)
ADD_SUBDIRECTORY(freetype2)
#SET(DASSIMP_BUILD_ASSIMP_TOOLS, OFF)
ADD_SUBDIRECTORY(assimp)
#ADD_SUBDIRECTORY(yoga)
ADD_SUBDIRECTORY(easing)
ADD_SUBDIRECTORY(imgui)
ADD_SUBDIRECTORY(rlottie)
target_include_directories(rlottie
    PUBLIC
        "${PROJECT_SOURCE_DIR}/thirdparty/rlottie/inc"
    )

