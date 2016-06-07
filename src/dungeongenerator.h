#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

#include <vector>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "roomgenerator.h"

class DungeonGenerator {
    private:
        Map::ptr map;
        RoomGenerator::prt room_generator;
        std::vector<Rectangle> areas;

        bool is_overlapping_areas(Rectangle rect);
        bool can_be_passable(Tile::ptr tile);
        void make_tunnel(Tile::ptr start);

        void make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size);
        void make_maze();
        void connect_areas(uint8_t connection_chance);
        void remove_deadends();

    public:
        DungeonGenerator(Map::ptr map);

        void generate();
};

#endif // DUNGEONGENERATOR_H
