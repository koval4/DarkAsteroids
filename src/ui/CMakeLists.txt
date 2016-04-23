project(ui)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
cmake_minimum_required(VERSION 2.8.)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../../cmake")

# Add _DEBUG_ define
if(CMAKE_BUILD_TYPE MATCHES "Debug")
   add_definitions(-D_DEBUG_)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

aux_source_directory(. UI_SRC_LIST)

# SDL
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR})

add_library(ui ${UI_SRC_LIST})
target_link_libraries(ui ${SDL2_LIBRARY}
                         ${SDL2_IMAGE_LIBRARIES}
                         ${SDL2_TTF_LIBRARIES})
