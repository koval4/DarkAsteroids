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

void DungeonGenerator::generate() {
    make_rooms(30, 5, 7);
    make_maze();
    connect_areas(35);
    remove_deadends();
}

void DungeonGenerator::make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size) {
    std::vector<Rectangle> placed_rooms;
    placed_rooms.reserve(attempts);
    for (uint16_t i = 0; i < attempts; i++) {
        Rectangle room_square;
        room_square.first = { rand()%map->get_width(), rand()%map->get_height() };
        uint8_t width = rand()%max_size + min_size;
        uint8_t height = rand()%max_size + min_size;
        room_square.last = { room_square.first.x + width, room_square.first.y + height };

        bool overlapping = false;
        for (auto& room : placed_rooms) {
            if (room_square.is_overlapping(room)) {
                overlapping = true;
                break;
            }
        }
        if (overlapping)
            continue;

        placed_rooms.push_back(room_square);

        // making room
        room_generator->generate(room_square);
    }
}

void DungeonGenerator::make_maze() {

}

void DungeonGenerator::connect_areas(uint8_t connection_chance) {

}

void DungeonGenerator::remove_deadends() {

}
