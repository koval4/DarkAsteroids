#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <array>
#include <tuple>
#include <unordered_map>
#include "common.h"
#include "event.h"
#include "state.h"

namespace sm {
    template <typename Context>
    class StateMachine {
    public:
        using state_iterator = typename states_t<Context>::const_iterator;

        template <typename Iterator>
        StateMachine(Iterator begin, Iterator end)
            : states(begin, end) {
            current_state = states.begin();
            for (state_iterator it = states.begin(); it != states.end(); ++it) {
                lookup.emplace(it->get_name(), it);
            }
        }

        StateMachine(const StateMachine&) = default;
        StateMachine(StateMachine&&) = default;

        ~StateMachine() = default;

        optional<action_t> run(const Context& npc, const Event& event) {
            optional<action_t> action;
            while (!action && current_state != states.end()) {
                action = current_state->run(npc, event);
                if (action) {
                    current_state = lookup.find(current_state->get_next_state())->second;
                    break;
                } else {
                    current_state++;
                }
            }
            return action;
        }

    private:
        using lookup_t = std::unordered_map<state_key_t, typename states_t<Context>::const_iterator>;

        states_t<Context> states;
        lookup_t          lookup;
        state_iterator    current_state;
    };

    template <typename Context, typename... EventType, size_t... index>
    std::array<BaseState<Context>, sizeof...(index)> make_state_machine(
        const std::tuple<State<Context, EventType>...>& states,
        std::index_sequence<index...>) {
        return { State<Context, EventType> { std::get<index>(states) }... };
    }

    template <typename Context, typename... EventType>
    StateMachine<Context> make_state_machine(const State<Context, EventType>&... states) {
        constexpr size_t states_number = sizeof...(states);
        std::array<BaseState<Context>, states_number> states_list =
            make_state_machine<Context>(
                std::make_tuple(states...),
                std::make_index_sequence<states_number>());
        return StateMachine<Context>(std::begin(states_list), std::end(states_list));
    }
}

#endif
