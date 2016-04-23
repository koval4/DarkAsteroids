project(DarkAsteroids)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
cmake_minimum_required(VERSION 2.8.)

# Add _DEBUG_ define
if(CMAKE_BUILD_TYPE MATCHES "Debug")
   add_definitions(-D_DEBUG_)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

aux_source_directory(src SRC_LIST)
include_directories(src/ui)

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_subdirectory(src/ui)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ui)
