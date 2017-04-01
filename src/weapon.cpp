#include "weapon.h"
#include <cmath>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include "item.h"
#include "actor.h"
#include "map.h"
#include "attack.h"
#include "rangedattack.h"
#include "closeattack.h"
#include "common.h"

std::map<std::string, Weapon> Weapon::LIST;

Weapon::Ammo::Ammo()
    : type()
    , ammo()
    , max_ammo(0)
    , ammo_left(0) {}

Weapon::Ammo::Ammo(const Weapon::Ammo& obj)
    : type(obj.type)
    , max_ammo(obj.max_ammo)
    , ammo_left(obj.ammo_left) {
    if (obj.ammo)
        ammo = obj.ammo->clone();
    else ammo = Item::ptr(nullptr);
}

Weapon::Ammo::Ammo(std::string line) : Ammo() {
    for (auto& it : split_with_match_chars(line)) {
        if (it.find("type") != std::string::npos) {
            type = take_value(it);
        } else if (it.find("max_ammo") != std::string::npos) {
            max_ammo = std::stoi(take_value(it));
        } else if (it.find("ammo_left") != std::string::npos) {
            ammo_left = std::stoi(take_value(it));
        }
    }
}

Weapon::Weapon()
    : Item()
    , hold_type(one_handed) {
    type = weapon;
}

Weapon::Weapon(const Weapon& obj)
    : Item(obj)
    , ammo(obj.ammo)
    , hold_type(obj.hold_type)
    , available_attacks(obj.available_attacks) {
    for (auto& attack : available_attacks) {
        attack->set_parent(this);
    }
}

Item::ptr Weapon::make(std::string prototype) {
    Item::ptr item(new Weapon(LIST.at(prototype)));
    // making weapon fully loaded
    for (auto& ammo : AMMO_LIST) {
        for (auto& type : dynamic_cast<Weapon*>(item.get())->ammo) {
            if (ammo.first.find(type.first) != std::string::npos) {
                type.second.ammo_left = type.second.max_ammo;
                type.second.ammo = Item::make(ammo.first);
            }
        }
    }
    return item;
}

Item::ptr Weapon::clone() {
    return Item::ptr(new Weapon(*this));
}

void Weapon::read_weapons_txt() {
    std::ifstream weapons_file("weapons.txt");
    std::string line;

    if (!weapons_file.is_open()) {
        log_file << "Unable to open weapons.txt" << std::endl;
        std::cerr << "Unable to open weapons.txt" << std::endl;
        return;
    }

    Weapon* weap;
    while (std::getline(weapons_file, line)) {
        if (line.front() == '#') continue;
        if (line.find_first_not_of(" \n\t\r") == std::string::npos) continue;   //if line is empty
        while (line.find(";") == std::string::npos) {   //line ends with semicolon
            std::string buffer_line = "";
            std::getline(weapons_file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line); //erasing unnecessary from end
        if (line == "NEW_ITEM")
            weap = new Weapon();
        else if (line == "PUSH_ITEM") {
            LIST.insert(std::pair<std::string, Weapon>(weap->name, *weap));
            log_file << "Added new weapon to WEAPONS_LIST: " << weap->name << std::endl;
            delete weap;
            weap = nullptr;
        } else if (line.find("available_attacks") != std::string::npos) {
            line = take_value(line);
            for (auto& it : split_with_match_chars(line)) {
                it = it.substr(it.find('{') + 1, it.find('}') - 1);
                Attack::ptr attack;
                if (it.find("Ranged ::") != std::string::npos) {
                    it = it.substr(it.find("::") + 2);
                    attack = RangedAttack::ptr(new RangedAttack(weap, it));
                    weap->available_attacks.push_back(attack);
                } else if (it.find("Close ::") != std::string::npos) {
                    it = it.substr(it.find("::") + 2);
                    attack = CloseAttack::ptr(new CloseAttack(weap, it));
                    weap->available_attacks.push_back(attack);
                }
            }
        } else if (line.find("ammo") != std::string::npos) {
            for (auto& it : split_with_match_chars(take_value(line))) {
                Ammo new_ammo = {it.substr(it.find("{") + 1, it.find("}") - 1)};
                weap->ammo.insert({new_ammo.type, new_ammo});
            }
        } else if (line.find("hold_type") != std::string::npos) {
            line = take_value(line);
            if (line == "one_handed")
                weap->hold_type = one_handed;
            else if (line == "two_handed")
                weap->hold_type = two_handed;
        } else weap->read_txt_properties(line);
    }
    weapons_file.close();
}

bool Weapon::check_ammo(Attack::ptr attack) const {
    std::cout << "Checking ammo for " << attack->get_name() << std::endl;
    RangedAttack* ranged_attack = dynamic_cast<RangedAttack*>(attack.get());
    if (ranged_attack != nullptr) {
        std::cout << attack->get_name() << " is ranged attack" << std::endl;
        if (ammo.at(ranged_attack->get_ammo_type()).ammo_left >= ranged_attack->get_ammo_usage())
            return true;
        else return false;
    } else {
        return true;
    }
}

Weapon::Hold_type Weapon::get_hold_type() const {
    return hold_type;
}

std::map<std::string, Weapon::Ammo> Weapon::get_ammo_info() const {
    return ammo;
}

std::vector<Attack::ptr> Weapon::get_available_attacks() const {
    std::vector<Attack::ptr> attacks;
    for (auto& it : available_attacks) {
        if (check_ammo(it))
            attacks.push_back(it);
    }
    return attacks;
}

void Weapon::make_attack( Actor& user
                        , Attack::ptr attack_type
                        , Coord start
                        , Coord end
                        , Actor* victim
                        , Bodypart* target
                        ) {
    std::cout << "Performing attack: " << attack_type->get_name() << std::endl;
    std::cout << attack_type->get_message() << std::endl;
    if (!check_ammo(attack_type))
        return;
    RangedAttack* ranged = dynamic_cast<RangedAttack*>(attack_type.get());
    if (ranged != nullptr) {
        std::cout << attack_type->get_name() << " is ranged." << std::endl;
        ammo.at(ranged->get_ammo_type()).ammo_left -= ranged->get_ammo_usage();
    }
    attack_type->make_attack(user, start, end, victim, target);
}

void Weapon::reload(std::string ammo_type, Stack& ammo_stack) {
    size_t load_ammo = ammo.at(ammo_type).max_ammo - ammo.at(ammo_type).ammo_left;
    if (ammo_stack.get_size() < load_ammo)
        load_ammo = ammo_stack.get_size();
    for (size_t i = 0; i < load_ammo; i++) {
        ammo.at(ammo_type).ammo = ammo_stack.take();
        ammo.at(ammo_type).ammo_left++;
    }
}

std::string Weapon::describe() {
    std::string line = Item::describe();

    if (hold_type == two_handed)
        line += "Two-handed weapon\n";
    else if (hold_type == one_handed)
        line += "One-handed weapon\n";

    // describing ammo in weapon
    line += "Ammo: \n";
    for (auto& it : this->ammo) {
        line +=   it.first
                  + ": " + std::to_string(it.second.ammo_left)
                  + "/" + std::to_string(it.second.max_ammo)
                  + "\n";
    }

    return line;
}

