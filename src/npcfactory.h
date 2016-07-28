#ifndef NPCFACTORY_H
#define NPCFACTORY_H

#include <vector>
#include <string>
#include <memory>
#include "actormanager.h"
#include "npc.h"

class NPCFactory {
    private:
        const std::shared_ptr<ActorManager> manager;

    public:
        explicit NPCFactory(const std::shared_ptr<ActorManager>& manager);

        std::vector<std::shared_ptr<NPC>> get_squad(std::string type, uint8_t size);
};

#endif
