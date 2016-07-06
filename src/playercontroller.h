#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <queue>
#include <memory>
#include "controller.h"
#include "ui/event_handler.h"
#include "ui/listener.h"
#include "map.h"
#include "player.h"

class PlayerController : public Controller {
    private:
        const Player::ptr player;
        Listener::ptr keyboard_listener;
        Listener::ptr mouse_listener;

        void show_inventory() const;
        void pick_item() const;

        void update_ap_lbl();       /// updates ap label text
        virtual void setup_ui() override;
        virtual void clear_ui() const override;

    public:
        using ptr = std::shared_ptr<PlayerController>;

        PlayerController(const EventHandler::ptr& handler,
                         const std::shared_ptr<ActionQueue>& action_queue,
                         const Player::ptr& player);
        ~PlayerController();
};

#endif // PLAYERCONTROLLER_H
