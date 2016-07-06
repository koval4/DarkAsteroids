#ifndef CONTROLLERSMANAGER_H
#define CONTROLLERSMANAGER_H

#include <unordered_map>
#include <queue>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <common.h>
#include "controller.h"
#include "ui/event_handler.h"

class ControllersManager {
    public:

    private:
        EventHandler::ptr handler;
        std::unordered_map<std::type_index, Controller::ptr> controllers;
        const std::shared_ptr<ActionQueue> action_queue;

        ControllersManager();

    public:
        static ControllersManager& inst();

        template<typename T, typename... Args>
        void make_controller(Args&&... args);
        template<typename T>
        void remove_controller();

        void pull_actions() const;
        const std::shared_ptr<ActionQueue> get_action_queue() const;
};

template<typename T, typename... Args>
void ControllersManager::make_controller(Args&&... args) {
    // used for type constraint
    static_assert(std::is_base_of<Controller, T>::value, "T must be derived from Controller");
    Controller::ptr controller = std::make_shared<T>(handler, action_queue, std::forward<Args>(args)...);
    controllers.emplace(typeid (T), controller);
}
template <typename T>
void ControllersManager::remove_controller() {
    controllers.erase(typeid (T));
}

#endif // CONTROLLERSMANAGER_H
