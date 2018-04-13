#ifndef ROOMGENERATOR_H
#define ROOMGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "tile.h"
#include "map.h"
#include "actormanager.h"
#include "room.h"

class RoomGenerator {
    public:
        using prt = std::shared_ptr<RoomGenerator>;

    private:
        Map& map;
        const ActorManager& actor_manager;

        void put_items();

    public:
        RoomGenerator(Map& map, const ActorManager& actor_manager);

        void generate(Rectangle square);
};

#endif // ROOMGENERATOR_H
