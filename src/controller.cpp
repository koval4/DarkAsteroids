#include "controller.h"
#include <memory>

Controller::Controller(const std::shared_ptr<ActionQueue>& action_queue)
    : action_queue(action_queue) {}

Controller::~Controller() {}

void Controller::pull_actions() const {
    handler.run();
}
