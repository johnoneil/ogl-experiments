add_library(framework STATIC shaders.cpp mesh.cpp model.cpp)

target_link_libraries(framework glm assimp)

target_include_directories(framework SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)