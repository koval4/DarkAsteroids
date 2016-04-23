#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "data.h"

/**
 * @brief The Item class
 */
class Item {
    public:
        typedef std::shared_ptr<Item> ptr;

        struct Material {
            std::string name;
            uint32_t price;
            uint32_t density;
            uint32_t yield_strength;
            uint32_t fracture;
            uint32_t strain_at_stress;

            Material();
			Material( std::string name
                    , uint32_t price
                    , uint32_t density
                    , uint32_t yield_strength
                    , uint32_t fracture
                    , uint32_t strain_at_stress
                    );
        };

		enum Type {
            misc,
            weapon,
            armor,
			consumable,
			container,
			wear_cont,
            corpse,
			bodypart,
            money,
            ammo
		};

        //############## DATABASES ################
        static std::unordered_map<std::string, Item> ITEMS_LIST;
        static std::unordered_map<std::string, Material> MATERIALS;
        static std::unordered_map<std::string, Item> AMMO_LIST;

    protected:
        std::string name;
        std::string description;
        float weight;
        uint32_t price;
        uint16_t volume;
        Material material;
		Type type;
        std::vector<std::string> item_slots;    /// slots in which item will be equipped

        /// calculates weight based on volume and material density
        void calc_weight();
    public:
        //################ CONSTRUCTORS ###############
        Item();
        Item(const Item& obj);  /// copy constructor

        //############## PTRS #################
        static Item::ptr make(std::string prototype);
        virtual Item::ptr clone();

        //############## DESTRUCTOR #################
        virtual ~Item();

        //########### DATABASES CREATION #############
        static void read_item_txt();
        static void read_materials_txt();
        /**
         * @brief read_txt_properties parses line for properties of this item
         * @param line -- ref to line that will be parsed
         */
        void read_txt_properties(std::string& line);

        //################# GETTERS ##################
        std::string get_name() const;
        std::vector<std::string> get_slots() const;
        float get_weight() const;
        uint32_t get_price() const;
        Material get_material() const;
        uint16_t get_volume() const;
		Type get_type() const;

        //################ SETTERS ####################
        void set_name(std::string name);
        void set_description(std::string description);
        void set_weight(int weight);
        void set_price(int price);

        //############# GAME LOGIC #################
        /**
         * @brief get_deformation
         * @param stress
         * @param item
         */
        virtual void get_deformation(uint32_t& stress, Item& item);
        virtual std::string describe();
};

#endif // ITEM_H
