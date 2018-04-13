#ifndef GAME_H
#define GAME_H

#include <vector>
#include <queue>
#include <functional>
#include "map.h"
#include "mapdrawer.h"
#include "actor.h"
#include "actormanager.h"
#include "player.h"

class ActionPhase {
public:
    ActionPhase(Map&& map, ActorManager&& actor_manager);

    void init();
    void run();
    void clean_up();

private:
    bool running;
    Map map;
    MapDrawer drawer;
    ActorManager actor_manager;
    Coord focus_point;

    void draw();
    void handle_events();
    void update();

};

#endif // GAME_H
