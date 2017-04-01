#include "dungeongenerator.h"
#include "game.h"
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
#include "controllersmanager.h"
#include "playercontroller.h"
#include "npccontroller.h"
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

Game::Game() {
    running = true;
}

void Game::make_turn(const std::shared_ptr<Actor>& actor) {
    actor->start_turn();
    actor->make_turn();
    auto player = std::dynamic_pointer_cast<Player>(actor);
    if (player) {
        ControllersManager::inst().make_controller<PlayerController>(player, &running);
        while (actor->can_make_turn() && running) {
            handle_events();
            draw(actor);
            update();
        }
        ControllersManager::inst().remove_controller<PlayerController>();
    } else {
        auto controller = ControllersManager::inst().make_controller<NPCController>(
            std::dynamic_pointer_cast<NPC>(actor)
        );
        while (actor->can_make_turn() && running) {
            controller->do_action();
            draw(actor);
            update();
        }
        ControllersManager::inst().remove_controller<NPCController>();
    }
    actor->end_turn();
}

void Game::draw(const std::shared_ptr<Actor>& actor) {
    GUI::inst().update();
    if (std::dynamic_pointer_cast<Player>(actor)) {
        MapDrawer {} .draw_map(map->at(actor->get_pos()));
    }
}

void Game::handle_events() {
    ControllersManager::inst().pull_actions();
}

void Game::update() {
    // processing action added in queue in handle_events()
    auto action_queue = ControllersManager::inst().get_action_queue();
    while (!action_queue->empty()) {
        action_queue->front()();
        action_queue->pop();
    }
    actor_manager->check_alive();
}

void Game::Init() {
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

void Game::Execute() {
    Init();
    for (uint16_t i = 0; i < 4; i++)
        players.push_back(Player::generate());
    Player::set_game_state(&running);

    //--------------------------setting player's gui---------------------------------
    Padding standart_padding = {3, 3, 3, 3};

    SDL_Rect output_form = {0, 400, 250, 200};
    GUI::inst().make_widget<Textbox>("output", output_form, Padding(5, 5, 5, 5), true);

    auto ap_lbl_width = 50;
    auto ap_lbl_height = 30;
    auto ap_lbl_x = Screen::inst().get_width() - ap_lbl_width - 5;
    auto ap_lbl_y = Screen::inst().get_height() - ap_lbl_height - 5;
    SDL_Rect ap_lbl_form = {ap_lbl_x, ap_lbl_y, ap_lbl_width, ap_lbl_height};
    Label::set_default_font_size(20);
    GUI::inst().make_widget<Label>("ap_lbl", "0/0", ap_lbl_form);
//    ui.ap_lbl->set_font("mplu-1m-regular.ttf", 20);
    Label::set_default_font_size(12);

    SDL_Rect weap_panel_form = {250, Screen::inst().get_height() - 70, 150, 70};
    GUI::inst().make_widget<Panel>("weapon_panel", weap_panel_form, standart_padding);

    SDL_Rect weap_info_form = {weap_panel_form.x + 3, weap_panel_form.y + 3, 120, 20};
    GUI::inst().make_widget<Label>("weapon_info", "Empty", weap_info_form);

    auto ammo_info_y = weap_info_form.y + weap_info_form.h + 3;
    SDL_Rect ammo_info_form = {weap_panel_form.x + 3, ammo_info_y, 120, 20};
    GUI::inst().make_widget<Label>("ammo_info", "No ammo", ammo_info_form);

    auto next_weap_x = weap_info_form.x + weap_info_form.w + 3;
    SDL_Rect next_weap_form = {next_weap_x, weap_panel_form.y + 3, 20, 20};
    GUI::inst().make_widget<Button>("next_weap", img("up_arrow.png"), next_weap_form, standart_padding);

    auto prev_weap_x = ammo_info_form.x + ammo_info_form.w + 3;
    SDL_Rect prev_weap_form = {prev_weap_x, ammo_info_form.y + 3, 20, 20};
    GUI::inst().make_widget<Button>("prev_weap", img("down_arrow.png"), prev_weap_form, standart_padding);

    auto reload_y = ammo_info_form.y + ammo_info_form.h + 3;
    SDL_Rect reload_form = { weap_panel_form.x + 3, reload_y, 144, 20};
    GUI::inst().make_widget<Button>("reload_btn", "Reload", reload_form, standart_padding);
    //-------------------------------------------------------------------------------------------------

    map = std::make_shared<Map>(100, 100);
    actor_manager = std::make_shared<ActorManager>(map);
    DungeonGenerator dungeon_generator { map, actor_manager };
    dungeon_generator.generate();
    dungeon_generator.place_players(players);
    Map::curr_map = map.get();
    actor_manager->start_turn();
    while (running)
        Loop();
    Clean_up();
}

void Game::Loop() {
    for (const auto& actor : actor_manager->get_actors()) {
        make_turn(actor);
        if (!running)
            return;
    }
}

void Game::Clean_up() {
}
