#include "actormanager.h"
#include <unordered_set>
#include "common.h"
#include "actor.h"
#include "map.h"

ActorManager::ActorManager(const Map& map)
    : map(map) {
    actor_iterator = actors_queue.begin();
}

const ActorManager::actors_t& ActorManager::get_actors() const {
    return actors;
}

Actor& ActorManager::get_curr_actor() {
    Actor& actor = ***actor_iterator;
    if (!actor.can_make_turn()) {
        ++actor_iterator;
        if (actor_iterator == actors_queue.end()) {
            actor_iterator = actors_queue.begin();
        }
    }
    return actor;
}

void ActorManager::make_turn_priority() {
    for (auto it = actors.begin(); it != actors.end(); ++it) {
        actors_queue.push_back(it);
    }
    actor_iterator = actors_queue.begin();
}

void ActorManager::check_alive() {
    while (!actors_queue.empty() && !(**actor_iterator)->is_alive()) {
        (**actor_iterator)->die();
        actors.erase(*actor_iterator);
        actor_iterator = actors_queue.erase(actor_iterator);
        if (actor_iterator == actors_queue.end()) {
            actor_iterator = actors_queue.begin();
        }
    }

    for (auto actor = actors_queue.begin(); actor != actors_queue.end(); ++actor) {
        if (!(**actor)->is_alive()) {
            (**actor)->die();
            actors.erase(*actor);
            actor = actors_queue.erase(actor);
        }
    }
}

void ActorManager::place_at(Coord pos, std::unique_ptr<Actor>&& actor) {
    map.at(pos)->place_actor(actor.get());
    actor->set_map(&map);
    actors.push_back(std::move(actor));
}

