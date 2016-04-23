#include "listener.h"
#include <functional>
#include <SDL2/SDL.h>
#include "event_processor.h"
#include "gui.h"

/**
 * @brief Listener::Listener
 */
Listener::Listener(EventType type)
    : type(type)
    , enabled(true) {}

Listener::~Listener() {
    EventProcessor::inst().unlisten(this);
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
* @brief run -- start handling events in separate thread
* @param handler -- function that handles event
*/
void Listener::run(std::function<void(SDL_Event&)> handler) {
    this->handler = handler;
    EventProcessor::inst().listen(this);
}

void Listener::notify(SDL_Event& event) {
    if (!enabled)
        return;
    if (handler)
        handler(event);
    GUI::inst().update();
}
