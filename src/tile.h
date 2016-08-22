#ifndef TILE_H
#define TILE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "data.h"
#include "item.h"

class Actor;

class Tile : public std::enable_shared_from_this<Tile> {
    public:
        using ptr  = std::shared_ptr<Tile>;
        static uint16_t size;   /// size of tile's side in pixels
        static uint16_t volume; /// volume of space in tile

    private:
        Coord pos;                                      /// coordinates of tile on map
        bool passable;                                  /// does actor could walk in tile
        std::string texture;                            /// path to texture of this tile
        std::shared_ptr<Actor> actor;                   /// actor that stands in this tile
        std::vector<Item::ptr> items;                   /// items that lying in this tile
        std::unordered_map<Coord, Tile::ptr> adjacent;  /// tiles that adjacent to this tile

    public:
        //################ CONSTRUCTORS ##################
        Tile();
        Tile(Coord pos, bool passable, std::string texture);

        //############### DESTRUCTOR ################
        ~Tile();

        //################# GETTERS ################
        Coord get_pos() const;
        inline bool is_passable() const;
        bool is_wall() const;
        std::string get_texture() const;
        std::vector<Item::ptr> get_items() const;
        const std::shared_ptr<Actor>& get_actor();
        std::vector<Tile::ptr> get_adjacent() const;
        Tile::ptr get_tile(Coord coord) const;

        //############## SETTERS ################
        void set_texture(std::string texture);
        void set_passable(bool passable);

        //############# ITEMS OPERATIONS ################
        Item::ptr take_item(Item::ptr& item);
        void put_item(Item::ptr item);

        //############ ACTORS OPERATIONS ##############
        std::shared_ptr<Actor> remove_actor();
        void place_actor(const std::shared_ptr<Actor>& actor);

        //############# TILES OPERATIONS #################
        void connect(Tile::ptr tile);

        bool is_visible_from(const Tile& other, uint16_t range) const;
};

bool Tile::is_passable() const {
    return passable && !actor;
}

#endif // TILE_H
