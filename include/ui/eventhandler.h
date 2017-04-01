#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <SDL2/SDL.h>
#include "listener.h"
#include "widget.h"

/**
 * @brief The EventProcessor class is used for events polling
 */
class EventHandler {
    public:
        using ptr = std::shared_ptr<EventHandler>;
    private:
        std::unordered_map<EventType, std::vector<const Listener*>> listeners;

    public:
        //############# CONSTRUCTORS #################
        /**
         * @brief EventProcessor -- default constructor, starts thread that polls event
         */
        EventHandler();

        //############### DESTRUCTOR ##################
        ~EventHandler();

        //################ GETTERS ################

        //############## LISTENERS REGISTRY ##############
        void listen(const Listener* listener);
        void listen(const Widget& widget);
        void unlisten(const Listener* listener);
        void unlisten(const Widget& widget);

        void run(std::function<bool(void)> condition);
        void run() const;
};

#endif // EVENT_PROCESSOR_H
