#include "npcfactory.h"
#include "npcprototype.h"

NPCFactory::NPCFactory(const std::shared_ptr<ActorManager>& manager)
    : manager(manager) {}

std::vector<std::shared_ptr<NPC>> NPCFactory::get_squad(std::string type, uint8_t size) {
    std::vector<std::shared_ptr<NPC>> squad;
    squad.reserve(size);

    for (uint8_t i = 0; i < size; i++) {
        auto prototype = NPCPrototype::PROTOTYPES.at(type);
        std::string name = prototype.get_name();
        std::string description = prototype.get_description();
        std::string texture = prototype.get_texture();
        Race race = prototype.get_race();
        Actor::Params params = Actor::Params::rand_params();
        params.calc_secondary_params();
        Actor::Skills skills;

        squad.push_back(
            std::make_shared<NPC>(
                texture,
                name,
                description,
                params,
                skills,
                race,
                1,
                1
            )
        );
    }

    return squad;
}
