#ifndef CLOSE_ATTACK_H
#define CLOSE_ATTACK_H

#include <string>
#include <memory>
#include "attack.h"

class CloseAttack : public Attack {
    public:
        typedef std::shared_ptr<CloseAttack> ptr;

    private:
    public:
        //########## CONSTRUCTORS ##############
        CloseAttack();
        CloseAttack(const CloseAttack& obj);
        CloseAttack(Weapon* parent, std::string line);

        //############## GAME LOGIC #############
        /**
         * @brief spawn_projectile -- getter for object that deals damage
         * @return                 -- item that will deal damage
         */
        Item::ptr spawn_projectile() override;
        /**
         * @brief make_attack -- performs attack with given params
         * @param user        -- actor that performs attack
         * @param start       -- position of user
         * @param end         -- target position
         * @param victim      -- actor that gets damage
         * @param target      -- aimed victim's bodypart
         */
        void make_attack( Actor& user
                        , Coord start
                        , Coord end
                        , Actor* victim = nullptr
                        , Bodypart* target = nullptr
                        ) override;
};

#endif // CLOSE_ATTACK_H
