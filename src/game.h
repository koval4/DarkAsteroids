#ifndef GAME_H
#define GAME_H

#include <vector>
#include <list>
#include "map.h"
#include "actor.h"
#include "actormanager.h"
#include "player.h"
#include "ui/panel.h"
#include "ui/label.h"
#include "ui/button.h"
#include "ui/textbox.h"

class Game {
    private:
        bool running;
        Map::ptr map;
        ActorManager::ptr actor_manager;
        Player::UI ui;
        std::list<Player::ptr> players;

    public:
        Game();

        void Init();
        void Execute();
        void Loop();
        void Clean_up();
};

#endif // GAME_H
