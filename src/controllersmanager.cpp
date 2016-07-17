#include "controllersmanager.h"

ControllersManager::ControllersManager()
    : action_queue(std::make_shared<ActionQueue>()) {}

ControllersManager& ControllersManager::inst() {
    static ControllersManager inst;
    return inst;
}

void ControllersManager::pull_actions() const {
    for (const auto& controller : controllers)
        controller.second->pull_actions();
}

const std::shared_ptr<ActionQueue> ControllersManager::get_action_queue() const {
    return action_queue;
}
