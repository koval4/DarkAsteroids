#include <stdlib.h>
#include <time.h>
#include <memory>
#include <vector>
#include "boost/sml.hpp"
#include "states/gameflow.h"

namespace sml = boost::sml;

int main(int argc, char *argv[]) {
    srand(time(nullptr));

    GameFlow game_flow;
    game_flow.run();

    return 0;
}
