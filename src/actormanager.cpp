#include "actormanager.h"
#include <unordered_set>
#include "data.h"
#include "actor.h"
#include "map.h"

ActorManager::ActorManager(const Map::ptr& map)
    : map(map) {}

std::unordered_set<Actor::ptr> ActorManager::get_actors() const {
    return actors;
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


