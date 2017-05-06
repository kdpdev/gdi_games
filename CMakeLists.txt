cmake_minimum_required(VERSION 2.8)

# guard against in-source builds
#if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
#    message(FATAL_ERROR "In-source builds not allowed.")
#endif()

set(SOURCE_DIRS
    src
    src/engine
    src/engine/core
    src/games
    src/games/utils
)

set(PROJECT_NAME GdiGames)
project(${PROJECT_NAME})
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/${CMAKE_BUILD_TYPE})

foreach(DIR ${SOURCE_DIRS})
    file(GLOB SOURCES_CPP ${DIR}/*.cpp)
    file(GLOB SOURCES_H ${DIR}/*.h)
    source_group(${DIR} FILES ${SOURCES_CPP} ${SOURCES_H})
    set(SOURCES ${SOURCES} ${SOURCES_CPP} ${SOURCES_H})
endforeach()

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
endif()

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
