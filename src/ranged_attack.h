#ifndef RANGED_ATTACK_H
#define RANGED_ATTACK_H

#include <string>
#include <memory>
#include "attack.h"

class RangedAttack : public Attack {
    public:
        typedef std::shared_ptr<RangedAttack> ptr;

    private:
        uint8_t ammo_usage;			/// how much ammo this attack uses
        uint8_t accuracy;           /// how accurate attack is (0..100)
        std::string ammo_type;      /// ammo type used for attack
        std::string projectile;		/// projectile spawned at attack

    public:
        //############## CONSTRUCTORS ##############
        RangedAttack();
        RangedAttack(const RangedAttack& obj);
        RangedAttack(Weapon* parent, std::string line);

        //########### GETTERS ##############
        uint8_t get_ammo_usage() const;
        uint8_t get_accuracy() const;
        std::string get_ammo_type() const;
        std::string get_projectile() const;

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

#endif // RANGED_ATTACK_H
