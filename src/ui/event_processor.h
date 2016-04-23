#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include <unordered_map>
#include <vector>
#include <stack>
#include <functional>
#include <SDL2/SDL.h>
#include "listener.h"

/**
 * @brief The EventProcessor class is singleton that polls events
 */
class EventProcessor {
    private:
        std::unordered_map<EventType, std::vector<Listener*>> listeners;

        /**
         * @brief EventProcessor -- default constructor, starts thread that polls event
         */
        EventProcessor();
    public:
        //############### DESTRUCTOR ##################
        ~EventProcessor();

        //################ GETTERS ################
        static EventProcessor& inst(); /// returns singleton instance of event processor

        //############## LISTENERS REGISTRY ##############
        void listen(Listener* listener);
        void unlisten(Listener* listener);

        //############# BLOCKING ##################
        void block_listeners();
        void unblock_listeners();

        void run(std::function<bool(void)> condition);

};

#endif // EVENT_PROCESSOR_H
