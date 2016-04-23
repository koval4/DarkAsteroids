#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <memory>
#include "data.h"
#include "actor.h"
#include "item.h"

//class Actor;

/**
 * @brief The Map class
 */
class Map {
    public:
        /**
         * @brief The Tile struct describes single tile of map
         */
        struct Tile {
			bool passable;                  /// does actor could walk in tile
			bool visible;                   /// does player see this tile
			std::string texture;            /// path to texture of this tile
			Actor::ptr actor;               /// actor that stands in this tile
			std::vector<Item::ptr> items;   /// items that lying in this tile

            static uint16_t tile_size;  /// size of tile's side in pixels

            Tile();
        };

        static Map* curr_map;	/// pointer to current game map

    private:
        std::vector<std::vector<Tile>> map; /// container for tiles, actual game map
        Coord viewpoint;                    /// center of player's view
        std::vector<Actor*> actors;         /// container with actors on map

    public:
        typedef std::unique_ptr<Map> ptr;

        //################# CONSTRUCTORS ##################
        Map();

        //############### MAP OPERATIONS ##################
        /**
         * @brief generate_map fills map with randomly generated tiles
         * @param width
         * @param height
         * @param players
         */
        void generate_map(uint16_t width, uint16_t height, std::vector<Actor::ptr>& players);
        void draw_map();        /// renders map on screen
        Tile& at(Coord pos);    ///
        inline bool is_passable_at(Coord pos) const;

        //################## SETTERS ###################
        void set_viewpoint(Coord viewpoint);

        //################## GETTERS ####################
        Coord get_viewpoint() const;
        std::vector<Actor*> get_actors() const;

        //############# GAME LOGIC #################
        bool is_in_sight(Coord start, Coord end);
};

bool Map::is_passable_at(Coord pos) const {
    const Tile* tile = &map[pos.x][pos.y];
    return tile->passable && !tile->actor;
}

#endif // MAP_H
