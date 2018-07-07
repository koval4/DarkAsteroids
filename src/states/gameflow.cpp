#include "states/gameflow.h"

#include <chrono>
#include <SDL2/SDL_events.h>
#include "boost/sml/utility/dispatch_table.hpp"
#include "states/initstate.h"

GameFlow::GameFlow()
    : init_notifier{}
    , ui_loader{}
    , entity_loader{}
    , impl{init_notifier, ui_loader, entity_loader} {}

void GameFlow::run() {
    init_notifier.on_ui_loaded([this] () { impl.process_event(UILoaded{}); });
    init_notifier.on_entities_loaded([this] () { impl.process_event(EntitiesLoaded{}); });
    init_notifier.on_finished([this] () { impl.process_event(InitFinished{}); });

    auto dispatch_event = sml::utility::make_dispatch_table<SDL_Event, SDL_FIRSTEVENT, SDL_LASTEVENT>(impl);

    impl.process_event(StartInit{});

    auto prev_frame_time = std::chrono::system_clock::now();
    do {
        auto frame_time = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - prev_frame_time);
        impl.process_event(TimeTick{diff.count()});
        prev_frame_time = frame_time;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            dispatch_event(event, event.type);
        }

    } while (!impl.is(sml::X));
}
