add_library(framework STATIC shaders.cpp)



target_include_directories(framework SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)