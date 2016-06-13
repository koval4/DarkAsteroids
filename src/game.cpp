#include "dungeongenerator.h"
#include "game.h"
#include <vector>
#include <list>
#include "ui/event_processor.h"
#include "ui/screen.h"
#include "ui/gui.h"
#include "ui/textbox.h"
#include "ui/button.h"
#include "ui/listbox.h"
#include "ui/listener.h"
#include "ui/slidebar.h"
#include "ui/panel.h"
#include "actor.h"
#include "player.h"
#include "npc.h"
#include "item.h"
#include "bodypart.h"
#include "weapon.h"
#include "container.h"
#include "armor.h"
#include "data.h"

Game::Game() {
    running = true;
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
    Actor::read_races_txt();
    NPC::read_npc_txt();
}

void Game::Execute() {
    Init();
    for (uint16_t i = 0; i < 4; i++) {
        players.push_back(std::make_shared<Player>());
        players.back()->generate();
    }
    Player::set_game_state(&running);

    //--------------------------setting player's gui---------------------------------
    Padding standart_margin = {3, 3, 3, 3};

    SDL_Rect output_form = {0, 400, 250, 200};
    ui.output = Textbox::ptr(new Textbox(output_form, {5, 5, 5, 5}, true));

    auto ap_lbl_width = 50;
    auto ap_lbl_height = 30;
    auto ap_lbl_x = Screen::inst().get_width() - ap_lbl_width - 5;
    auto ap_lbl_y = Screen::inst().get_height() - ap_lbl_height - 5;
    SDL_Rect ap_lbl_form = {ap_lbl_x, ap_lbl_y, ap_lbl_width, ap_lbl_height};
    Label::set_default_font_size(20);
    ui.ap_lbl = Label::ptr(new Label("0/0", ap_lbl_form));
//    ui.ap_lbl->set_font("mplus-1m-regular.ttf", 20);
    Label::set_default_font_size(12);

    SDL_Rect weap_panel_form = {250, Screen::inst().get_height() - 70, 150, 70};
    ui.weapon_panel = Panel::ptr(new Panel(weap_panel_form, standart_margin));

    SDL_Rect weap_info_form = {weap_panel_form.x + 3, weap_panel_form.y + 3, 120, 20};
    ui.weapon_info = Label::ptr(new Label("Empty", weap_info_form));

    auto ammo_info_y = weap_info_form.y + weap_info_form.h + 3;
    SDL_Rect ammo_info_form = {weap_panel_form.x + 3, ammo_info_y, 120, 20};
    ui.ammo_info = Label::ptr(new Label("No ammo", ammo_info_form));

    auto next_weap_x = weap_info_form.x + weap_info_form.w + 3;
    SDL_Rect next_weap_form = {next_weap_x, weap_panel_form.y + 3, 20, 20};
    ui.next_weap = Button::ptr(new Button(img("up_arrow.png"), next_weap_form, standart_margin));

    auto prev_weap_x = ammo_info_form.x + ammo_info_form.w + 3;
    SDL_Rect prev_weap_form = {prev_weap_x, ammo_info_form.y + 3, 20, 20};
    ui.prev_weap = Button::ptr(new Button(img("down_arrow.png"), prev_weap_form, standart_margin));

    auto reload_y = ammo_info_form.y + ammo_info_form.h + 3;
    SDL_Rect reload_form = { weap_panel_form.x + 3, reload_y, 144, 20};
    ui.reload_btn = Button::ptr(new Button("Reload", reload_form, standart_margin));
    Player::set_player_gui(&ui);
    //-------------------------------------------------------------------------------------------------

    game_map = std::make_shared<Map>(100, 100);
    DungeonGenerator dungeon_generator { game_map };
    dungeon_generator.generate();
    dungeon_generator.place_players(players);
    Map::curr_map = game_map.get();
    while (running) {
        Loop();
    }
    Clean_up();
}

void Game::Loop() {
    for (auto& it : game_map->get_actors()) {
        it->make_turn();
        if (!running)
            return;
    }
}

void Game::Clean_up() {
}
