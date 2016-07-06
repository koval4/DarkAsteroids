#include "controllersmanager.h"

ControllersManager::ControllersManager()
    : handler(std::make_shared<EventHandler>())
    , action_queue(std::make_shared<ActionQueue>()) {}

ControllersManager& ControllersManager::inst() {
    static ControllersManager inst;
    return inst;
}

void ControllersManager::pull_actions() const {
    handler->run();
}

const std::shared_ptr<ActionQueue> ControllersManager::get_action_queue() const {
    return action_queue;
}
