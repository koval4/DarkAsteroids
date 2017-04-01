#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "actor.h"
#include "common.h"
#include "ui/textbox.h"
#include "ui/label.h"
#include "ui/button.h"
#include "ui/panel.h"

/**
 * @brief The Player class
 */
class Player : public Actor {
    public:
        typedef std::shared_ptr<Player> ptr;

    private:
        static bool* game_running;  /// pointer to game state

        void update_ap_lbl();       /// updates ap label text
        void update_weap_info();    /// updates labels in weapon_panel

        void reload_weapon();

    public:
        Player(
            std::string texture,
            std::string name,
            std::string description,
            Params params,
            Skills skills,
            Race race
        );

        //############### SETTERS ################
        static void set_game_state(bool* running);

        //############## GAME LOGIC ################
        static std::shared_ptr<Player> generate();
        virtual void make_turn() override;
        virtual void pick_item() override;
        virtual void attack(Coord target) override;
        void show_inventory();
};

#endif // PLAYER_H
