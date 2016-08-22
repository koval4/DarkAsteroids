#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <memory>
#include "data.h"
#include "bodypart.h"
#include "race.h"
#include "body.h"
#include "inventory.h"
#include "container.h"

/// forward declaration of Map class
class Map;
class Tile;
class Attack;
class Group;

/**
 * @brief The Actor class describes game logic of characters
 */
class Actor {
    public:
        using ptr = std::shared_ptr<Actor>;
        using wptr = std::weak_ptr<Actor>;

        /**
         * @brief The Params struct
         */
        struct Params {
            uint16_t strength = 0;
            uint16_t perception = 0;
            uint16_t agility = 0;
            uint16_t speed = 0;
            uint16_t endurance = 0;

            uint16_t max_ap = 0;            /// action points number at the beginning of a turn
            uint16_t action_points = 0;     /// how much action points actor has now
            uint16_t max_weight = 0;        /// how much weight actor could carry
            uint16_t curr_weight = 0;       /// how much weight actor carries now
            uint16_t visibility_range = 0;  ///
            int pain_threshold = 0;         /// how much pain actor could withstand
            int curr_pain = 0;              /// how much pain actor feels now

            Params();
            Params( uint16_t strength
                  , uint16_t perception
                  , uint16_t agility
                  , uint16_t speed
                  , uint16_t endurance);

            static Params rand_params();    /// randomises parameters of actor
            void calc_secondary_params();   /// calculates secondary parameters of the actor (such as action points,max weight etc.)
        };

        /**
         * @brief The Skills struct
         */
        struct Skills {
            uint16_t close_combat = 0;  /// skill of unarmed combat and using cold weapons
            uint16_t rifles = 0;        /// skill of using any rifle
            uint16_t pistols = 0;       /// skill of using any pistol
        };

    protected:
        Coord pos;                  /// position of character in map
        std::string name;           /// name of the actor
        std::string texture;        /// texture of character used for rendering
        std::string description;    /// description of the actor
        uint16_t level;             ///
        uint16_t experience;        ///
        Params params;              ///
        Skills skills;              ///
        Race race;                  ///
        Body body;                  /// container with bodyparts of this actor
        std::weak_ptr<Group> group;

        bool making_turn;

        Inventory inventory;

        const Map* map;                   /// map where actor is situated

        void calc_skills();             /// calculates starting skills of the actor

        //############## BODY ################

        void feel_pain();

        //##########################

        /**
         * @brief find_path_to -- a* algorithm that searches path from the actor's position to position passed to function
         * @param pos -- position of the end of a path
         * @return path from current position to pos
         */
        std::vector<Coord> find_path_to(Coord pos);

    public:
        //############# CONSTRUCTORS ###############
        Actor(std::string texture,
              std::string name,
              std::string description,
              Params params,
              Skills skills,
              Race race,
              uint16_t level = 1,
              uint16_t experience = 0,
              const Map* map = nullptr);
        Actor(const Actor& obj);

        virtual ~Actor();

        //############### GETTERS ##################
        std::string get_texture() const;
        Coord get_pos() const;
        Params get_params() const;
        const Map* get_map() const;
        const std::shared_ptr<Tile> get_tile() const;
        const Inventory& get_inventory() const;

        //############# SETTERS ###################
        void set_pos(Coord pos);
        void set_map(const Map* map);

        //############# GAME LOGIC #################
        bool can_make_turn() const;
        virtual void make_turn() = 0;   /// virtual function wich describes logic of what actor doing in one turn
        virtual void pick_item() = 0;
        virtual void attack(Coord target) = 0;
        /**
         * @brief move_to tries to move player from current psition to position passed to function
         * @param pos
         */
        void move_to(Coord pos);
        /**
         * @brief get_wound
         * @param momentum
         * @param item
         * @param contact_area
         * @param target
         */
        void get_wound( int32_t& momentum
                      , Item& item
                      , uint32_t contact_area
                      , Bodypart* target = nullptr
                      );
        bool is_alive() const;
        void die();
        std::vector<std::shared_ptr<Attack>> get_available_attacks() const;
        void start_turn();
        void end_turn();
        void decrease_action_points(uint16_t value);
        bool is_enemy(const Actor& other) const;

        //############ INVENTORY ################
        /**
         * @brief equip_item -- adds item to inventory
         * @param item       -- item that will be added to inventory
         */
        void equip_item(Item::ptr item);
        /**
         * @brief grab_item -- adds item to grasps
         * @param item      -- item that will be added to grasps
         */
        void grab_item(Item::ptr item);
        /**
         * @brief drop_item -- removes item from inventory
         * @param item      -- item that will be removed from inventory
         */
        void drop_item(Item::ptr item);
        const std::shared_ptr<Weapon> get_current_weapon() const;
};

#endif // ACTOR_H
