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

    private:
        uint16_t width;
        uint16_t height;
        std::vector<std::vector<Tile::ptr>> map; /// container for tiles, actual game map
        std::vector<Actor*> actors;         /// container with actors on map

    public:
        typedef std::shared_ptr<Map> ptr;

        //################# CONSTRUCTORS ##################
        Map();

        //############### MAP OPERATIONS ##################
        /**
         * @brief generate_map fills map with randomly generated tiles
         * @param width
         * @param height
         * @param players
         */
        void generate_map(uint8_t width, uint8_t height, std::vector<Actor::ptr>& players);
        Tile::ptr at(Coord pos) const;    ///

        //################## SETTERS ###################

        //################## GETTERS ####################
        std::vector<Actor*> get_actors() const;
        uint16_t get_width() const;
        uint16_t get_height() const;
};

#endif // MAP_H
