#ifndef ROOM_H
#define ROOM_H

#include "data.h"
#include "tile.h"

class Room {
    private:
        Rectangle area;
        std::vector<Tile::ptr> tiles;

    public:
        Room();
};

#endif // ROOM_H
