#ifndef GAMEFLOW_STATES_H
#define GAMEFLOW_STATES_H

#include <cstdint>
#include <SDL2/SDL_events.h>
#include "boost/sml.hpp"
#include "states/events.h"
#include "states/initstate.h"

namespace sml = boost::sml;

constexpr auto game_idle_state = sml::state<class GameIdle>;
constexpr auto init_state = sml::state<InitState>;
constexpr auto main_menu_state = sml::state<class MainMenuState>;
constexpr auto action_phase_state = sml::state<class ActionPhaseState>;

struct GameFlowImpl {
    auto operator()() const noexcept {
        return sml::make_transition_table(
            * game_idle_state + start_init = init_state
            , init_state + init_finished = main_menu_state
            , main_menu_state + new_game_selected = action_phase_state
            , main_menu_state + quit_selected = sml::X
            , action_phase_state + quit_to_mm_selected = main_menu_state
            , action_phase_state + game_over = main_menu_state

            , init_state + app_quit = sml::X
            , main_menu_state + app_quit = sml::X
        );
    }
};

class GameFlow {
public:
    GameFlow();

    void run();

private:
    InitStateNotifier     init_notifier;
    UILoader              ui_loader;
    EntityLoader          entity_loader;
    sml::sm<GameFlowImpl> impl;
};

#endif // GAMEFLOW_STATES_H
