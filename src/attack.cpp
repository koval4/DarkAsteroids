#include "attack.h"
#include <string>
#include "item.h"

Attack::Attack()
    : parent(nullptr)
    , name()
    , message()
    , ap_cost(0)
    , contact_area(0)
    , start_force(0)
    , range(0)
    , thermal_energy(0) {}

Attack::Attack(const Attack& obj)
    : parent(obj.parent)
    , name(obj.name)
    , message(obj.message)
    , ap_cost(obj.ap_cost)
    , contact_area(obj.contact_area)
    , start_force(obj.start_force)
    , range(obj.range)
    , thermal_energy(obj.thermal_energy) {}

Attack::Attack(Weapon* parent, std::string line)
    : parent(parent) {
    for (auto& it : split_with_match_chars(line)) {
        if (it.find("name") != std::string::npos) {
            it = take_value(it);
            name = it;
        } else if (it.find("message") != std::string::npos) {
            it = take_value(it);
            message = it;
        } else if (it.find("ap_cost") != std::string::npos) {
            ap_cost = std::stoi(take_value(it));
        } else if (it.find("contact_area") != std::string::npos) {
            it = take_value(it);
            contact_area = std::stoi(it);
        } else if (it.find("start_force") != std::string::npos) {
            it = take_value(it);
            start_force = std::stoi(it);
        } else if (it.find("range") != std::string::npos) {
            it = take_value(it);
            range = std::stoi(it);
        } else if (it.find("thermal_energy") != std::string::npos) {
            it = take_value(it);
            thermal_energy = std::stoi(it);
        }
    }
}

std::string Attack::get_name() const {
    return name;
}

std::string Attack::get_message() const {
    return message;
}

uint8_t Attack::get_cost() const {
    return ap_cost;
}

uint32_t Attack::get_contact_area() const {
    return contact_area;
}

uint32_t Attack::get_start_force() const {
    return start_force;
}

uint8_t Attack::get_range() const {
    return range;
}

uint32_t Attack::get_thermal_energy() const {
    return thermal_energy;
}

void Attack::set_parent(Weapon* parent) {
    this->parent = parent;
}
