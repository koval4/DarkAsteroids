#include "roomgenerator.h"
#include <string>
#include <vector>
#include <memory>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "room.h"

RoomGenerator::RoomGenerator(Map::ptr map)
    : map(map) {}

void RoomGenerator::generate(Rectangle square) {
    // choosing textures of floor and walls
    // hardcoded as for now
    std::string floor_texture = img("tile_floor.png");
    std::string wall_texture = img("tile_wall.png");

    // making walls
    auto make_wall = [&square, this, &wall_texture] (uint8_t i, uint8_t j) -> void {
        Tile::ptr tile = map->at({i, j});
        tile->set_passable(false);
        tile->set_texture(wall_texture);
    };

    for (uint8_t i = square.first.x; i <= square.last.x; i++) {
        make_wall(i, square.first.y);
        make_wall(i, square.last.y);
    }
    for (uint8_t i = square.first.y; i <= square.last.y; i++) {
        make_wall(square.first.x, i);
        make_wall(square.last.x, i);
    }

    // making floor
    for (uint8_t i = square.first.x + 1; i < square.last.x; i++) {
        for (uint8_t j = square.first.y + 1; j++ < square.last.y; j++) {
            Tile::ptr tile = map->at({i, j});
            tile->set_passable(true);
            tile->set_texture(floor_texture);
        }
    }

    put_items();
    place_npc();
}

void RoomGenerator::put_items() {

}

void RoomGenerator::place_npc() {

}
