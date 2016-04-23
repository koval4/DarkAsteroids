#ifndef ATTACK_H
#define ATTACK_H

#include <string>
#include <memory>
#include "item.h"
#include "actor.h"

class Attack {
    public:
        typedef std::shared_ptr<Attack> ptr;

    protected:
        Weapon* parent;             /// weapon that has this attack
        std::string name;			/// name of the attack
        std::string message;		/// message shown when attack performed
        uint8_t ap_cost;            /// how much action points is consumed to perform an attack
        uint32_t contact_area;		/// contact area of projectile with target
        uint32_t start_force;		/// force implied to ammo at the start
        uint8_t range;				/// usage range of weapon
        uint32_t thermal_energy;	/// thremal energy of projectile at the beginning of attack

    public:
        //############## CONSTRUCTORS ##############
        Attack();
        Attack(const Attack& obj);
        Attack(Weapon* parent, std::string line);

        //############### GETTERS ##############
        std::string get_name() const;
        std::string get_message() const;
        uint8_t get_cost() const;
        uint32_t get_contact_area() const;
        uint32_t get_start_force() const;
        uint8_t get_range() const;
        uint32_t get_thermal_energy() const;

        //############## SETTERS ################
        void set_parent(Weapon* parent);

        //############## GAME LOGIC #############
        /**
         * @brief spawn_projectile -- getter for object that deals damage
         * @return                 -- item that will deal damage
         */
        virtual Item::ptr spawn_projectile() = 0;
        /**
         * @brief make_attack -- performs attack with given params
         * @param user        -- actor that performs attack
         * @param start       -- position of user
         * @param end         -- target position
         * @param victim      -- actor that gets damage
         * @param target      -- aimed victim's bodypart
         */
        virtual void make_attack( Actor& user
                                , Coord start
                                , Coord end
                                , Actor* victim = nullptr
                                , Bodypart* target = nullptr
                                ) = 0;
};

#endif // ATTACK_H
