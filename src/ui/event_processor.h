#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <SDL2/SDL.h>
#include "listener.h"
#include "widget.h"

/**
 * @brief The EventProcessor class is singleton that polls events
 */
class EventProcessor {
    private:
        std::unordered_map<EventType, std::vector<const Listener*>> listeners;

    public:
        //############# CONSTRUCTORS #################
        /**
         * @brief EventProcessor -- default constructor, starts thread that polls event
         */
        EventProcessor();

        //############### DESTRUCTOR ##################
        ~EventProcessor();

        //################ GETTERS ################

        //############## LISTENERS REGISTRY ##############
        void listen(const Listener* listener);
        void listen(const Widget& widget);
        void unlisten(const Listener* listener);
        void unlisten(const Widget& widget);

        void run(std::function<bool(void)> condition);

};

#endif // EVENT_PROCESSOR_H
