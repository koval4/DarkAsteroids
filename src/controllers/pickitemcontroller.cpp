#include "controllers/pickitemcontroller.h"
#include <memory>
#include "controllers/controller.h"
#include "controllers/controllersmanager.h"
#include "tile.h"
#include "player.h"
#include "ui/gui.h"
#include "ui/panel.h"
#include "ui/listbox.h"
#include "ui/textbox.h"
#include "ui/button.h"

PickItemController::PickItemController(const std::shared_ptr<ActionQueue>& action_queue,
    Player& player,
    const std::shared_ptr<Tile> tile)
    : Controller (action_queue)
    , player(player)
    , tile(tile) {}

PickItemController::~PickItemController() {}

void PickItemController::setup_ui() {
    SDL_Rect panel_form = {100, 100, 600, 400};
    Padding big_padding = {5};
    panel = GUI::inst().make_widget<Panel>("pick_item_panel", panel_form, big_padding);

    SDL_Rect items_list_form = {105, 105, 290, 390};
    items_list = GUI::inst().make_widget<Listbox>("items_list", items_list_form);

    SDL_Rect item_descr_form = {405, 105, 290, 300};
    item_descr = GUI::inst().make_widget<Textbox>("item_descr", item_descr_form, big_padding, true);

    Padding small_padding = {3};
    SDL_Rect drop_btn_form = {500, 410, 100, 30};
    pick_btn = GUI::inst().make_widget<Button>("pick_btn", "Drop it", drop_btn_form, small_padding);

    SDL_Rect done_btn_form = {500, 465, 100, 30};
    done_btn = GUI::inst().make_widget<Button>("done_btn", "Done", done_btn_form, small_padding);

    panel->set_widgets({ items_list, item_descr, pick_btn, done_btn });

    for (const auto& item : tile->get_items())
        items_list->add_item(item->get_name());
}

void PickItemController::setup_handlers() {
    items_list->on_item_clicked([this] (std::string clicked_item) -> void {
        add_action([this, clicked_item] (void) -> void {
            for (const auto& item : this->tile->get_items()) {
                if (item->get_name() == clicked_item) {
                    selected_item = item;
                    break;
                }
            }
            if (selected_item)
                item_descr->set_text(selected_item->describe());
        });
    });

    pick_btn->on_click([this] (void) -> void {
        add_action([this] (void) -> void {
            item_descr->clear_text();
            items_list->remove_item(selected_item->get_name());
            this->tile->take_item(selected_item);
            this->player.equip_item(selected_item);
        });
    });

    done_btn->on_click([this] (void) -> void {
        add_action([] (void) -> void {
            ControllersManager::inst().remove_controller<PickItemController>();
        });
    });

    handler.listen(*panel);
}

void PickItemController::clear_ui() const {
    GUI::inst().remove_widget("pick_item_panel");
    GUI::inst().remove_widget("items_list");
    GUI::inst().remove_widget("item_descr");
    GUI::inst().remove_widget("pick_btn");
    GUI::inst().remove_widget("done_btn");
}
