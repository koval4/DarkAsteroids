#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H

#include <memory>
#include <vector>
#include "common.h"
#include "actor.h"
#include "map.h"

class ActorManager {
    using actors_t = std::vector<std::unique_ptr<Actor>>;

public:
    explicit ActorManager(const Map& map);
    ActorManager(const ActorManager&) = delete;
    ActorManager(ActorManager&&) = default;

    const actors_t& get_actors() const;
    Actor& get_curr_actor();

    void make_turn_priority();

    void check_alive();
    void place_at(Coord pos, std::unique_ptr<Actor>&& actor);

private:
    actors_t actors;
    std::vector<actors_t::iterator>            actors_queue;
    const Map&                                 map;
    decltype(actors_queue)::iterator          actor_iterator;    // actor which makes current turn

};

#endif // ACTORMANAGER_H
