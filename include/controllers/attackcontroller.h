#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H

#include <vector>
#include <memory>
#include "controller.h"
#include "actor.h"
#include "player.h"
#include "attack.h"
#include "ui/panel.h"
#include "ui/listbox.h"
#include "ui/button.h"

class AttackController : public Controller {
    private:
        const std::shared_ptr<Player> player;
        const std::shared_ptr<Actor> target;

        std::vector<std::shared_ptr<Attack>> available_attacks;
        std::shared_ptr<Attack> selected_attack;

        std::shared_ptr<Panel> panel;
        std::shared_ptr<Listbox> attacks_list;
        std::shared_ptr<Button> attack_btn;
        std::shared_ptr<Button> aim_btn;

    public:
        AttackController(
            const std::shared_ptr<ActionQueue>& action_queue,
            const std::shared_ptr<Player>& player,
            const std::shared_ptr<Actor>& target);
        ~AttackController();

        virtual void setup_ui() override;
        virtual void setup_handlers() override;
        virtual void clear_ui() const override;
};

#endif // ATTACKCONTROLLER_H
