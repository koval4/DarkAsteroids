project(DarkAsteroids)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
cmake_minimum_required(VERSION 2.8.)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DarkAsteroids_SOURCE_DIR}/cmake")

# Add _DEBUG_ define
if(CMAKE_BUILD_TYPE MATCHES "Debug")
   add_definitions(-D_DEBUG_)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

# SDL
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR})

include_directories("include" "include/ui")
set(SRC_LIST
  src/main.cpp
  src/common.cpp
  src/game.cpp

  src/ui/gui.cpp
  src/ui/texture.cpp
  src/ui/widget.cpp
  src/ui/screen.cpp
  src/ui/button.cpp
  src/ui/eventhandler.cpp
  src/ui/eventprocessor.cpp
  src/ui/label.cpp
  src/ui/listbox.cpp
  src/ui/listener.cpp
  src/ui/panel.cpp
  src/ui/scrollable.cpp
  src/ui/slidebar.cpp
  src/ui/textbox.cpp
  src/ui/texture.cpp

  src/tile.cpp
  src/map.cpp
  src/mapdrawer.cpp
  src/dungeongenerator.cpp
  src/room.cpp
  src/roomgenerator.cpp

  src/attack.cpp
  src/closeattack.cpp
  src/rangedattack.cpp
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
  src/itemsstack.cpp
  src/weapon.cpp

  src/controllers/controller.cpp
  src/controllers/controllersmanager.cpp
  src/controllers/attackcontroller.cpp
  src/controllers/inventorycontroller.cpp
  src/controllers/npccontroller.cpp
  src/controllers/pickitemcontroller.cpp
  src/controllers/playercontroller.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES} )
