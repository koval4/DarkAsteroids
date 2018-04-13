#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <queue>
#include <memory>
#include "controller.h"
#include "ui/eventhandler.h"
#include "ui/listener.h"
#include "map.h"
#include "player.h"

class PlayerController : public Controller {
    private:
        Player& player;
        bool* game_state;
        Listener::ptr keyboard_listener;
        Listener::ptr mouse_listener;

        void show_inventory() const;
        void pick_item() const;

        void update_ap_lbl();       /// updates ap label text

    public:
        using ptr = std::shared_ptr<PlayerController>;

        PlayerController(
            const std::shared_ptr<ActionQueue>& action_queue,
            Player& player,
            bool* game_state);
        ~PlayerController();

        virtual void setup_ui() override;
        virtual void setup_handlers() override;
        virtual void clear_ui() const override;
};

#endif // PLAYERCONTROLLER_H
