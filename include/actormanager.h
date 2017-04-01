#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H

#include <memory>
#include <unordered_set>
#include "common.h"
#include "actor.h"
#include "map.h"

class ActorManager {
    public:
        using ptr = std::shared_ptr<ActorManager>;

    private:
        std::unordered_set<Actor::ptr> actors;
        const Map::ptr map;
        std::unordered_set<Actor::ptr>::iterator curr_actor;    // actor which makes current turn

    public:
        explicit ActorManager(const Map::ptr& map);

        std::unordered_set<Actor::ptr> get_actors() const;
        const Actor::ptr get_curr_actor() const;

        void start_turn();

        void end_turn();
        void check_alive();
        void place_at(Coord pos, Actor::ptr actor);
        void kill(const Actor::ptr& actor);
};

#endif // ACTORMANAGER_H
