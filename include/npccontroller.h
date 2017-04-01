#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include <memory>
#include "controller.h"
#include "npc.h"

class NPCController : public Controller {
    private:
        std::shared_ptr<NPC> npc;

    public:
        explicit NPCController(
            const std::shared_ptr<ActionQueue>& action_queue,
            const std::shared_ptr<NPC>& npc
        );

        virtual void setup_ui() override;
        virtual void setup_handlers() override;
        virtual void clear_ui() const override;

        void do_action();
};

#endif
