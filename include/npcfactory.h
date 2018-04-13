#ifndef NPCFACTORY_H
#define NPCFACTORY_H

#include <vector>
#include <string>
#include <memory>
#include "actormanager.h"
#include "npc.h"

class NPCFactory {
    private:
        const ActorManager& manager;

    public:
        explicit NPCFactory(const ActorManager& manager);

        std::vector<std::shared_ptr<NPC>> get_squad(std::string type, uint8_t size);
};

#endif
