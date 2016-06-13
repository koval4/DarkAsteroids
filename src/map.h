#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <memory>
#include "data.h"
#include "tile.h"
#include "actor.h"
#include "item.h"

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

    public:
        //################# CONSTRUCTORS ##################
        Map(uint8_t width, uint8_t height);

        //############### MAP OPERATIONS ##################

        //################## SETTERS ###################
        void place_actor(Coord pos, Actor::ptr actor);

        //################## GETTERS ####################
        Tile::ptr at(Coord pos) const;    ///
        std::vector<Actor::ptr> get_actors() const;
        uint8_t get_width() const;
        uint8_t get_height() const;
};

#endif // MAP_H
