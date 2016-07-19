#include "playercontroller.h"
#include <memory>
#include "controller.h"
#include "ui/event_handler.h"
#include "ui/gui.h"
#include "ui/listener.h"
#include "ui/screen.h"
#include "controllersmanager.h"
#include "player.h"
#include "tile.h"
#include "map.h"
#include "inventorycontroller.h"
#include "pickitemcontroller.h"
#include "attackcontroller.h"

PlayerController::PlayerController(
    const std::shared_ptr<ActionQueue>& action_queue,
    const Player::ptr& player,
    bool* game_state)
    : Controller(action_queue)
    , player(player)
    , game_state(game_state) {
    keyboard_listener = std::make_shared<Listener>(SDL_KEYDOWN);
    mouse_listener = std::make_shared<Listener>(SDL_MOUSEBUTTONDOWN);
}

PlayerController::~PlayerController() {}

void PlayerController::show_inventory() const {
    ControllersManager::inst().make_controller<InventoryController>(player);
}

void PlayerController::pick_item() const {
    ControllersManager::inst().make_controller<PickItemController>(
        player,
        player->get_map()->at(player->get_pos())
    );
}

void PlayerController::update_ap_lbl() {
    auto ap_lbl = GUI::inst().get_widget<Label>("ap_lbl");
    ap_lbl->set_text(
        std::to_string(player->get_params().action_points) + "/" + std::to_string(player->get_params().max_ap)
    );
}

void PlayerController::setup_ui() {
    update_ap_lbl();
}

void PlayerController::setup_handlers() {
    keyboard_listener->set_handler([this] (SDL_Event & event) -> void {
        auto pos = player->get_pos();
        switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                add_action([this] (void) -> void {
                    player->end_turn();
                });
                break;
            case SDLK_ESCAPE:
                // show_menu()
                add_action([this] (void) -> void {
                    *game_state = false;
                });
                break;
            case SDLK_g:
                add_action([this] (void) -> void {
                    pick_item();
                });
                break;
            case SDLK_i:
                add_action([this] (void) -> void {
                    show_inventory();
                });
                break;
            case SDLK_UP:
                add_action([this, pos] () -> void {
                    player->move_to({pos.x, static_cast<uint8_t>(pos.y - 1) });
                });
                break;
            case SDLK_DOWN:
                add_action([this, pos] () -> void {
                    player->move_to({pos.x, static_cast<uint8_t>(pos.y + 1)});
                });
                break;
            case SDLK_LEFT:
                add_action([this, pos] () -> void {
                    player->move_to({static_cast<uint8_t>(pos.x - 1), pos.y});
                });
                break;
            case SDLK_RIGHT:
                add_action([this, pos] () -> void {
                    player->move_to({static_cast<uint8_t>(pos.x + 1), pos.y});
                });
                break;
            default:
                break;
        }
    });

    mouse_listener->set_handler([this] (SDL_Event & event) -> void {
        auto tiles_hor = Screen::inst().get_width() / Tile::size;
        auto tiles_vert = Screen::inst().get_height() / Tile::size;
        // calculating position on map that has been clicked
        Coord mpos = {
            static_cast<uint8_t>((player->get_pos().x - tiles_hor / 2) + event.button.x / Tile::size),
            static_cast<uint8_t>((player->get_pos().y - tiles_vert / 2) + event.button.y / Tile::size)
        };

        // TODO: add enemy check to attack-on-click

        auto actor_at_mpos = player->get_map()->at(mpos)->get_actor();
        if (actor_at_mpos && actor_at_mpos != player) {
            add_action([this, actor_at_mpos] (void) -> void {
                ControllersManager::inst().make_controller<AttackController>(player, actor_at_mpos);
            });
        } else player->move_to(mpos);
    });

    handler.listen(keyboard_listener.get());
    handler.listen(mouse_listener.get());
}

void PlayerController::clear_ui() const {

}
