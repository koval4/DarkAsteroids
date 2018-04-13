#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

#include <vector>
#include <list>
#include "common.h"
#include "tile.h"
#include "map.h"
#include "roomgenerator.h"
#include "actormanager.h"
#include "player.h"

class DungeonGenerator {
public:
    DungeonGenerator(Map& map, ActorManager& actor_manager);

    void generate();
    void place_players(std::vector<std::unique_ptr<Player>>&& players);

private:
    using Area = std::vector<Tile*>;
    using AreaIterator = std::vector<Area>::iterator;

    Map& map;
    RoomGenerator room_generator;
    ActorManager& actor_manager;
    std::vector<Rectangle> rooms;
    std::vector<Area> areas;

    Area make_area(Rectangle rect);
    bool is_overlapping_rooms(Rectangle rect);
    bool can_be_passable(const Tile& tile);
    void make_tunnel(Tile* current, Area& area);
    bool is_in_area(const Tile& tile, const Area& area);
    AreaIterator find_area(const Tile& tile);
    void merge_areas(AreaIterator fst, AreaIterator snd);

    void make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size);
    void make_maze();
    void connect_areas(uint8_t connection_chance);
    void remove_deadends();

    void place_npc(uint8_t quantity, uint8_t min_squad_size, uint8_t max_squad_size);
    void place_actors(std::vector<std::unique_ptr<Actor>>&& actors, Rectangle area) const;

};

#endif // DUNGEONGENERATOR_H
