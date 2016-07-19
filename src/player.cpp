#include "player.h"
#include <memory>
#include <functional>
#include <algorithm>
#include "actor.h"
#include "data.h"
#include "tile.h"
#include "map.h"
#include "map_drawer.h"
#include "ui/gui.h"
#include "ui/screen.h"
#include "ui/textbox.h"
#include "ui/label.h"
#include "ui/button.h"
#include "ui/panel.h"
#include "ui/listbox.h"
#include "ui/listener.h"
#include "weapon.h"
#include "armor.h"
#include "container.h"

bool* Player::game_running = nullptr;

Player::Player(
        std::string texture,
        std::string name,
        std::string description,
        Params params,
        Skills skills,
        Race race)
    : Actor(texture, name, description, params, skills, race)
    , curr_weap(nullptr) {}

void Player::update_ap_lbl() {
    auto ap_lbl = GUI::inst().get_widget("ap_lbl");
    std::dynamic_pointer_cast<Label>(ap_lbl)->set_text(
        std::to_string(params.action_points) + "/" + std::to_string(params.max_ap)
    );
}

void Player::update_weap_info() {
    auto weapon_info = std::dynamic_pointer_cast<Label>(GUI::inst().get_widget("weapon_info"));
    auto ammo_info = std::dynamic_pointer_cast<Label>(GUI::inst().get_widget("ammo_info"));
    if (curr_weap != nullptr) {
        weapon_info->set_text(curr_weap->get_name());
        // first ammo type in weapon ammo info is main caliber
        auto weap_ammo = curr_weap->get_ammo_info().begin();
        ammo_info->set_text( weap_ammo->first
                           + ": " + std::to_string(weap_ammo->second.ammo_left)
                           + "/" + std::to_string(weap_ammo->second.max_ammo)
                           );
    } else {
        weapon_info->set_text("Empty");
        ammo_info->set_text("No ammo");
    }
}

void Player::set_game_state(bool* running) {
    game_running = running;
}

std::shared_ptr<Player> Player::generate() {
    std::string texture = img("player.png");
    std::string name = "Player";
    std::string description = "Player";
    Params params = Params::rand_params();
    params.calc_secondary_params();
    Skills skills;
    auto race = Race::RACES_LIST.at("Human");

    auto player = std::make_shared<Player>(texture, name, description, params, skills, race);

    player->params.action_points += 20;
    player->equip_item(Weapon::make("CPP-17"));
    player->equip_item(Armor::make("Pressure suit"));
    Item::ptr backpack = Container::make("Backpack");
    for (uint8_t i = 0; i < 100; i++)
        std::dynamic_pointer_cast<Container>(backpack)->add_item(Item::make("5.56x45 ammo"));
    player->equip_item(backpack);

    return player;
}

void Player::reload_weapon() {
//    // searching in inventory for ammo
//    for (auto& container : containers) {
//        for (auto& item : container->open_container()) {
//            for (auto& ammo_type : curr_weap->get_ammo_info()) {
//                if (  item->get_name().find(ammo_type.second.type) != std::string::npos
//                        && (ammo_type.second.ammo_left < ammo_type.second.max_ammo)
//                   ) {
//                    Stack* ammo_stack = dynamic_cast<Stack*>(item);
//                    curr_weap->reload(ammo_type.second.type, *ammo_stack);
//                    if (ammo_stack->get_size() == 0)
//                        container->remove_item(ammo_stack);
//                }
//            }
//        }
//    }
//    for (auto& grasp : grasps) {
//        for (auto& item : grasp.second->get_grabbed_items()) {
//            for (auto& ammo_type : curr_weap->get_ammo_info()) {
//                if (  item->get_name().find(ammo_type.second.type) != std::string::npos
//                        && (ammo_type.second.ammo_left < ammo_type.second.max_ammo)
//                   ) {
//                    Stack::ptr ammo_stack = std::dynamic_pointer_cast<Stack>(item);
//                    curr_weap->reload(ammo_type.second.type, *ammo_stack);
//                    if (ammo_stack->get_size() == 0)
//                        grasp.second->remove_item(ammo_stack);
//                }
//            }
//        }
//    }
}

void Player::make_turn() {
    params.action_points = params.max_ap;
    update_ap_lbl();
    // current weapon is the first available
//    if (curr_weap == nullptr && !weapons.empty())
//        curr_weap = *weapons.begin();
    update_weap_info();
    // setting handlers for weapon changing
    auto next_weap = GUI::inst().get_widget<Button>("next_weap");
    auto prev_weap = GUI::inst().get_widget<Button>("prev_weap");
    auto reload_btn = GUI::inst().get_widget<Button>("reload_btn");
//    next_weap->on_click(std::bind(&Player::switch_to_next_weapon, this));
//    prev_weap->on_click(std::bind(&Player::switch_to_prev_weapon, this));
    reload_btn->on_click(std::bind(&Player::reload_weapon, this));
}

void Player::pick_item() {}

void Player::attack(Coord target) {
//    bool done = false;
//    Padding margin = {3};
//    auto btns_height = 70;
//    Attack::ptr atk;

//    //----------------------interface-creation--------------------------
//    auto spacing = 3;
//    auto panel_width = 300;
//    auto panel_x = (Screen::inst().get_width() - panel_width) / 2;
//    SDL_Rect panel_form = {panel_x, 50, panel_width, Screen::inst().get_height() - 100};
//    Panel panel(panel_form, margin);

//    auto attacks_x = panel_form.x + margin.left;
//    auto attacks_y = panel_form.y + margin.top;
//    auto attacks_width = panel_form.w - margin.left - margin.right;
//    auto attacks_height = panel_form.h - margin.top - margin.bottom - spacing - btns_height;
//    SDL_Rect attacks_form = {attacks_x, attacks_y, attacks_width, attacks_height};
//    Listbox attacks_list(attacks_form);

//    auto btns_y = attacks_form.y + attacks_form.h + spacing;
//    auto btns_width = attacks_width / 2 - spacing;
//    SDL_Rect attack_btn_form = {attacks_x, btns_y, btns_width, btns_height};
//    Button attack_btn("Attack", attack_btn_form, margin);

//    auto aim_btn_x = attack_btn_form.x + attack_btn_form.w + spacing;
//    SDL_Rect aim_btn_form = {aim_btn_x, btns_y, btns_width, btns_height};
//    Button aim_btn("Aim", aim_btn_form, margin);
//    //----------------------------------------------------------------------

//    //----------------------adding-attacks-to-list--------------------------
//    std::vector<Attack::ptr> attacks;
//    for (auto& attack : curr_weap->get_available_attacks()) {
//        // TODO: add skill check for available attacks
//        if (attack->get_cost() <= params.action_points)
//            attacks.push_back(attack);
//    }
//    if (attacks.empty())
//        return;
//    for (auto& it : attacks)
//        attacks_list.add_item(it->get_name());
//    //---------------------------------------------------------------------

//    //----------------------setting-handlers-----------------------------
//    attacks_list.on_item_clicked([&atk, &attacks] (std::string item) -> void {
//        for (auto& it : attacks) {
//            if (it->get_name() == item) {
//                atk = it;
//                return;
//            }
//        }
//    });

//    attack_btn.on_click([this, &atk, &target, &done] () -> void {
//        if (!atk)
//            return;
//        params.action_points -= atk->get_cost();
//        curr_weap->make_attack(*this, atk, this->pos, target, map->at(target)->get_actor().get());
//        update_weap_info();
//        done = true;
//    });
//    //------------------------------------------------------------------

//    panel.set_widgets({&attacks_list, &attack_btn, &aim_btn});
//    GUI::inst().add_widget(&panel);
//    GUI::inst().update();

//    Listener listener(SDL_KEYDOWN);
//    EventHandler processor;
//    processor.listen(&listener);
//    processor.listen(panel);
//    listener.set_handler([this, &done] (SDL_Event & event) -> void {
//        switch (event.key.keysym.sym) {
//            case SDLK_ESCAPE :
//                event.type = 0;
//                done = true;
//                break;
//            default:
//                break;
//        }
//    });

//    processor.run([this, &done] (void) -> bool { return !done && params.action_points > 0 && *game_running; });
//    GUI::inst().remove_widget(&panel);
}

void Player::show_inventory() { }
