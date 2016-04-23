#include "event_processor.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <functional>
#include <SDL2/SDL.h>
#include "listener.h"

/**
* @brief EventProcessor -- default constructor, starts thread that polls event
*/
EventProcessor::EventProcessor() {}

EventProcessor::~EventProcessor() {}

//################## GETTERS #####################

/**
 * @brief EventProcessor::inst -- static function to get instance of singleton class
 * @return reference to singleton instance of event processor
 */
EventProcessor& EventProcessor::inst() {
    static EventProcessor instance;
    return instance;
}

void EventProcessor::listen(Listener* listener) {
    listeners[listener->get_type()].push_back(listener);
}

void EventProcessor::unlisten(Listener* listener) {
    auto& listnr_at = listeners[listener->get_type()];
    auto it = listnr_at.begin();
    while (it != listnr_at.end()) {
        if (*it == listener) {
            listnr_at.erase(it);
            return;
        }
        it++;
    }
}

void EventProcessor::block_listeners() {
    for (auto& type_it : listeners) {
        for (auto& it : type_it.second) {
            it->set_enabled(false);
        }
    }
}

void EventProcessor::unblock_listeners() {
    for (auto& type_it : listeners) {
        for (auto& it : type_it.second) {
            it->set_enabled(true);
        }
    }
}

void EventProcessor::run(std::function<bool ()> condition) {
    if (listeners.empty())
        return;
    SDL_Event event;
    while (condition()) {
        SDL_PollEvent(&event);
#ifdef _DEBUG_
        if (event.type == SDL_KEYDOWN) {
            std::cout << "Key " << event.key.keysym.sym << " pressed!" << std::endl;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            std::cout << "Mouse button pressed. Position: x = " << event.button.x << ", y = " << event.button.y << std::endl;
        }
#endif
        for (auto& listener : listeners[event.type]) {
            if (!condition()) return;
            listener->notify(event);
        }
    }
}
