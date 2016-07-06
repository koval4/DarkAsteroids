#ifndef INVENTORYCONTROLLER_H
#define INVENTORYCONTROLLER_H

#include <memory>
#include "ui/listener.h"
#include "ui/panel.h"
#include "ui/listbox.h"
#include "ui/textbox.h"
#include "ui/button.h"
#include "controller.h"
#include "player.h"
#include "item.h"

class InventoryController : public Controller {
    private:
        const Player::ptr player;
        Item::ptr selected_item;
        std::string selected_str;
        Listener::ptr keyboard_listener;
        Listener::ptr mouse_listener;

        Panel::ptr panel;
        Listbox::ptr items_list;
        Textbox::ptr item_descr;
        Button::ptr drop_btn;
        Button::ptr done_btn;

        virtual void setup_ui() override;
        virtual void clear_ui() const override;

   public:
        InventoryController(
                const EventHandler::ptr& handler,
                const std::shared_ptr<ActionQueue>& action_queue,
                const Player::ptr player);
        ~InventoryController();
};

#endif // INVENTORYCONTROLLER_H
