#ifndef SM_COMMON_H
#define SM_COMMON_H

#include <functional>
#include <vector>
#include <string>
#include <experimental/optional>

namespace sm {
    template <typename Context>
    class BaseState;

    using std::experimental::optional;

    using action_t = std::function<void(void)>;

    template <typename EventType, typename Context>
    using condition_t = std::function<bool(const Context&, const EventType&)>;

    template <typename EventType, typename Context>
    using applier_t = std::function<action_t(const Context&, const EventType&)>;

    template <typename Context>
    using states_t = std::vector<BaseState<Context>>;

    using state_key_t = std::string;
}

#endif
