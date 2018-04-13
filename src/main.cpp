#include <stdlib.h>
#include <time.h>
#include <memory>
#include <vector>
#include "player.h"
#include "actionphasebuilder.h"

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    std::vector<std::unique_ptr<Player>> players;
    for (size_t i = 0; i < 4; i++) {
        players.push_back(Player::generate());
    }
    ActionPhaseBuilder{std::move(players), 100, 100}.build().run();
    return 0;
}
