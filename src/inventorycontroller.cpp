#include "inventorycontroller.h"
#include <memory>
#include "ui/listener.h"
#include "ui/gui.h"
#include "ui/panel.h"
#include "ui/listbox.h"
#include "ui/textbox.h"
#include "ui/button.h"
#include "controller.h"
#include "controllersmanager.h"
#include "item.h"
#include "inventory.h"
#include "player.h"

InventoryController::InventoryController(
    const std::shared_ptr<ActionQueue>& action_queue,
    const Player::ptr player)
    : Controller (action_queue)
    , player(player) {}

InventoryController::~InventoryController() {}

void InventoryController::setup_ui() {
    SDL_Rect panel_form = {100, 100, 600, 400};
    Padding big_padding = {5};
    panel = GUI::inst().make_widget<Panel>("inventory_panel", panel_form, big_padding);

    SDL_Rect items_list_form = {105, 105, 290, 390};
    items_list = GUI::inst().make_widget<Listbox>("items_list", items_list_form);

    SDL_Rect item_descr_form = {405, 105, 290, 390};
    item_descr = GUI::inst().make_widget<Textbox>("item_descr", item_descr_form, big_padding, true);

    Padding small_padding = {3};
    SDL_Rect drop_btn_form = {500, 410, 100, 30};
    drop_btn = GUI::inst().make_widget<Button>("drop_btn", "Drop it", drop_btn_form, small_padding);

    SDL_Rect done_btn_form = {500, 465, 100, 30};
    done_btn = GUI::inst().make_widget<Button>("done_btn", "Done", done_btn_form, small_padding);

    panel->set_widgets({ items_list, item_descr, drop_btn, done_btn });

    for (auto& item : player->get_inventory().list_items())
        items_list->add_item(item.first + " : " + item.second);
}

void InventoryController::setup_handlers() {
    items_list->on_item_clicked([this] (std::string clicked_item) -> void {
        add_action([this, clicked_item] (void) -> void {
            selected_str = clicked_item;
            std::string slot = clicked_item.substr(0, clicked_item.find(" : "));
            std::string item_name = clicked_item.substr(clicked_item.find(" : ") + 3);
            selected_item = this->player->get_inventory().get_item_by_name(slot, item_name);
            if (selected_item)
                item_descr->set_text(selected_item->describe());
        });
    });

    drop_btn->on_click([this] (void) -> void {
        add_action([this] (void) -> void {
            if (!selected_item)
                return;
            this->player->drop_item(selected_item);
            item_descr->clear_text();
            items_list->remove_item(selected_str);
        });
    });

    done_btn->on_click([this] (void) -> void {
        add_action([] (void) -> void {
            ControllersManager::inst().remove_controller<InventoryController>();
        });
    });

    handler.listen(*panel);
}

void InventoryController::clear_ui() const {
    GUI::inst().remove_widget("inventory_panel");
    GUI::inst().remove_widget("items_list");
    GUI::inst().remove_widget("item_descr");
    GUI::inst().remove_widget("drop_btn");
    GUI::inst().remove_widget("done_btn");
}
