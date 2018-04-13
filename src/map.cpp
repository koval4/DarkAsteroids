#include "map.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "common.h"
#include "tile.h"
#include "ui/screen.h"
#include "ui/gui.h"
#include "npc.h"

//############# STATIC VARIABLES #################

Map* Map::curr_map = nullptr; /// pointer to current game map

//################### CONSTRUCTORS ###################

Map::Map(uint8_t width, uint8_t height)
    : width(width)
    , height(height)
    , map(height, std::vector<Tile::ptr>{width}) {
    for (uint8_t i = 0; i < width; i++) {
        for (uint8_t j = 0; j < height; j++) {
            map[i][j] = std::make_shared<Tile>(Coord(i, j), false, img("tile_wall.png"));
        }
    }

    // connecting tiles
    for (uint16_t i = 1; i < width - 1; i++) {
        for (uint16_t j = 1; j < height - 1; j++) {
            map[i][j]->connect(map[i+1][j]);
            map[i][j]->connect(map[i-1][j]);
            map[i][j]->connect(map[i][j+1]);
            map[i][j]->connect(map[i][j-1]);
        }
    }
}

void Map::place_actor(Coord pos, Actor::ptr actor) {
    map[pos.x][pos.y]->place_actor(actor.get());
    actor->set_map(this);
    actor->set_pos(pos);
    actors.push_back(actor);
}

void Map::add_room(std::shared_ptr<Room> room) {
    rooms.push_back(room);
}
//################### MAP OPERATIONS ##################

//################## SETTERS #####################

//################# GETTERS ###################

Tile::ptr Map::at(Coord pos) const {
    return map[pos.x][pos.y];
}

std::vector<Actor::ptr> Map::get_actors() const {
    return actors;
}

uint8_t Map::get_width() const {
    return width;
}

uint8_t Map::get_height() const {
    return height;
}
