#include "controller.h"
#include <memory>

Controller::Controller(const EventHandler::ptr& handler,
                       const std::shared_ptr<ActionQueue>& action_queue)
    : handler(handler)
    , action_queue(action_queue) {}

Controller::~Controller() {

}
