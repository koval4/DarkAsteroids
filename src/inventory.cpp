#include "inventory.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "data.h"
#include "tile.h"
#include "item.h"
#include "bodypart.h"
#include "container.h"
#include "weapon.h"
#include "actor.h"

Inventory::Inventory(const Body& body) {
    for (auto& it : body.get_parts()) {  //iterating through body
        if (it.second->is_grasp())  //if grasp => adding pair to grasps map
            grasps.insert(std::pair<std::string, Bodypart*>(it.first, it.second.get()));
        else equipment.insert(std::pair<std::string, Bodypart*>(it.first, it.second.get()));  //else adding pair to equipment map
    }
}

PAIRS<std::string, Bodypart*> Inventory::find_slots(std::string slot) {
    PAIRS<std::string, Bodypart*> slots;
    if (slot == "Grasp") {  //if item graspable => return grasps map
        for (auto& it : grasps)
            slots.push_back(it);
    } else {
        for (auto& it : equipment) {
            if (it.first.find(slot) != std::string::npos) {   //if slot is found
                slots.push_back(it);
            }
        }
    }
    return slots;
}

Bodypart* Inventory::find_less_loaded_bpart(PAIRS<std::string, Bodypart*> slots) {
    if (slots.empty())
        return nullptr;
    Bodypart* less_loaded = slots[0].second;
    int min_items = slots[0].second->get_items_weight();
    for (auto& it : slots) {
        if (min_items == 0)
            break;
        if (it.second->get_items_weight() < min_items) {
            min_items = it.second->get_items_weight();
            less_loaded = it.second;
        }
    }
    return less_loaded;
}

void Inventory::remove_weap(const std::shared_ptr<Weapon> weapon) {
    auto it = weapons.begin();
    while (it != weapons.end()) {
        if (*it == weapon)
            weapons.erase(it);
        else it++;
    }
}

const Item::ptr Inventory::get_item_by_name(std::string slot, std::string name) const {
    if (equipment.find(slot) != equipment.end()) {
        for (auto& item : equipment.at(slot)->get_items()) {
            if (item->get_name() == name)
                return item;
        }
    } else if (grasps.find(slot) != grasps.end()) {
        for (auto& item : grasps.at(slot)->get_grabbed_items()) {
            if (item->get_name() == name)
                return item;
        }
    }
    return nullptr;
}

PAIRS<std::string, std::string> Inventory::list_items() const {
    PAIRS<std::string, std::string> items;
    for (const auto& slot : equipment)
        for (const auto& item : slot.second->get_items())
            items.push_back({slot.first, item->get_name()});
    for (const auto& slot : grasps)
        for (const auto& item : slot.second->get_grabbed_items())
            items.push_back({slot.first, item->get_name()});
    for (const auto& container : containers)
        for (const auto& item : container->open_container())
            items.push_back({container->get_name(), item->get_name()});
    return items;
}

void Inventory::equip_item(Item::ptr item, const std::shared_ptr<Tile> tile) {
    //if item is grabbable
    if ((item->get_type() != Item::armor)
            && (item->get_type() != Item::wear_cont) ) {
        if (grasps.empty())
            return;
        grab_item(std::move(item), tile);
        return;// "success";
    }

    //searching for slots to equip item
    std::vector<std::string> needed_slots = item->get_slots();   //rquired slots
    std::vector<Bodypart*> found_slots; //vector with found bodyparts, found_slots.size() must be equal to needed_slots.size()
    for (auto& it : needed_slots) { //searching appropriate bodypart for each slot
        PAIRS<std::string, Bodypart*> slots = find_slots(it);    //getting vector with appropriate bodyparts
        if (slots.empty() && tile) {  //if none appropriate slot => return
            tile->put_item(std::move(item));
            return;// "no_slot";
        }
        Bodypart* bpart_slot = find_less_loaded_bpart(slots);
        found_slots.push_back(bpart_slot);
    }

    for (auto& it : found_slots)
        it->add_item(item);

    if (item->get_type() == Item::wear_cont) {
        containers.push_back(std::dynamic_pointer_cast<Container>(item));
    }
}

void Inventory::grab_item(Item::ptr item, const Tile::ptr tile) {
    if (!item)
        return;
    const Weapon::ptr weap = std::dynamic_pointer_cast<Weapon>(item);

    if (weap) {
        if ((weap->get_hold_type() == Weapon::two_handed) && (grasps.size() < 2))
            return;
    }
    //grabbing with 1 grasp
    bool is_equipped = false;
    PAIRS<std::string, Bodypart*> slots = find_slots("Grasp");
    Bodypart* bpart_slot = find_less_loaded_bpart(slots);

    if (bpart_slot != nullptr) {
        bpart_slot->grab_item(item);
        is_equipped = true;
    } else return;

    //if no free grasp => first used
    if (!is_equipped) {
        auto it = grasps.begin();
        for (auto& it_item : it->second->get_grabbed_items())
            drop_item(Item::ptr(it_item), tile);
        it->second->grab_item(item);
    }

    //if needed => use 2nd grasp
    if (    (item->get_type() == Item::weapon)
            && (weap->get_hold_type() == Weapon::two_handed)) {
        is_equipped = false;
        auto it = ++grasps.begin();
        it->second->grab_item(item);
    }
    if (item->get_type() == Item::weapon) {
        weapons.push_back(weap);
    }
}

void Inventory::drop_item(Item::ptr item, const Tile::ptr tile) {
    //removing specific properties
    if (item->get_type() == Item::weapon)
        remove_weap(std::dynamic_pointer_cast<Weapon>(item));
    else if (item->get_type() == Item::wear_cont) {
        auto it = containers.begin();
        while (item != *it)
            it++;
        containers.erase(it);
    }
    //placing null at inventory slots
    for (auto& it : item->get_slots()) {
        auto it_object = equipment.find(it);
        if (it_object != equipment.end()) {
            equipment.at(it_object->first)->remove_item(item);
        } else {
            bool dropped = false;
            for (auto& iter : grasps) {
                for (auto& it_items : iter.second->get_grabbed_items()) {
                    if (it_items == item) {
                        iter.second->drop_item(item);
                        dropped = true;
                        break;
                    }
                }
                if (dropped)
                    break;
            }
            if (!dropped) { //if item wasn't found in grasps map => searching in containers
                for (auto& contnr : containers) {
                    for (auto& item_it : contnr->open_container()) { //searching item in contnr_it content
                        if (item_it == item.get()) {
                            contnr->remove_item(item_it);
                            dropped = true;
                            break;
                        }
                    }
                    if (dropped)
                        break;
                }
            }
        }
    }
//    log_file << name << " has dropped " << item->get_name() << "." << std::endl;
    if (tile)
        tile->put_item(std::move(item));
}

void Inventory::drop_all(const Tile::ptr tile) {
    // possible bug here, item could be copied
    for (auto& bpart : equipment) {
        for (auto& item : bpart.second->get_items()) {
            tile->put_item(Item::ptr(new Item(*item)));
        }
    }
    for (auto& bpart : grasps) {
        for (auto& item : bpart.second->get_grabbed_items()) {
            tile->put_item(Item::ptr(new Item(*item)));
        }
    }
}
