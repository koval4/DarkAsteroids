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
    Screen::init(); // call to create an instance of screen and to initialize SDL
    Textbox::set_default_back_path(img("bordered_back.png"));
    Textbox::set_default_font_path("mplus-1m-regular.ttf");
    Textbox::set_default_font_size(12);
    Textbox::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Button::set_default_background(img("bordered_back.png"));
    Button::set_default_font("mplus-1m-regular.ttf");
    Button::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Button::set_default_font_size(14);
    Listbox::set_default_background(img("bordered_back.png"));
    Listbox::set_default_font("mplus-1m-regular.ttf");
    Listbox::set_default_font_size(12);
    Listbox::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Listbox::set_default_item_back(img("bordered_back.png"));
    Listbox::set_default_item_height(25);
    Listbox::set_default_item_margin({2, 6, 2, 5});
    Listbox::set_default_spacing(2);
    Slidebar::set_default_back_txtr(img("slidebar.png"));
    Slidebar::set_default_slider_txtr(img("slider.png"));
    Slidebar::set_default_up_txtr(img("up_arrow.png"));
    Slidebar::set_default_down_txtr(img("down_arrow.png"));
    Panel::set_default_background(img("bordered_back.png"));
    Label::set_default_font("mplus-1m-regular.ttf");
    Label::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Label::set_default_font_size(12);

    Item::read_materials_txt();
    Item::read_item_txt();
    Bodypart::read_bodyparts_txt();
    Weapon::read_weapons_txt();
    Armor::read_armor_txt();
    Container::read_containers_txt();
    Race::read_races_txt();
    NPCPrototype::read_from_file("npc.txt");
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

