#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <queue>
#include <functional>
#include "ui/event_handler.h"

using ActionQueue = std::queue<std::function<void(void)>>;

class Controller {
    public:
        using ptr = std::shared_ptr<Controller>;

    protected:
        const EventHandler::ptr handler;
        const std::shared_ptr<ActionQueue> action_queue;

        template<typename T>
        void add_action(T action);
        virtual void setup_ui() = 0;
        virtual void clear_ui() const = 0;

    public:
        Controller(
                const EventHandler::ptr& handler,
                const std::shared_ptr<ActionQueue>& action_queue);
        virtual ~Controller();
};

template <typename T>
void Controller::add_action(T action) {
    action_queue->push(action);
}

#endif // CONTROLLER_H
