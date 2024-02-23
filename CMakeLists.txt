
cmake_minimum_required(VERSION 3.13)
project(main)

add_definitions(-std=c++17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -save-temps=obj")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps=obj")

set(GEN_LIB_PATH "${PROJECT_SOURCE_DIR}/dist")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GEN_LIB_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GEN_LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GEN_LIB_PATH})

 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# link_directories("/home/yfxu/lib"  )

file( GLOB APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# message(STATUS ${APP_SOURCES} )
add_executable(main ${APP_SOURCES})  
