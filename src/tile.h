#ifndef TILE_H
#define TILE_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "data.h"
#include "actor.h"
#include "item.h"

class Tile {
    public:
        typedef std::shared_ptr<Tile> ptr;
        static uint16_t size;   /// size of tile's side in pixels
        static uint16_t volume; /// volume of space in tile

    private:
        Coord pos;                              /// coordinates of tile on map
        bool passable;                          /// does actor could walk in tile
        std::string texture;                    /// path to texture of this tile
        Actor::ptr actor;                       /// actor that stands in this tile
        std::vector<Item::ptr> items;           /// items that lying in this tile
        std::map<Coord, Tile::ptr> adjacent;    /// tiles that adjacent to this tile

    public:
        //################ CONSTRUCTORS ##################
        Tile();
        Tile(Coord pos, bool passable, std::string texture);

        //################# GETTERS ################
        Coord get_pos() const;
        inline bool is_passable() const;
        std::string get_texture() const;
        std::vector<Item::ptr*> get_items();
        Actor::ptr& get_actor();
        std::vector<Tile::ptr> get_adjacent() const;
        Tile::ptr get_tile(Coord coord) const;

        //############## SETTERS ################
        void set_texture(std::string texture);

        //############# ITEMS OPERATIONS ################
        Item::ptr take_item(Item::ptr& item);
        void put_item(Item::ptr item);

        //############ ACTORS OPERATIONS ##############
        Actor::ptr remove_actor();
        void place_actor(Actor::ptr actor);

        //############# TILES OPERATIONS #################
        void connect(Tile::ptr tile);
};

bool Tile::is_passable() const {
    return passable && !actor;
}

#endif // TILE_H
