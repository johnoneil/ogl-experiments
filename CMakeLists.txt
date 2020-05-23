
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ogl VERSION 0.1 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# TODO: configure builds
set(CMAKE_BUILD_TYPE Debug)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "-s ${CIMG_FLAGS}")

#set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

#everyone has access to some headers
include_directories(thirdparty/stb)
include_directories(thirdparty/imgui)

# Framework library
ADD_SUBDIRECTORY(src/framework)

# we want all assets at the root of our build
FILE(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# third party libraries from source
ADD_SUBDIRECTORY (thirdparty)

# TODO: remove auto-generated html in favor of custom template
IF (EMSCRIPTEN)
    SET (BUILD_HTML True)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_ZLIB=1")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_LIBPNG=1")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_BZIP2=1")
ENDIF()

#demos
ADD_SUBDIRECTORY (demos)