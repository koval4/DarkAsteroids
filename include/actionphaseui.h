#ifndef ACTIONPHASEUI_H
#define ACTIONPHASEUI_H

#include "ui/panel.h"
#include "ui/textbox.h"
#include "ui/label.h"
#include "ui/button.h"
#include "player.h"

class ActionPhaseUI {
public:
    ActionPhaseUI();
    ~ActionPhaseUI();

    void update_weapon_panel(const Player& player);

private:
    static Textbox& setup_output();
    static Label&   setup_action_points_lbl();
    static Panel&   setup_weapon_panel();
    static Label&   setup_weapon_info();
    static Label&   setup_ammo_info();
    static Button&  setup_next_weapon_btn();
    static Button&  setup_prev_weapon_btn();
    static Button&  setup_reload_btn();

    Textbox& output;
    Label&   action_points_lbl;

    Panel&  weapon_panel;
    Label&  weapon_info;
    Label&  ammo_info;
    Button& next_weapon_btn;
    Button& prev_weapon_btn;
    Button& reload_btn;
};

#endif // ACTIONPHASEUI_H
