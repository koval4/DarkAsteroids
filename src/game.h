#ifndef GAME_H
#define GAME_H

#include <vector>
#include "map.h"
#include "actor.h"
#include "player.h"
#include "ui/panel.h"
#include "ui/label.h"
#include "ui/button.h"
#include "ui/textbox.h"

class Game {
    private:
        bool running;
        Map game_map;
        Player::UI ui;
        std::vector<Actor::ptr> players;

    public:
        Game();

        void Init();
        void Execute();
        void Loop();
        void Clean_up();
};

#endif // GAME_H
