#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <memory>
#include "common.h"
#include "tile.h"
#include "actor.h"
#include "item.h"
#include "room.h"

//class Actor;

/**
 * @brief The Map class
 */
class Map {
    public:
        static Map* curr_map;	/// pointer to current game map
        typedef std::shared_ptr<Map> ptr;

    private:
        uint8_t width;
        uint8_t height;
        std::vector<std::vector<Tile::ptr>> map; /// container for tiles, actual game map
        std::vector<Actor::ptr> actors;         /// container with actors on map
        std::vector<std::shared_ptr<Room>> rooms;

    public:
        //################# CONSTRUCTORS ##################
        Map(uint8_t width, uint8_t height);

        //############### MAP OPERATIONS ##################

        //################## SETTERS ###################
        void place_actor(Coord pos, Actor::ptr actor);
        void add_room(std::shared_ptr<Room> room);

        //################## GETTERS ####################
        Tile::ptr at(Coord pos) const;    ///
        std::vector<Actor::ptr> get_actors() const;
        uint8_t get_width() const;
        uint8_t get_height() const;
};

#endif // MAP_H
