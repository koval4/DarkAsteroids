#include "event_handler.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <SDL2/SDL.h>
#include "listener.h"

/**
* @brief EventProcessor -- default constructor, starts thread that polls event
*/
EventHandler::EventHandler() {}

EventHandler::~EventHandler() {}

//################## GETTERS #####################

void EventHandler::listen(const Listener* listener) {
    listeners[listener->get_type()].push_back(listener);
}

void EventHandler::listen(const Widget& widget) {
    for (auto& listener : widget.get_listeners())
        listen(listener);
}

void EventHandler::unlisten(const Listener* listener) {
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

void EventHandler::unlisten(const Widget& widget) {
    for (auto& listener : widget.get_listeners())
        unlisten(listener);
}

void EventHandler::run(std::function<bool ()> condition) {
    if (listeners.empty())
        return;
    SDL_Event event;
    while (condition()) {
        while (SDL_PollEvent(&event)) {
#ifdef _DEBUG_
            if (event.type == SDL_KEYDOWN)
                std::cout << "Key " << event.key.keysym.sym << " pressed!" << std::endl;
            else if (event.type == SDL_MOUSEBUTTONDOWN)
                std::cout << "Mouse button pressed. Position: x = " << event.button.x << ", y = " << event.button.y << std::endl;
#endif
            for (auto& listener : listeners[event.type]) {
                if (!condition()) return;
                if (listener->is_enabled()) listener->notify(event);
            }
        }
    }
}

void EventHandler::run() const {
    if (listeners.empty())
        return;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
#ifdef _DEBUG_
        if (event.type == SDL_KEYDOWN)
            std::cout << "Key " << event.key.keysym.sym << " pressed!" << std::endl;
        else if (event.type == SDL_MOUSEBUTTONDOWN)
            std::cout << "Mouse button pressed. Position: x = " << event.button.x << ", y = " << event.button.y << std::endl;
#endif
        if (listeners.find(event.type) == listeners.end())
            continue;
        for (auto& listener : listeners.at(event.type)) {
            if (listener->is_enabled()) listener->notify(event);
        }
    }
}
