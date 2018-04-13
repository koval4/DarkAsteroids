#ifndef PICKITEMCONTROLLER_H
#define PICKITEMCONTROLLER_H

#include <memory>
#include "controller.h"
#include "item.h"
#include "tile.h"
#include "player.h"
#include "ui/panel.h"
#include "ui/listbox.h"
#include "ui/textbox.h"
#include "ui/button.h"

class PickItemController : public Controller {
    private:
        std::shared_ptr<Item> selected_item;

        Player& player;
        const std::shared_ptr<Tile> tile;

        std::shared_ptr<Panel> panel;
        std::shared_ptr<Listbox> items_list;
        std::shared_ptr<Textbox> item_descr;
        std::shared_ptr<Button> done_btn;
        std::shared_ptr<Button> pick_btn;

    public:
        PickItemController(
            const std::shared_ptr<ActionQueue>& action_queue,
            Player& player,
            const std::shared_ptr<Tile> tile);
        ~PickItemController();

        virtual void setup_ui() override;
        virtual void setup_handlers() override;
        virtual void clear_ui() const override;
};

#endif // PICKITEMCONTROLLER_H
