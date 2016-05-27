#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

#include <vector>
#include "tile.h"
#include "map.h"
#include "roomgenerator.h"

class DungeonGenerator {
    private:
        using Area = std::vector<Tile::ptr>;
        Map::ptr map;
        RoomGenerator::prt room_generator;
        std::vector<Area> areas;

    public:
        DungeonGenerator(Map::ptr map);

        void generate();
        void make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size);
        void make_maze();
        void connect_areas(uint8_t connection_chance);
        void remove_deadends();
};

#endif // DUNGEONGENERATOR_H
