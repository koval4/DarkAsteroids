#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

#include <vector>
#include <list>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "roomgenerator.h"
#include "player.h"

class DungeonGenerator {
    private:
        using Area = std::vector<Tile::ptr>;
        using AreaIterator = std::vector<Area>::iterator;

        Map::ptr map;
        RoomGenerator::prt room_generator;
        std::vector<Rectangle> rooms;
        std::vector<Area> areas;

        Area make_area(Rectangle rect);
        bool is_overlapping_rooms(Rectangle rect);
        bool can_be_passable(Tile::ptr tile);
        void make_tunnel(Tile::ptr current, Area& area);
        bool is_in_area(Tile::ptr tile, const Area& area);
        AreaIterator find_area(Tile::ptr tile);
        void merge_areas(AreaIterator fst, AreaIterator snd);

        void make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size);
        void make_maze();
        void connect_areas(uint8_t connection_chance);
        void remove_deadends();

    public:
        DungeonGenerator(Map::ptr map);

        void generate();
        void place_players(std::list<Player::ptr> players);
};

#endif // DUNGEONGENERATOR_H
