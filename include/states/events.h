#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <SDL2/SDL_events.h>
#include "boost/sml.hpp"

namespace sml = boost::sml;

struct StartInit {
    static constexpr auto id = SDL_USEREVENT + 1;
};
constexpr auto start_init = sml::event<StartInit>;

struct InitFinished {
    static constexpr auto id = SDL_USEREVENT + 2;
};
constexpr auto init_finished = sml::event<InitFinished>;

struct QuitSelected {
    static constexpr auto id = SDL_USEREVENT + 3;
};
constexpr auto quit_selected = sml::event<QuitSelected>;

struct NewGameSelected {
    static constexpr auto id = SDL_USEREVENT + 4;
};
constexpr auto new_game_selected = sml::event<NewGameSelected>;

struct QuitToMMSelected {
    static constexpr auto id = SDL_USEREVENT + 5;
};
constexpr auto quit_to_mm_selected = sml::event<QuitToMMSelected>;

struct GameOver {
    static constexpr auto id = SDL_USEREVENT + 6;
};
constexpr auto game_over = sml::event<GameOver>;

struct TimeTick {
    static constexpr auto id = SDL_USEREVENT + 7;
    int64_t delta;
};
constexpr auto time_tick = sml::event<TimeTick>;

///////////////////////////////////////////////

struct UILoaded {
    static constexpr auto id = SDL_USEREVENT + 8;
};
constexpr auto ui_loaded = sml::event<UILoaded>;

struct EntitiesLoaded {
    static constexpr auto id = SDL_USEREVENT + 9;
};
constexpr auto entities_loaded = sml::event<EntitiesLoaded>;

/////////////////////////////////////////////////

struct AppQuit {
    static constexpr auto id = SDL_QUIT;
};
constexpr auto app_quit = sml::event<AppQuit>;

#endif
