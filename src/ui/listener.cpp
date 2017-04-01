#include "ui/listener.h"
#include <functional>
#include <SDL2/SDL.h>
#include "ui/gui.h"

/**
 * @brief Listener::Listener
 */
Listener::Listener(EventType type)
    : type(type)
    , enabled(true) {}

Listener::~Listener() {
}

EventType Listener::get_type() const {
    return type;
}

bool Listener::is_enabled() const {
    return enabled;
}

void Listener::set_enabled(bool enabled) {
    this->enabled = enabled;
}

/**
* @brief set_handler -- start handling events in separate thread
* @param handler -- function that handles event
*/
void Listener::set_handler(std::function<void(SDL_Event&)> handler) {
    this->handler = handler;
}

void Listener::notify(SDL_Event& event) const {
    if (!enabled)
        return;
    if (handler)
        handler(event);
    GUI::inst().update();
}
