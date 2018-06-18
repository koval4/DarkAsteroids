#include "states/gameflow.h"

#include <chrono>
#include "states/initstate.h"

GameFlow::GameFlow()
    : init_notifier{}
    , ui_loader{}
    , entity_loader{}
    , impl{init_notifier, ui_loader, entity_loader} {}

void GameFlow::run() {
    init_notifier.on_finished([this] () { impl.process_event(InitFinished{}); });

    impl.process_event(StartInit{});

    auto prev_frame_time = std::chrono::system_clock::now();
    do {
        auto frame_time = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - prev_frame_time);
        impl.process_event(TimeTick{diff.count()});
        prev_frame_time = frame_time;
    } while (!impl.is(sml::X));
}
