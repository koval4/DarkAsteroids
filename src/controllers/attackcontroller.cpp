#include "controllers/attackcontroller.h"
#include "ui/screen.h"
#include "ui/gui.h"
#include "controllers/controllersmanager.h"

AttackController::AttackController(
    const std::shared_ptr<ActionQueue>& action_queue,
    const std::shared_ptr<Player>& player,
    const std::shared_ptr<Actor>& target)
    : Controller(action_queue)
    , player(player)
    , target(target)
    , available_attacks(player->get_available_attacks()) {}

AttackController::~AttackController() {}

void AttackController::setup_ui() {
    if (available_attacks.empty()) {
        add_action([] (void) -> void {
            ControllersManager::inst().remove_controller<AttackController>();
        });
        return;
    }

    Padding padding = {3};
    auto spacing = 3;
    auto btns_height = 70;

    auto panel_width = 300;
    auto panel_x = (Screen::inst().get_width() - panel_width) / 2;
    SDL_Rect panel_form = {panel_x, 50, panel_width, Screen::inst().get_height() - 100};
    panel = GUI::inst().make_widget<Panel>("attack_panel", panel_form, padding);


    auto attacks_x = panel_form.x + padding.left;
    auto attacks_y = panel_form.y + padding.top;
    auto attacks_width = panel_form.w - padding.left - padding.right;
    auto attacks_height = panel_form.h - padding.top - padding.bottom - spacing - btns_height;
    SDL_Rect attacks_form = {attacks_x, attacks_y, attacks_width, attacks_height};
    attacks_list = GUI::inst().make_widget<Listbox>("attacks_list", attacks_form);

    auto btns_y = attacks_form.y + attacks_form.h + spacing;
    auto btns_width = attacks_width / 2 - spacing;
    SDL_Rect attack_btn_form = {attacks_x, btns_y, btns_width, btns_height};
    attack_btn = GUI::inst().make_widget<Button>("attack_btn", "Attack", attack_btn_form, padding);

    auto aim_btn_x = attack_btn_form.x + attack_btn_form.w + spacing;
    SDL_Rect aim_btn_form = {aim_btn_x, btns_y, btns_width, btns_height};
    aim_btn = GUI::inst().make_widget<Button>("aim_btn", "Aim", aim_btn_form, padding);

    for (const auto& attack : available_attacks)
        attacks_list->add_item(attack->get_name());
}

void AttackController::setup_handlers() {
    attacks_list->on_item_clicked([this] (std::string clicked_item) -> void {
        for (const auto& attack : available_attacks) {
            if (attack->get_name() == clicked_item) {
                selected_attack = attack;
                return;
            }
        }
    });

    attack_btn->on_click([this] (void) -> void {
        if (!selected_attack)
            return;
        add_action([this] (void) -> void {
            player->decrease_action_points(selected_attack->get_cost());
            player->get_current_weapon()->make_attack(
                *player,
                selected_attack,
                player->get_pos(),
                target->get_pos(),
                target.get()
            );
            ControllersManager::inst().remove_controller<AttackController>();
        });
    });

    handler.listen(*attacks_list);
    handler.listen(*attack_btn);
}

void AttackController::clear_ui() const {
    GUI::inst().remove_widget(aim_btn->get_access_name());
    GUI::inst().remove_widget(attack_btn->get_access_name());
    GUI::inst().remove_widget(attacks_list->get_access_name());
    GUI::inst().remove_widget(panel->get_access_name());
}
