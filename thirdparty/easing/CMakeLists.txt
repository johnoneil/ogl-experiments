SET (PROJECT_NAME easing)

SET( SRCS
    easing.c)

add_library(${PROJECT_NAME} STATIC ${SRCS} )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
