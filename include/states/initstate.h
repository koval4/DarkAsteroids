#ifndef INIT_STATE_H
#define INIT_STATE_H

#include <boost/signals2.hpp>
#include "boost/sml.hpp"

namespace sml = boost::sml;
namespace sig = boost::signals2;

constexpr auto init_idle_state = sml::state<class Idle>;
constexpr auto loading_ui_state = sml::state<class LoadingUI>;
constexpr auto loading_entities_state = sml::state<class LoadingEntities>;

struct UILoaded {};
constexpr auto ui_loaded = sml::event<UILoaded>;

struct EntitiesLoaded {};
constexpr auto entities_loaded = sml::event<EntitiesLoaded>;

struct InitStateNotifier {
    template <typename Callback>
    sig::scoped_connection on_ui_loaded(Callback&& callback) {
        return ui_loaded.connect(callback);
    }

    template <typename Callback>
    sig::scoped_connection on_entities_loaded(Callback&& callback) {
        return entities_loaded.connect(callback);
    }

    template <typename Callback>
    sig::scoped_connection on_finished(Callback&& callback) {
        return init_finished.connect(callback);
    }

    sig::signal<void()> ui_loaded;
    sig::signal<void()> entities_loaded;
    sig::signal<void()> init_finished;
};

struct UILoader {
    void load_ui();
};

struct EntityLoader {
    void load_entities();
};

struct InitState {
    auto operator()() const noexcept {
        auto notify_ui_loaded       = [] (InitStateNotifier& notifier) { notifier.ui_loaded(); };
        auto notify_entities_loaded = [] (InitStateNotifier& notifier) { notifier.entities_loaded(); };
        auto notify_init_finished   = [] (InitStateNotifier& notifier) { notifier.init_finished(); };

        auto on_idle_entered = [&] (InitStateNotifier& notifier, UILoader& loader) {
            notify_ui_loaded(notifier);
            loader.load_ui();
        };
        auto on_ui_loaded = [&] (InitStateNotifier& notifier, EntityLoader& loader) {
            notify_entities_loaded(notifier);
            loader.load_entities();
        };

        return sml::make_transition_table(
            * init_idle_state / on_idle_entered = loading_ui_state
            , loading_ui_state + ui_loaded / on_ui_loaded = loading_entities_state
            , loading_entities_state + entities_loaded / notify_init_finished = sml::X
        );
    }
};

#endif // INIT_STATE_H
