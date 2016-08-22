#include "npccontroller.h"

NPCController::NPCController(
    const std::shared_ptr<ActionQueue>& action_queue,
    const std::shared_ptr<NPC>& npc)
    : Controller(action_queue)
    , npc(npc) {}

void NPCController::setup_ui() {}

void NPCController::setup_handlers() {}

void NPCController::clear_ui() const {}

void NPCController::do_action() {
    add_action(npc->get_action());
}
