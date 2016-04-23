#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <memory>
#include "data.h"
#include "bodypart.h"
#include "container.h"

/// forward declaration of Map class
class Map;

/**
 * @brief The Actor class describes game logic of characters
 */
class Actor {
    public:
        typedef std::unique_ptr<Actor> ptr;

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
        };

        /**
         * @brief The Skills struct
         */
        struct Skills {
            uint16_t close_combat = 0;  /// skill of unarmed combat and using cold weapons
            uint16_t rifles = 0;        /// skill of using any rifle
            uint16_t pistols = 0;       /// skill of using any pistol
        };

        typedef std::unordered_map<std::string, Bodypart::ptr> Body;

        struct Race {
            std::string name;

			std::unordered_map<std::string, Bodypart> body;
            uint16_t max_body_size;
            uint16_t min_body_size;

            Race();
			Race(const Race& obj);
        };

        static std::unordered_map<std::string, Race> RACES_LIST;

    protected:
        Coord pos;                  /// position of character in map
        std::string texture;        /// texture of character used for rendering
        std::string name;           /// name of the actor
        std::string description;    /// description of the actor
        uint16_t level;             ///
        uint16_t experience;        ///
        Params params;              ///
        Skills skills;              ///
        Race race;                  ///
        Body body;                  /// container with bodyparts of this actor
        uint16_t body_size;         /// total size of a body

        std::unordered_map<std::string, Bodypart*> equipment;   /// actor equips Item in slot(string type) , so map makes some slots and fills it with items
        std::unordered_map<std::string, Bodypart*> grasps;      /// here actor holds weapon, items etc.
        std::vector<Weapon*> weapons;                           /// currently equipped weapons
        std::vector<Container::ptr> containers;                     /// currently equipped containers

        Map* map;                   /// map where actor is situated

        void rand_params();             /// randomises parameters of actor
        void calc_secondary_params();   /// calculates secondary parameters of the actor (such as action points,max weight etc.)
        void calc_skills();             /// calculates starting skills of the actor

        //############## BODY ################

        void make_body();
        /**
         * @brief make_slots
         */
        void make_slots();
        /**
         * @brief find_slots -- searches specific bodyparts by part of a name
         * @param slot       -- slot name used for search
         * @return           -- slots that contains 'slot' in name
         */
        PAIRS<std::string, Bodypart*> find_slots(std::string slot);
        /**
         * @brief find_less_loaded_bpart -- searches bodypart with least weight of equipment
         * @param slots                  -- slots where bodypart is searched
         * @return                       -- bodypart with minimal equipment weight
         */
        Bodypart* find_less_loaded_bpart(PAIRS<std::string, Bodypart*> slots);
        void feel_pain();

        //##########################

        /**
         * @brief find_path_to -- a* algorithm that searches path from the actor's position to position passed to function
         * @param pos -- position of the end of a path
         * @return path from current position to pos
         */
        std::vector<Coord> find_path_to(Coord pos);
        void remove_weap(Weapon* weapon);
        void die();

    public:
        //############# CONSTRUCTORS ###############
        Actor();
        Actor(const Actor& obj);

        virtual ~Actor();

        //############ DATABASE ###################
        static void read_races_txt();

        //############### GETTERS ##################
        std::string get_texture() const;
        Coord get_pos() const;

        //############# SETTERS ###################
        void set_pos(Coord pos);
        void set_map(Map* map);

        //############# GAME LOGIC #################
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
};

#endif // ACTOR_H
