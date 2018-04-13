#include "actionphaseui.h"
#include "ui/gui.h"

ActionPhaseUI::ActionPhaseUI()
    : output(setup_output())
    , action_points_lbl(setup_action_points_lbl())
    , weapon_panel(setup_weapon_panel())
    , weapon_info(setup_weapon_info())
    , ammo_info(setup_ammo_info())
    , next_weapon_btn(setup_next_weapon_btn())
    , prev_weapon_btn(setup_prev_weapon_btn())
    , reload_btn(setup_reload_btn()) {}

ActionPhaseUI::~ActionPhaseUI() {
    
}

void ActionPhaseUI::update_weapon_panel(const Player &player) {
    
}

Textbox& ActionPhaseUI::setup_output() {
    SDL_Rect output_form = {0, 400, 250, 200};
    return *GUI::inst().make_widget<Textbox>("output", output_form, Padding {5, 5, 5, 5}, true);
}

Label& ActionPhaseUI::setup_action_points_lbl() {
    auto ap_lbl_width = 50;
    auto ap_lbl_height = 30;
    auto ap_lbl_x = Screen::inst().get_width() - ap_lbl_width - 5;
    auto ap_lbl_y = Screen::inst().get_height() - ap_lbl_height - 5;
    SDL_Rect ap_lbl_form = {ap_lbl_x, ap_lbl_y, ap_lbl_width, ap_lbl_height};
    return *GUI::inst().make_widget<Label>("ap_lbl", "0/0", ap_lbl_form);
}

Panel& ActionPhaseUI::setup_weapon_panel() {
    Padding standart_padding = {3, 3, 3, 3};
    SDL_Rect weap_panel_form = {250, Screen::inst().get_height() - 70, 150, 70};
    return *GUI::inst().make_widget<Panel>("weapon_panel", weap_panel_form, standart_padding);
}

Label& ActionPhaseUI::setup_weapon_info() {
    SDL_Rect weap_info_form = {weap_panel_form.x + 3, weap_panel_form.y + 3, 120, 20};
    return *GUI::inst().make_widget<Label>("weapon_info", "Empty", weap_info_form);
}

Label& ActionPhaseUI::setup_ammo_info() {
    auto ammo_info_y = weap_info_form.y + weap_info_form.h + 3;
    SDL_Rect ammo_info_form = {weap_panel_form.x + 3, ammo_info_y, 120, 20};
    return *GUI::inst().make_widget<Label>("ammo_info", "No ammo", ammo_info_form);
}

Button& ActionPhaseUI::setup_next_weapon_btn() {
    auto next_weap_x = weap_info_form.x + weap_info_form.w + 3;
    SDL_Rect next_weap_form = {next_weap_x, weap_panel_form.y + 3, 20, 20};
    return *GUI::inst().make_widget<Button>("next_weap", img("up_arrow.png"), next_weap_form, standart_padding);
}

Button& ActionPhaseUI::setup_prev_weapon_btn() {
    auto prev_weap_x = ammo_info_form.x + ammo_info_form.w + 3;
    SDL_Rect prev_weap_form = {prev_weap_x, ammo_info_form.y + 3, 20, 20};
    return *GUI::inst().make_widget<Button>("prev_weap", img("down_arrow.png"), prev_weap_form, standart_padding);
}

Button& ActionPhaseUI::setup_reload_btn() {
    auto reload_y = ammo_info_form.y + ammo_info_form.h + 3;
    SDL_Rect reload_form = { weap_panel_form.x + 3, reload_y, 144, 20};
    return *GUI::inst().make_widget<Button>("reload_btn", "Reload", reload_form, standart_padding);
}
