#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "actor.h"
#include "data.h"
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
        Weapon* curr_weap;

        void update_ap_lbl();       /// updates ap label text
        void update_weap_info();    /// updates labels in weapon_panel

        void switch_to_next_weapon();
        void switch_to_prev_weapon();
        void reload_weapon();

    public:
        Player();

        //############### SETTERS ################
        static void set_game_state(bool* running);

        //############## GAME LOGIC ################
        void generate();
        virtual void make_turn() override;
        virtual void pick_item() override;
        virtual void attack(Coord target) override;
        void show_inventory();
};

#endif // PLAYER_H
