#include "npc.h"
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include "map.h"
#include "common.h"

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

std::vector<std::shared_ptr<Tile>> NPC::get_tiles_in_fov() const {
    // TODO: REFACTOR THIS !!!!
    using tiles_t = std::vector<std::shared_ptr<Tile>>;
    tiles_t tiles;
    tiles.reserve(params.visibility_range * params.visibility_range);
    auto center = map->at(pos);

    // filter (is_visible_from center params.visibility_range) get_adjacent
    auto get_adjacent = [this, &center] (auto tile) -> tiles_t {
        auto adjacent = tile->get_adjacent();
        adjacent.erase(
            std::remove_if(begin(adjacent), end(adjacent),
            [this, &center] (auto tile) {
                return !tile->is_visible_from(*center, params.visibility_range);
            })
        );
        return adjacent;
    };

    // recursife fn, concatenates all visible adjacent tiles into vector
    std::function<tiles_t(const std::shared_ptr<Tile>&)> get_result;
    get_result = [&] (auto tile) -> tiles_t {
        auto adjacent = get_adjacent(tile);
        if (adjacent.empty())
            return {};
        tiles_t result;
        result.reserve(adjacent.size() * 4);
        // getting visible adjacent tiles for each adjacent tile
        for (const auto& it : adjacent) {
            auto it_adjacent = get_result(it);
            if (!it_adjacent.empty())
                result.insert(end(result), begin(it_adjacent), end(it_adjacent));
        }
        result.erase(std::unique(begin(result), end(result)));
        return result;
    };

    return get_result(center);
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

std::shared_ptr<Attack> NPC::choose_attack() {
    std::shared_ptr<Attack> b;

    return b;
}

void NPC::make_turn() {
    auto actors = get_actors_in_fov();
    // filter to enemies only
    actors.erase(
        std::remove_if(begin(actors), end(actors),
        [this, &actors] (auto actor) -> bool {
            return !this->is_enemy(*actor);
        })
    );
    if (actors.empty()) {
        making_turn = false;
        return;
    }
    // get to attack range to closer enemy
    auto distance_to_this = std::bind(get_distance, pos, std::placeholders::_1);
    auto enemy = *std::min_element(begin(actors), end(actors),
        [this, &distance_to_this] (auto lhs, auto rhs) -> bool {
            return distance_to_this(lhs->get_pos()) < distance_to_this(rhs->get_pos());
        }
    );
    // choose attack
    auto att = choose_attack();
    // move to attack range
    if (att->get_range() < distance_to_this(enemy->get_pos())) {
        auto path = find_path_to(enemy->get_pos());
        path.erase(end(path) - att->get_range());
    }
}

void NPC::pick_item() {

}

void NPC::attack(Coord target) {

}

std::function<void(void)> NPC::get_action() {
    auto action = actions.front();
    actions.pop();
    return action;
}
