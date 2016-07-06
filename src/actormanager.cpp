#include "actormanager.h"
#include <unordered_set>
#include "data.h"
#include "actor.h"
#include "map.h"

ActorManager::ActorManager(const Map::ptr& map)
    : map(map) {
    curr_actor = actors.begin();
}

std::unordered_set<Actor::ptr> ActorManager::get_actors() const {
    return actors;
}

const Actor::ptr ActorManager::get_curr_actor() const {
    return *curr_actor;
}

void ActorManager::start_turn() {
    curr_actor = actors.begin();
}

void ActorManager::end_turn() {
    curr_actor++;
    if (curr_actor == actors.end())
        start_turn();
}

void ActorManager::check_alive() {
    auto actor = actors.begin();
    while (actor != actors.end()) {
        if (!(*actor)->is_alive()) {
            (*actor)->die();
            actors.erase(actor);
        } else actor++;
    }
}

void ActorManager::place_at(Coord pos, Actor::ptr actor) {
    map->at(pos)->place_actor(actor);
    actor->set_map(map.get());
    actors.insert(actor);
}

void ActorManager::kill(const Actor::ptr& actor) {
    auto it = actors.find(actor);
    if (it != actors.end()) {
        actor->die();
        actors.erase(it);
    }
}


