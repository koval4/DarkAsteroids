#ifndef CONTROLLERSMANAGER_H
#define CONTROLLERSMANAGER_H

#include <unordered_map>
#include <queue>
#include <string>
#include <typeindex>
#include <typeinfo>
#include "common.h"
#include "controller.h"
#include "ui/eventhandler.h"

class ControllersManager {
    public:

    private:
        std::unordered_map<std::type_index, Controller::ptr> controllers;
        const std::shared_ptr<ActionQueue> action_queue;

        ControllersManager();

    public:
        static ControllersManager& inst();

        template<typename T, typename... Args>
        std::shared_ptr<T> make_controller(Args&&... args);
        template<typename T>
        void remove_controller();

        void pull_actions() const;
        const std::shared_ptr<ActionQueue> get_action_queue() const;
};

template<typename T, typename... Args>
std::shared_ptr<T> ControllersManager::make_controller(Args&&... args) {
    // used for type constraint
    static_assert(std::is_base_of<Controller, T>::value, "T must be derived from Controller");
    auto controller = std::make_shared<T>(action_queue, std::forward<Args>(args)...);
    controller->setup_ui();
    controller->setup_handlers();
    controllers.emplace(typeid (T), controller);
    return controller;
}
template <typename T>
void ControllersManager::remove_controller() {
    controllers.at(typeid(T))->clear_ui();
    controllers.erase(typeid(T));
}

#endif // CONTROLLERSMANAGER_H
