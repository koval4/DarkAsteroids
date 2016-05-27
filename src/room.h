#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>
#include "data.h"
#include "tile.h"

class Room {
    public:
        using ptr = std::shared_ptr<Room>;

    private:
        Rectangle area;
        std::vector<Tile::ptr> tiles;
        std::string floor_texture;
        std::string wall_texture;

    public:
        Room();

        void make();
};

#endif // ROOM_H
