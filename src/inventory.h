#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "data.h"
#include "item.h"
#include "bodypart.h"
#include "container.h"
#include "weapon.h"
#include "actor.h"
#include "tile.h"

class Tile;

class Inventory {
    public:
        using ptr = std::shared_ptr<Inventory>;

    private:
        std::unordered_map<std::string, Bodypart*> equipment;   /// actor equips Item in slot(string type) , so map makes some slots and fills it with items
        std::unordered_map<std::string, Bodypart*> grasps;      /// here actor holds weapon, items etc.
        std::vector<Weapon::ptr> weapons;                           /// currently equipped weapons
        std::vector<Container::ptr> containers;                     /// currently equipped containers

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
        void remove_weap(const std::shared_ptr<Weapon> weapon);

    public:
        Inventory(std::unordered_map<std::string, Bodypart::ptr> body);

        const Item::ptr get_item_by_name(std::string slot, std::string name) const;
        PAIRS<std::string, std::string> list_items() const;

        /**
         * @brief equip_item -- adds item to inventory
         * @param item       -- item that will be added to inventory
         */
        void equip_item(Item::ptr item, const std::shared_ptr<Tile> tile);
        /**
         * @brief grab_item -- adds item to grasps
         * @param item      -- item that will be added to grasps
         */
        void grab_item(Item::ptr item, const std::shared_ptr<Tile> tile);
        /**
         * @brief drop_item -- removes item from inventory
         * @param item      -- item that will be removed from inventory
         */
        void drop_item(Item::ptr item, const std::shared_ptr<Tile> tile);
        void drop_all(const std::shared_ptr<Tile> tile);
};

#endif // INVENTORY_H
