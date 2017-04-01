#ifndef WEAPON_H
#define WEAPON_H

#include <memory>
#include <map>
#include <string>
#include <vector>
#include "item.h"
#include "itemsstack.h"
#include "attack.h"
#include "common.h"

class Actor;

class Weapon : public Item {
    public:
        typedef std::shared_ptr<Weapon> ptr;
        static std::map<std::string, Weapon> LIST;
        /**
         * @brief The Ammo struct
         */
        struct Ammo {
            std::string type;   /// which ammo type weapon is using (part of ammo's name)
            Item::ptr ammo;     /// loaded ammo
            uint8_t max_ammo;	/// how much ammo is possible to load in this weapon
            uint8_t ammo_left;  /// how much ammo left loaded

            Ammo();
            Ammo(const Ammo& obj);
            Ammo(std::string line);
        };

        enum Hold_type {
            one_handed,
            two_handed
        };

    private:
        std::map<std::string, Ammo> ammo;           /// information about ammo used in this weapon
        Hold_type hold_type;                        /// type of a weapon's hold (on- or two-handed)
        std::vector<Attack::ptr> available_attacks; /// attacks that could be performed with this weapon

        bool check_ammo(Attack::ptr attack) const;
    public:
        //############# CONSTRUCTORS #############
        Weapon();
        Weapon(const Weapon& obj);

        //############### PTRS #################
        static Item::ptr make(std::string prototype);
        virtual Item::ptr clone() override;

        //################ DATABASE ################
        /**
         * @brief read_weapons_txt
         */
        static void read_weapons_txt();

        //############## GETTERS ###############
        Hold_type get_hold_type() const;
        std::map<std::string, Ammo> get_ammo_info() const;
        std::vector<Attack::ptr> get_available_attacks() const;

        //############## GAME LOGIC ###############
        void make_attack(Actor& user
                        , Attack::ptr attack_type
                        , Coord start
                        , Coord end
                        , Actor* victim = nullptr
                        , Bodypart* target = nullptr
                        );
        void reload(std::string ammo_type, Stack& ammo_stack);
        virtual std::string describe() override;
};

#endif // WEAPON_H
