#include "dungeongenerator.h"
#include <vector>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "room.h"
#include "roomgenerator.h"

DungeonGenerator::DungeonGenerator(Map::ptr map)
    : map(map) {
    room_generator.reset(new RoomGenerator(map));
}

bool DungeonGenerator::is_overlapping_areas(Rectangle rect) {
    for (auto& area : areas)
        if (rect.is_overlapping(area))
            return true;
    return false;
}

bool DungeonGenerator::can_be_passable(Tile::ptr tile) {
    for (auto& adjacent : tile->get_adjacent())
        if (adjacent->is_passable())
            return false;
    return true;
}

void DungeonGenerator::make_tunnel(Tile::ptr start) {
    start->set_passable(true);
    auto adjacent = start->get_adjacent();

    // removing tiles, which cannot be passable
    auto tile_iter = adjacent.begin();
    while (tile_iter != adjacent.end()) {
        if (!can_be_passable(*tile_iter))
            adjacent.erase(tile_iter);
        else tile_iter++;
    }

    if (adjacent.empty())
        return;

    // continuing tunnel in random direction
    auto next = adjacent[rand()%adjacent.size()];
    make_tunnel(next);
}

void DungeonGenerator::make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size) {
    areas.reserve(attempts);
    for (uint16_t i = 0; i < attempts; i++) {
        Rectangle room_square;
        // statatic casts just to silence -Wc++11-narrowing issue
        room_square.first = { static_cast<uint8_t>(rand()%map->get_width()),
                              static_cast<uint8_t>(rand()%map->get_height()) };
        uint8_t width = rand()%max_size + min_size;
        uint8_t height = rand()%max_size + min_size;
        room_square.last = { static_cast<uint8_t>(room_square.first.x + width),
                             static_cast<uint8_t>(room_square.first.y + height) };

        if (is_overlapping_areas(room_square))
            continue;

        areas.push_back(room_square);

        // making room
        room_generator->generate(room_square);
    }
}

void DungeonGenerator::make_maze() {
    for (uint8_t i = 0; i < map->get_height() - 1; i++)
        for (uint8_t j = 0; j < map->get_width() - 1; j++)
            if (can_be_passable(map->at({j, i})))
                make_tunnel(map->at({j, i}));
}

void DungeonGenerator::connect_areas(uint8_t connection_chance) {

}

void DungeonGenerator::remove_deadends() {

}

void DungeonGenerator::generate() {
    make_rooms(30, 5, 7);
    make_maze();
    connect_areas(35);
    remove_deadends();
}
