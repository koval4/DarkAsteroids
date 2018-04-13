#include "actionphasebuilder.h"
#include <memory>
#include "actionphase.h"
#include "dungeongenerator.h"

ActionPhaseBuilder::ActionPhaseBuilder(
    std::vector<std::unique_ptr<Player>>&& players,
    size_t map_width,
    size_t map_height)
    : players(std::move(players))
    , map_width(map_width)
    , map_height(map_height) {}

ActionPhase ActionPhaseBuilder::build() {
    Map map { static_cast<uint8_t>(map_width), static_cast<uint8_t>(map_height) };
    ActorManager actor_manager { map };
    DungeonGenerator dungeon_generator { map, actor_manager };
    dungeon_generator.generate();
    dungeon_generator.place_players(std::move(players));
    actor_manager.make_turn_priority();

    return ActionPhase{std::move(map), std::move(actor_manager)};
}
