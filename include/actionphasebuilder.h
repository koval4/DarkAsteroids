#ifndef ACTIONPHASEBUILDER_H
#define ACTIONPHASEBUILDER_H

#include <memory>
#include <vector>
#include "actionphase.h"
#include "player.h"

class ActionPhaseBuilder {
public:
    ActionPhaseBuilder(
        std::vector<std::unique_ptr<Player>>&& players,
        size_t map_width,
        size_t map_height);

    ActionPhase build();

private:
    std::vector<std::unique_ptr<Player>> players;
    size_t                               map_width;
    size_t                               map_height;
};

#endif // ACTIONPHASEBUILDER_H
