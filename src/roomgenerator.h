#ifndef ROOMGENERATOR_H
#define ROOMGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "room.h"

class RoomGenerator {
    public:
        using prt = std::shared_ptr<RoomGenerator>;

    private:
        Map::ptr map;

    public:
        explicit RoomGenerator(Map::ptr map);

        void generate(Rectangle square);
        void put_items();
        void place_npc();
};

#endif // ROOMGENERATOR_H
