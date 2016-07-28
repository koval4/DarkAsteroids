#ifndef ROOMGENERATOR_H
#define ROOMGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "actormanager.h"
#include "room.h"

class RoomGenerator {
    public:
        using prt = std::shared_ptr<RoomGenerator>;

    private:
        const Map::ptr map;
        const ActorManager::ptr actor_manager;

        void put_items();

    public:
        RoomGenerator(const Map::ptr& map, const ActorManager::ptr& actor_manager);

        void generate(Rectangle square);
};

#endif // ROOMGENERATOR_H
