#include "dungeongenerator.h"
#include "actionphase.h"
#include "mapdrawer.h"
#include <vector>
#include <list>
#include "ui/eventhandler.h"
#include "ui/screen.h"
#include "ui/gui.h"
#include "ui/textbox.h"
#include "ui/button.h"
#include "ui/listbox.h"
#include "ui/listener.h"
#include "ui/slidebar.h"
#include "ui/panel.h"
#include "controllers/controllersmanager.h"
#include "controllers/playercontroller.h"
#include "controllers/npccontroller.h"
#include "race.h"
#include "actor.h"
#include "actormanager.h"
#include "player.h"
#include "npcprototype.h"
#include "npc.h"
#include "item.h"
#include "bodypart.h"
#include "weapon.h"
#include "container.h"
#include "armor.h"
#include "common.h"

ActionPhase::ActionPhase(Map&& map, ActorManager&& actor_manager)
    : running(true)
    , map(std::move(map))
    , actor_manager(std::move(actor_manager)) {}

void ActionPhase::draw() {
    GUI::inst().update();
    drawer.draw_map(map.at(focus_point));
}

void ActionPhase::handle_events() {
    ControllersManager::inst().pull_actions();
}

void ActionPhase::update() {
    actor_manager.get_curr_actor().make_turn();
    actor_manager.check_alive();
}

void ActionPhase::init() {
}

void ActionPhase::run() {
    running = true;
    while (running) {
        draw();
        update();
        handle_events();
    }
    clean_up();
}

void ActionPhase::clean_up() {}

