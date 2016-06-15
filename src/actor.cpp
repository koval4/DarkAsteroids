#include "actor.h"
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <fstream>
#include "map.h"
#include "data.h"
#include "weapon.h"

//############## STATIC VARIABLES ##############

std::unordered_map<std::string, Actor::Race> Actor::RACES_LIST;

//################## PARAMS ################

Actor::Params::Params() {}

Actor::Params::Params( uint16_t strength
                       , uint16_t perception
                       , uint16_t agility
                       , uint16_t speed
                       , uint16_t endurance)
    : strength(strength)
    , perception(perception)
    , agility(agility)
    , speed(speed)
    , endurance(endurance) {}

//################## RACE ###################

Actor::Race::Race() {}

Actor::Race::Race(const Actor::Race& obj)
    : name(obj.name)
    , max_body_size(obj.max_body_size)
    , min_body_size(obj.min_body_size) {
    for (auto& it : obj.body)
        body.insert(it);
}

//################# CONSTRUCTORS ##################

Actor::Actor()
    : pos()
    , texture()
    , name()
    , description()
    , level(0)
    , experience(0)
    , params()
    , skills()
    , body()
    , body_size(0)
    , map(nullptr) {}

Actor::Actor(const Actor& obj)
    : pos(obj.pos)
    , texture(obj.texture)
    , name(obj.name)
    , description(obj.description)
    , level(obj.level)
    , experience(obj.experience)
    , params(obj.params)
    , skills(obj.skills)
    , race(obj.race)
    , body_size(obj.body_size)
    , map(obj.map) {
    make_body();
    make_slots();
}

Actor::~Actor() {

}

void Actor::read_races_txt() {
    std::ifstream file("races_list.txt");
    std::string line;

    if (!file.is_open()) {
        std::cout << "Unable to open race-list.txt!" << std::endl;
        log_file << "Unable to open race-list.txt!" << std::endl;
        return;
    }

    Race temp_race;
    while (std::getline(file, line)) {
        if (line.front() == '#') continue;  //comment line
        if (line.find_first_not_of(" \n\t\r") == std::string::npos) continue;   //if line is empty
        while (line.find(";") == std::string::npos) {   //line ends with semicolon
            std::string buffer_line = "";
            std::getline(file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line); //erasing unnecessary from end
        if (line == "NEW_RACE") {
            temp_race = {};
        } else if (line == "PUSH_RACE") {
            RACES_LIST.insert(std::pair<std::string, Race>(temp_race.name, Race(temp_race)));
            log_file << "Added new item to RACES_LIST: " << temp_race.name << std::endl;
            temp_race = {};
        } else if (line.find("name") != std::string::npos) {
            line.erase(0, line.find("=") + 1);
            triml_string(line);
            temp_race.name = line;
        } else if (line.find("min_body_size") != std::string::npos) {
            line.erase(0, line.find("=") + 1);
            triml_string(line);
            temp_race.min_body_size = std::stoi(line);
        } else if (line.find("max_body_size") != std::string::npos) {
            line.erase(0, line.find("=") + 1);
            triml_string(line);
            temp_race.max_body_size = std::stoi(line);
        } else if (line.find("body") != std::string::npos) {
            line.erase(0, line.find("=") + 1);
            triml_string(line);
            for (auto& it : split_with_match_chars(line)) {
                Bodypart bpart(it);
                temp_race.body.insert(std::pair<std::string, Bodypart>(bpart.get_name(), bpart));
            }
        }
    }

    file.close();
}

//############ PRIVATE F-NS ##############

void Actor::rand_params() {
    params.strength = rand()%3 + 5;
    params.perception = rand()%3 + 5;
    params.agility = rand()%3 + 5;
    params.speed = rand()%3 + 5;
    params.endurance = rand()%3 + 5;
}

void Actor::calc_secondary_params() {
    params.max_ap = (2*params.speed + params.agility)/2;
    params.max_weight = 3*params.strength + 2*params.endurance;
    params.visibility_range = params.perception;
    params.pain_threshold = 1000 * params.endurance;
}

void Actor::calc_skills() {
    skills.close_combat = 3*params.strength + 2*params.endurance + 2*params.speed;
}

void Actor::make_body() {
    #ifdef _DEBUG_
    std::cout << "make_body() called by " << name << " (" << this << " )  :" << std::endl;
    #endif // _DEBUG_
    body_size = race.min_body_size + rand()%(race.max_body_size - race.min_body_size + 1);
    #ifdef _DEBUG_
    std::cout << "body_size = " << body_size << std::endl;
    #endif
    for (auto& it : race.body) {
        body.insert(std::pair<std::string, Bodypart::ptr>(it.first, Bodypart::ptr(new Bodypart(it.second))));
        body.at(it.first)->set_max_weight(params.max_weight / (body_size / body.at(it.first)->get_volume()));
    }
}

void Actor::make_slots() {
    for (auto& it : body) {  //iterating through body
        if (it.second->is_grasp())  //if grasp => adding pair to grasps map
            grasps.insert(std::pair<std::string, Bodypart*>(it.first, it.second.get()));
        else equipment.insert(std::pair<std::string, Bodypart*>(it.first, it.second.get()));  //else adding pair to equipment map
    }
}

PAIRS<std::string, Bodypart*> Actor::find_slots(std::string slot) {
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

Bodypart* Actor::find_less_loaded_bpart(PAIRS<std::string, Bodypart*> slots) {
    if (slots.size() == 0)
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

void Actor::feel_pain() {
    for (auto& bodypart : body)
        params.curr_pain += bodypart.second->get_pain();
}

std::vector<Coord> Actor::find_path_to(Coord pos) {
    typedef std::pair<Coord, int> Node;
    struct Predicate {
        bool operator () (const Node& node1, const Node& node2) {
            return (node1.second > node2.second);
        }
    };
    std::priority_queue<Node, std::vector<Node>, Predicate> open_nodes;
    open_nodes.emplace(this->pos, 0);
    std::unordered_map<Coord, int> g_costs;
    g_costs[this->pos] = 0;
    std::unordered_map<Coord, Coord> came_from;
    came_from[this->pos] = this->pos;

    while (!open_nodes.empty()) {
        Coord curr_node = open_nodes.top().first;
        open_nodes.pop();
        if (curr_node == pos)
            break;

        for (auto& tile : map->at(curr_node)->get_adjacent()) {
            if (!tile->is_passable())
                continue;
            // calculating cost for next node
            Coord next_node = tile->get_pos();
            int next_cost = g_costs[curr_node] + 1;
            if (g_costs.count(next_node) == 0 || next_cost < g_costs[next_node]) {
                g_costs[next_node] = next_cost;
                int heuristics = 1*(abs(next_node.x - pos.x) + abs(next_node.y - pos.y));
                int priority = next_cost + heuristics;
                came_from[next_node] = curr_node;
                open_nodes.emplace(next_node, priority);
            }
        }
    }

    std::vector<Coord> path = {pos};
    Coord prev_pos = came_from[pos];
    do {
        path.insert(path.begin(), prev_pos);
        prev_pos = came_from[prev_pos];
    } while (prev_pos != this->pos);
    return path;
}

void Actor::remove_weap(Weapon* weapon) {
    auto it = weapons.begin();
    while (*it != weapon)
        it++;
    weapons.erase(it);
}

//############### GETTERS ################

std::string Actor::get_texture() const {
    return texture;
}

Coord Actor::get_pos() const {
    return pos;
}

//################## SETTERS ################

void Actor::set_pos(Coord pos) {
    this->pos = pos;
}

void Actor::set_map(const Map* map) {
    this->map = map;
}

//################# GAME LOGIC ###############

void Actor::move_to(Coord pos) {
    if (!map->at(pos)->is_passable())
        return;
    uint16_t dx = abs(pos.x - this->pos.x);
    uint16_t dy = abs(pos.y - this->pos.y);
    if (dx <= 1 && dy <= 1) {
        if (dx != dy && params.action_points > 0) {
            params.action_points--;
            map->at(pos)->place_actor(map->at(this->pos)->remove_actor());
            this->pos = pos;
        } else if (params.action_points > 1) {
            params.action_points -= 2;
            map->at(pos)->place_actor(map->at(this->pos)->remove_actor());
            this->pos = pos;
        } else {
            return;
        }
    } else {
        std::vector<Coord> path = find_path_to(pos);
        if (path.size() > params.action_points)
            return;
        else {
            map->at(pos)->place_actor(map->at(this->pos)->remove_actor());
            this->pos = pos;
            params.action_points -= path.size();
        }
    }
}

void Actor::equip_item(Item::ptr item) {
    log_file << name << " equips " << item->get_name() << std::endl;
    if (!item) //if item doesn't exist => return, nothing changed
        return;// "null_item";

    if ( ( item->get_weight() + params.curr_weight ) <= params.max_weight )
        params.curr_weight += item->get_weight();
    else {
        log_file << name << " are overloaded. Droping item" << std::endl;
        if (map != nullptr)
            map->at(pos)->put_item(std::move(item));
        return;// "too_heavy";
    }

    if ( params.action_points >= 2 )
        params.action_points -= 2;
    else {
        log_file << name << " does not have enough action points to pick up items." << std::endl;
        if (map != nullptr)
            map->at(pos)->put_item(std::move(item));
        return;// "lack_ap";
    }

    //if item is grabbable
    if ((item->get_type() != Item::armor)
            && (item->get_type() != Item::wear_cont) ) {
        if (grasps.empty())
            return;
        grab_item(std::move(item));
        return;// "success";
    }

    //searching for slots to equip item
    std::vector<std::string> needed_slots = item->get_slots();   //rquired slots
    std::vector<Bodypart*> found_slots; //vector with found bodyparts, found_slots.size() must be equal to needed_slots.size()
    for (auto& it : needed_slots) { //searching appropriate bodypart for each slot
        PAIRS<std::string, Bodypart*> slots = find_slots(it);    //getting vector with appropriate bodyparts
        if (slots.size() == 0) {  //if none appropriate slot => return
            map->at(pos)->put_item(std::move(item));
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

    return;// "success";
}

void Actor::grab_item(Item::ptr item) {
    if (!item)
        return;
    Weapon* weap = nullptr;

    if (item->get_type() == Item::weapon) {
        weap = dynamic_cast<Weapon*>(item.get());
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
            drop_item(Item::ptr(it_item));
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

void Actor::drop_item(Item::ptr item) {
    //removing specific properties
    params.curr_weight -= item->get_weight();
    if (item->get_type() == Item::weapon)
        remove_weap(dynamic_cast<Weapon*>(item.get()));
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
    log_file << name << " has dropped " << item->get_name() << "." << std::endl;
    map->at(pos)->put_item(std::move(item));
}

void Actor::get_wound( int32_t& momentum
                       , Item& item
                       , uint32_t contact_area
                       , Bodypart* target
                     ) {
    // if target nullptr => choose random bodypart
    if (target == nullptr) {
        auto it = body.begin();
        for (size_t i = 0; i < rand()%body.size(); i++)
            it++;
        target = it->second.get();
    }

    float mass1 = item.get_weight();
    float mass2 = target->get_weight();
    // v1' = (m1 - m2)/(m1 + m2)  * v1      <= velocity of weapon after collision
    int32_t vel1 = (((float)mass1 - mass2)/((float)mass1 + mass2))*(momentum/mass1);
    momentum -= mass1 * vel1;    // changing momentum of item
    // v2' = (2*m1)/(m1 + m2)  * v1         <= velocity of bodypart after collision
    int32_t vel2 = (((float)2 * mass1)/((float)mass1 + mass2))*(momentum/mass1);
    #ifdef _DEBUG_
    std::cout << "vel1 : " << vel1 << std::endl;
    std::cout << "vel2 : " << vel2 << std::endl;
    #endif // _DEBUG_

    uint32_t bpart_stress = ((float) get_force(mass2 * vel2)  * 100)/contact_area;
    #ifdef _DEBUG_
    std::cout << "contact_area : " << contact_area << std::endl;
    std::cout << "bpart_stress : " << bpart_stress << std::endl;
    #endif // _DEBUG_
    target->get_hit(bpart_stress, item);
    feel_pain();
}

void Actor::die() {
    // possible bug here, item could be copied
    for (auto& bpart : equipment) {
        for (auto& item : bpart.second->get_items()) {
            Map::curr_map->at(pos)->put_item(Item::ptr(new Item(*item)));
        }
    }
    for (auto& bpart : grasps) {
        for (auto& item : bpart.second->get_grabbed_items()) {
            Map::curr_map->at(pos)->put_item(Item::ptr(new Item(*item)));
        }
    }
}
