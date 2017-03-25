project(DarkAsteroids)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
cmake_minimum_required(VERSION 2.8.)

# Add _DEBUG_ define
if(CMAKE_BUILD_TYPE MATCHES "Debug")
   add_definitions(-D_DEBUG_)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

include_directories(include src/ui)
set(SRC_LIST
  src/main.cpp
  src/data.cpp
  src/game.cpp

  src/tile.cpp
  src/map.cpp
  src/map_drawer.cpp
  src/dungeongenerator.cpp
  src/room.cpp
  src/roomgenerator.cpp

  src/attack.cpp
  src/close_attack.cpp
  src/ranged_attack.cpp
  src/body.cpp
  src/bodypart.cpp
  src/group.cpp
  src/groupsmanager.cpp
  src/inventory.cpp
  src/race.cpp
  src/actor.cpp
  src/npc.cpp
  src/npcfactory.cpp
  src/npcprototype.cpp
  src/player.cpp
  src/actormanager.cpp

  src/item.cpp
  src/armor.cpp
  src/container.cpp
  src/stack.cpp
  src/weapon.cpp

  src/controller.cpp
  src/controllersmanager.cpp
  src/attackcontroller.cpp
  src/inventorycontroller.cpp
  src/npccontroller.cpp
  src/pickitemcontroller.cpp
  src/playercontroller.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_subdirectory(src/ui)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ui)
