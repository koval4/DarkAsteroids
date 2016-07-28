#include "npc.h"
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "data.h"

NPC::NPC(
    std::string texture,
    std::string name,
    std::string description,
    Params params,
    Skills skills,
    Race race,
    uint16_t level,
    uint16_t experience)
    : Actor(texture, name, description, params, skills, race, level, experience) {}

NPC::NPC(const NPC& obj) : Actor(obj) {}

std::vector<Tile::ptr> NPC::get_tiles_in_fov() const {
    std::vector<Tile::ptr> tiles;

    return  tiles;
}

std::vector<Actor::ptr> NPC::get_actors_in_fov() const {
    std::vector<Actor::ptr> actors;

    for (const auto& tile : get_tiles_in_fov()) {
        auto actor = tile->get_actor();
        if (actor)
            actors.push_back(actor);
    }

    return  actors;
}

void NPC::make_turn() {

}

void NPC::pick_item() {

}

void NPC::attack(Coord target) {

}
