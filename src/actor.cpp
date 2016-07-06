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
#include "inventory.h"

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
    , name()
    , texture()
    , description()
    , level(0)
    , experience(0)
    , params()
    , skills()
    , race()
    , body()
    , body_size(0)
    , map(nullptr)
    , inventory() {}

Actor::Actor(
        std::string texture,
        std::string name,
        std::string description,
        Params params,
        Skills skills,
        Actor::Race race,
        uint16_t level,
        uint16_t experience)
    : pos()
    , name(name)
    , texture(texture)
    , description(description)
    , level(level)
    , experience(experience)
    , params(params)
    , skills(skills)
    , race(race)
    , body()
    , body_size(0)
    , inventory()
    , map(nullptr) {
    make_body();
}

Actor::Actor(const Actor& obj)
    : pos(obj.pos)
    , name(obj.name)
    , texture(obj.texture)
    , description(obj.description)
    , level(obj.level)
    , experience(obj.experience)
    , params(obj.params)
    , skills(obj.skills)
    , race(obj.race)
    , body_size(obj.body_size)
    , inventory(obj.inventory)
    , map(obj.map) {
    make_body();
}

Actor::~Actor() {}

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
    inventory = std::make_shared<Inventory>(body);
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

//############### GETTERS ################

std::string Actor::get_texture() const {
    return texture;
}

Coord Actor::get_pos() const {
    return pos;
}

Actor::Params Actor::get_params() const {
    return params;
}

const Map*Actor::get_map() const {
    return map;
}

const std::shared_ptr<Tile> Actor::get_tile() const {
    return map->at(pos);
}

const std::shared_ptr<Inventory> Actor::get_inventory() const {
    return inventory;
}

//################## SETTERS ################

void Actor::set_pos(Coord pos) {
    this->pos = pos;
}

void Actor::set_map(const Map* map) {
    this->map = map;
}

bool Actor::can_make_turn() const {
    return params.action_points > 0;
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

    if (map)
        inventory->equip_item(item, map->at(pos));
    else inventory->equip_item(item, nullptr);
}

void Actor::grab_item(Item::ptr item) {
    if (map)
        inventory->grab_item(item, map->at(pos));
    else inventory->grab_item(item, nullptr);
}

void Actor::drop_item(Item::ptr item) {
    //removing specific properties
    params.curr_weight -= item->get_weight();
    if (map)
        inventory->drop_item(item, map->at(pos));
    else inventory->drop_item(item, nullptr);
    log_file << name << " has dropped " << item->get_name() << "." << std::endl;
}

bool Actor::is_alive() const {
    return params.curr_pain < params.pain_threshold;
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
    if (map)
        inventory->drop_all(map->at(pos));
    else inventory->drop_all(nullptr);
}
