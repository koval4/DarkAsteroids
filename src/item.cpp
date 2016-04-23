#include "item.h"
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include "data.h"

//################ STATIC VARIABLES ###################

std::unordered_map<std::string, Item> Item::ITEMS_LIST;
std::unordered_map<std::string, Item::Material> Item::MATERIALS;
std::unordered_map<std::string, Item> Item::AMMO_LIST;

//############## MATERIAL CONSTRUCTORS ###############

Item::Material::Material()
    : name("")
    , price(0)
    , density(0)
    , yield_strength(0)
    , fracture(0)
    , strain_at_stress(0) {}

Item::Material::Material(std::string name
                        , uint32_t price
                        , uint32_t density
                        , uint32_t yield_strength
                        , uint32_t fracture
                        , uint32_t strain_at_stress)
    : name(name)
    , price(price)
    , density(density)
    , yield_strength(yield_strength)
    , fracture(fracture)
    , strain_at_stress(strain_at_stress) {}

//#################### CONSTRUCTORS ##################

Item::Item()
    : name("")
    , description("")
    , weight(0)
    , price(0)
    , volume(0)
    , material()
    , type(misc) {}

Item::Item(const Item& obj)
    : name(obj.name)
    , description(obj.description)
    , weight(obj.weight)
    , price(obj.price)
    , volume(obj.volume)
    , material(obj.material)
    , type(obj.type)
    , item_slots(obj.item_slots) {
    calc_weight();
}

Item::ptr Item::make(std::string prototype) {
    Item::ptr item(new Item(ITEMS_LIST.at(prototype)));
    item->calc_weight();
    return item;
}

Item::ptr Item::clone() {
    return Item::ptr(new Item(*this));
}

//################## DESTRUCTOR ######################

Item::~Item() {}

//################### PRIVATE F-NS ################

void Item::calc_weight() {
    weight = ((float)material.density * volume) / 1000;
}

void Item::read_txt_properties(std::string& line) {
    //setting item params
    if (line.find("name") != std::string::npos) {
        name = take_value(line);
    } else if (line.find("description") != std::string::npos) {
        description = take_value(line);
    } else if (line.find("weight") != std::string::npos) {   //fetching weight
        weight = std::stof(take_value(line));
    } else if (line.find("price") != std::string::npos) {   //fetching price
        price = std::stoi(take_value(line));
    } else if (line.find("volume") != std::string::npos) {
        volume = std::stoi(take_value(line));
        calc_weight();
    } else if (line.find("material") != std::string::npos) {
        material = MATERIALS.at(take_value(line));
    } else if (line.find("slots") != std::string::npos) {
        item_slots = split_string(take_value(line), ',');
    } else if (line.find("type") != std::string::npos) {
        line = take_value(line);
        if (line == "misc")
            type = misc;
        else if (line == "weapon")
            type = weapon;
        else if (line == "armor")
            type = armor;
        else if (line == "consumable")
            type = consumable;
        else if (line == "container")
            type = container;
        else if (line == "wear_cont")
            type = wear_cont;
        else if (line == "corpse")
            type = corpse;
        else if (line == "bodypart")
            type = bodypart;
        else if (line == "money")
            type = money;
        else if (line == "ammo")
            type = ammo;
    }
}

//################# DATABASE CREATION ####################

void Item::read_item_txt() {
    std::ifstream item_file("items.txt");
    std::string line = "";

    if (!item_file.is_open()) {
        log_file << "Unable to open items.txt" << std::endl;
        std::cerr << "Unable to open items.txt" << std::endl;
        return;
    }

    Item itm;
    while (std::getline(item_file, line)) {
        if (line.front() == '#') continue;
        if (line.find_first_not_of(" \n\t\r") == std::string::npos) continue;   //if line is empty
        while (line.find(";") == std::string::npos){    //line ends with semicolon
            std::string buffer_line = "";
            std::getline(item_file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line); //erasing unnecessary from end
        if (line == "NEW_ITEM")
            itm = Item();
        else if (line == "PUSH_ITEM") {
            if (itm.type == ammo)
                AMMO_LIST.insert(std::pair<std::string, Item>(itm.name, itm));
            ITEMS_LIST.insert(std::pair<std::string, Item>(itm.name, itm));
            log_file << "Added new item to ITEMS_LIST: " << itm.name << std::endl;
        } else itm.read_txt_properties(line);
    }
    item_file.close();
}

void Item::read_materials_txt() {
    std::ifstream materials_file("materials.txt");
    std::string line = "";

    if (!materials_file.is_open()) {
        log_file << "Unable to open materials.txt" << std::endl;
        std::cerr << "Unable to open materials.txt" << std::endl;
        return;
    }

    Material mat;
    while (std::getline(materials_file, line)) {
        // comment line
        if (line.front() == '#') continue;
        // empty line
        if (line.find_first_not_of(" \n\t\r") == std::string::npos)
            continue;
        // append to line until semicolon is readen
        while (line.find(";") == std::string::npos) {
            std::string buffer_line;
            std::getline(materials_file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line);
        //----------processing-line--------------
        if (line == "NEW_MATERIAL")
            mat = {};
        else if (line == "PUSH_MATERIAL") {
            MATERIALS.insert(std::pair<std::string, Material>(mat.name, mat));
            log_file << "Added new material to MATERIALS: " << mat.name << std::endl;
        } else if (line.find("name") != std::string::npos) {
            line = take_value(line);
            mat.name = line;
        } else if (line.find("price") != std::string::npos) {
            line = take_value(line);
            mat.price = std::stoi(line);
        } else if (line.find("density") != std::string::npos) {
            line = take_value(line);
            mat.density = std::stoi(line);
        } else if (line.find("yield_strength") != std::string::npos) {
            line = take_value(line);
            mat.yield_strength = std::stoi(line);
        } else if (line.find("fracture") != std::string::npos) {
            line = take_value(line);
            mat.fracture = std::stoi(line);
        } else if (line.find("strain_at_stress") != std::string::npos) {
            line = take_value(line);
            mat.strain_at_stress = std::stoi(line);
        }
    }

    materials_file.close();
}

//################## GETTERS #########################

std::string Item::get_name() const {
    return name;
}

std::vector<std::string> Item::get_slots() const {
    return item_slots;
}

float Item::get_weight() const {
    return weight;
}

uint32_t Item::get_price() const {
    return price;
}

Item::Material Item::get_material() const {
    return material;
}

uint16_t Item::get_volume() const {
	return volume;
}

Item::Type Item::get_type() const {
	return type;
}

//################# SETTERS ##################

void Item::set_name(std::string name) {
    this->name = name;
}

void Item::set_description(std::string description) {
    this->description = description;
}

void Item::set_weight(int weight) {
    this->weight = weight;
}

void Item::set_price(int price) {
    this->price = price;
}

//#################### GAME LOGIC #################

void Item::get_deformation(uint32_t& stress, Item& item) {
// TODO (koval4#1#08/13/2015): add condition reduction

// small replacements to improve readability
#define YIELD material.yield_strength
#define FRACTURE material.fracture

    uint32_t strain = 0;
    double strain_cost = 1 + (((double)this->YIELD + this->FRACTURE) /
                              ((double)item.YIELD + item.FRACTURE) *
                              0.1);  // strain cost is a relation between plastic
    // deformation of this item to passed item
    stress /= strain_cost;  // stress is divided to strain cost
    #ifdef _DEBUG_
    std::cout << "Deformation of " << name << std::endl;
    std::cout << "this->YIELD : " << this->YIELD
              << " | this->FRACTURE : " << this->FRACTURE << std::endl;
    std::cout << "item.YIELD : " << item.YIELD
              << " | item.FRACTURE : " << item.FRACTURE << std::endl;
    std::cout << "stress : " << stress << std::endl;
    std::cout << "strain_cost : " << strain_cost << std::endl;
    std::cout << "stress : " << stress << std::endl;
    //std::cout << "condition before: " << condition << std::endl;
    #endif  // _DEBUG_

    if (stress > FRACTURE) {
        //condition = 0;
        this->material.name = "Tattered " + this->material.name;
        strain = (stress * stress) * this->get_material().strain_at_stress;
    } else if (stress < this->YIELD) {
        strain = stress * this->get_material().strain_at_stress;
        stress = 0;
        //uint16_t rvalue = rand() % 5;
        //if (condition > rvalue)
        //    condition -= rvalue;
        //else
        //    this->material.name = "Tattered " + this->material.name;
    } else {
        strain = (stress * stress) * this->get_material().strain_at_stress;
        stress =
            (((double)this->FRACTURE - this->YIELD) / 100) * (stress - this->YIELD);
        uint16_t cond_reduction =
            ((stress - this->YIELD) * 100) / ((double)this->FRACTURE - this->YIELD);
        //if (condition > cond_reduction)
        //    condition -= cond_reduction;
        //else
        //    this->material.name = "Tattered " + this->material.name;
    }
    #ifdef _DEBUG_
    //std::cout << "condition after: " << condition << std::endl;
    std::cout << "strain : " << strain << std::endl;
    #endif  // _DEBUG_
}

std::string Item::describe() {
    std::string line = name + "\n";
    line += description + "\n";
    line += "Weight: " + std::to_string(weight) + "\n";
    line += "Price: " + std::to_string(price) + "\n";
    return line;
}
