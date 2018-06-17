#include "states/gameflow.h"
#include <chrono>

void GameFlow::run() {
    impl.process_event(StartInit{});

    auto prev_frame_time = std::chrono::system_clock::now();
    do {
        auto frame_time = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - prev_frame_time);
        impl.process_event(TimeTick{diff.count()});
        prev_frame_time = frame_time;
    } while (!impl.is(sml::X));
}
