#ifndef LISTENER_H
#define LISTENER_H

#include <functional>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include "../data.h"

/**
 * @brief The Listener class describes objects that handles some events in separate thread
 */
class Listener {
    public:
        typedef std::shared_ptr<Listener> ptr;
    private:
        EventType type;                            /// type of listened events
        std::function<void(SDL_Event&)> handler;    /// function that handles event
        bool enabled;                               /// is listener enabled or not

    public:
       //###################### CONSTRUCTORS ##################
       /**
         * @brief Listener
         */
        Listener(EventType type);

        //################## DESTRUCTORS ############################
        ~Listener();

        //################ GETTERS ##############
        EventType get_type() const;
        bool is_enabled() const;

        //################ SETTERS ###############
        void set_enabled(bool enabled);

        /**
         * @brief run -- start handling events in separate thread
         * @param handler -- function that handles event
         */
        void run(std::function<void(SDL_Event&)> handler);
        void notify(SDL_Event& event);
};

#endif // LISTENER_H
