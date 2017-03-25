#ifndef STATE_H
#define STATE_H

#include <memory>
#include "common.h"
#include "event.h"

namespace sm {
    template <typename Context>
    class BaseState {
    public:
        BaseState() = default;

        template <typename Impl>
        BaseState(Impl impl)
            : impl(std::make_shared<StateModel<Impl>>(std::move(impl))) {}

        state_key_t get_name() const {
            return impl->get_name();
        }

        state_key_t get_next_state() const {
            return impl->get_next_state();
        }

        optional<action_t> run(const Context& npc, const Event& event) const {
            return impl->run(npc, event);
        }

    private:
        struct IState {
            virtual ~IState() = default;
            virtual state_key_t get_name() const = 0;
            virtual state_key_t get_next_state() const = 0;
            virtual optional<action_t> run(const Context& npc, const Event& event) const = 0;
        };

        template <typename Impl>
        struct StateModel final : IState {
            StateModel(Impl impl) : impl(std::move(impl)) {}

            state_key_t get_name() const override {
                return impl.get_name();
            }

            state_key_t get_next_state() const override {
                return impl.get_next_state();
            }

            optional<action_t> run(const Context& npc, const Event& event) const override {
                return impl.run(npc, event);
            }

            Impl impl;
        };

        std::shared_ptr<const IState> impl;
    };


    template <typename Context, typename EventType>
    class State {
    public:
        using event_t = EventType;
        using context_t = Context;

        State(
            const char* name,
            condition_t<event_t, context_t> condition,
            applier_t<event_t, context_t> applier,
            state_key_t next_state)
            : name(name)
            , condition(condition)
            , applier(applier)
            , next_state(next_state) {}

        state_key_t get_name() const {
            return name;
        }

        state_key_t get_next_state() const {
            return next_state;
        }

        optional<action_t> run(const Context& npc, const Event& event) const {
            const event_t* actual_event = dynamic_cast<const event_t*>(&event);
            if (actual_event && condition(npc, *actual_event)) {
                return applier(npc, *actual_event);
            } else {
                return {};
            }
        }

    private:
        state_key_t                     name;
        condition_t<event_t, context_t> condition;
        applier_t<event_t, context_t>   applier;
        state_key_t                     next_state;
    };

    template <typename Context, typename EventType>
    State<Context, EventType> make_state(
        const char* name,
        condition_t<EventType, Context> condition,
        applier_t<EventType, Context> applier,
        state_key_t next_state) {
        return State<Context, EventType>{ name, condition, applier, next_state };
    }
}

#endif
