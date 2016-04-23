#include "bodypart.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "data.h"
#include "item.h"

//############## STATIC VARIABLES ###############

std::unordered_map<std::string, Bodypart> Bodypart::LIST;

//########### CONSTRUCTORS #################

Bodypart::Bodypart()
    : Item()
    , connection(nullptr)
    , items_weight(0)
    , max_weight(0)
    , grasp(false)
    , vital(false)
    , pain(0) {
    type = bodypart;
}

Bodypart::Bodypart(const Bodypart& obj)
    : Item(obj)
    , connection(obj.connection)
    , str_connection(obj.str_connection)
    , items_weight(0)
    , max_weight(obj.max_weight)
    , grasp(obj.grasp)
    , vital(obj.vital)
    , pain(obj.pain) {
    type = bodypart;
    for (auto& it : obj.inner_parts)
        inner_parts.push_back(Bodypart::ptr(new Bodypart(*it)));
    for (auto& it : obj.equipped_items)
        equipped_items.push_back(it->clone());
    for (auto& it : obj.grabbed_items)
        grabbed_items.push_back(it->clone());
}

Bodypart::Bodypart(std::string descr) : Bodypart() {
    size_t first_parnths = descr.find('(');

    // getting name of the bodypart
    name = descr.substr(0, descr.find_first_of(":("));
    triml_string(name);
    trimr_string(name);

    // if prototype is defined
    if (descr.find(':') < descr.find('(')) {
        // getting prototype name;
        std::string prototype_name = descr.substr(descr.find(':') + 1, first_parnths - descr.find(':') - 1);
        triml_string(prototype_name);
        trimr_string(prototype_name);
        Bodypart* prototype = &LIST.at(prototype_name);
        //copying prototype params;
        description = prototype->description;
        material = prototype->material;
        volume = prototype->volume;
        weight = prototype->weight;
        price = prototype->price;
        type = prototype->type;
        str_connection = prototype->str_connection;
        grasp = prototype->grasp;
        vital = prototype->vital;
    }

    // getting parameters of bodypart
    // working with data in parenthesis
    descr = descr.substr(first_parnths + 1, find_match_char(descr, first_parnths) - 1);
    for (auto& parameter : split_with_match_chars(descr)) {
        // making inner parts
        if (parameter.find("inner_parts") != std::string::npos) {
            parameter = take_value(parameter);
            size_t first_brasket = parameter.find('{');
            make_inner_parts(parameter.substr(first_brasket + 1, find_match_char(parameter, first_parnths) - 1));
        } else if (parameter.find("connection") != std::string::npos) {
            parameter = take_value(parameter);
            str_connection = parameter;
        } else if (parameter.find("description") != std::string::npos) {
            parameter = take_value(parameter);
            description = parameter;
        } else if (parameter.find("material") != std::string::npos) {
            parameter = take_value(parameter);
            material = MATERIALS.at(parameter);
        } else if (parameter.find("volume") != std::string::npos) {
            parameter = take_value(parameter);
            volume = std::stoi(parameter);
        } else if (parameter.find("price") != std::string::npos) {
            parameter = take_value(parameter);
            price = std::stoi(parameter);
        } else if (parameter.find("grasp") != std::string::npos) {
            parameter = take_value(parameter);
            if (parameter == "true")
                grasp = true;
            else grasp = false;
        } else if (parameter.find("vital") != std::string::npos) {
            parameter = take_value(parameter);
            if (parameter == "true")
                vital = true;
            else vital = false;
        }
    }
}

Item::ptr Bodypart::clone() {
    return Item::ptr(new Bodypart(*this));
}

void Bodypart::make_inner_parts(std::string line) {
    for (auto& it : split_with_match_chars(line)) {
        inner_parts.push_back(Bodypart::ptr(new Bodypart(it)));
    }
}

//############# DATABASE ###################

void Bodypart::read_bodyparts_txt() {
    std::ifstream file("bodyparts.txt");
    std::string line = "";

    if (!file.is_open()) {
        log_file << "Unable to open bodyparts.txt" << std::endl;
        std::cout << "Unable to open bodyparts.txt" << std::endl;
        return;
    }

    Bodypart* bpart = nullptr;

    while (std::getline(file, line)) {
        if (line.front() == '#') continue;
        if (line.find_first_not_of(" \n\t\r") == std::string::npos) continue;   //if line is empty
        while (line.find(";") == std::string::npos) {   //line ends with semicolon
            std::string buffer_line = "";
            std::getline(file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line); //erasing unnecessary from end
        if (line == "NEW_ITEM") {
            bpart = new Bodypart();
            //bpart->item_type = bodypart;
        } else if (line == "PUSH_ITEM") {
            LIST.insert(std::pair<std::string, Bodypart>(bpart->name, *bpart));
            log_file << "Added new item to BPART_LIST: " << bpart->name << std::endl;
            delete bpart;
            bpart = nullptr;
        } else if (line.find("inner_parts") != std::string::npos) {
            line = take_value(line);
            std::vector<std::string> str_inner_parts = split_string(line, ',');
            for (auto& it : str_inner_parts) {
                bpart->inner_parts.push_back(Bodypart::ptr(new Bodypart(LIST.at(it))));
            }
        } else if (line.find("connection") != std::string::npos) {
            line = take_value(line);
            bpart->str_connection = line;
        } else if (line.find("is_grasp") != std::string::npos) {
            line = take_value(line);
            if (line == "true")
                bpart->grasp = true;
            else bpart->grasp = false;
        } else if (line.find("is_vital") != std::string::npos) {
            line = take_value(line);
            if (line == "true")
                bpart->vital = true;
            else bpart->vital = false;
        } else bpart->read_txt_properties(line);
    }

    file.close();
}

//############# GETTERS #################

bool Bodypart::is_grasp() const {
    return grasp;
}

int Bodypart::get_pain() const {
    return pain;
}

int Bodypart::get_max_weight() const {
    return max_weight;
}

int Bodypart::get_items_weight() const {
    return items_weight;
}

std::vector<Item::ptr> Bodypart::get_items() {
    return equipped_items;
}

std::vector<Item::ptr> Bodypart::get_grabbed_items() {
    return grabbed_items;
}

//################ SETTERS ###############

void Bodypart::set_max_weight(int max_weight) {
    this->max_weight = max_weight;
}

void Bodypart::set_relative_size(int abs_volume, int percent) {
    if (percent > 100)
        percent = 100;
    else if (percent < 1)
        percent = 1;
    this->volume = (((float)abs_volume)/100) * percent;
    calc_weight();
}

void Bodypart::connect_to(Bodypart* connection) {
    this->connection = connection;
    connection->connected_parts.push_back(this);
}

//################ ITEMS OPERATIONS ################

void Bodypart::add_item(Item::ptr item) {
	std::cout << "Adding item " << item->get_name() << " | " << item << " to " << name << " | " << this << std::endl;
    //if ((items_weight +  item->get_weight()) <= max_weight)
    items_weight += item->get_weight();
	equipped_items.push_back(item);
}

void Bodypart::remove_item(Item::ptr item) {
    auto it = equipped_items.begin();
    while (it != equipped_items.end()) {
        if (*it == item) {
            items_weight -= item->get_weight();
            equipped_items.erase(it);
            return;
        }
        it++;
    }
}

void Bodypart::grab_item(Item::ptr item) {
    items_weight += item->get_weight();
	grabbed_items.push_back(item);
}

void Bodypart::drop_item(Item::ptr item) {
    auto it = grabbed_items.begin();
    while (it != grabbed_items.end()) {
        if (*it == item) {
            grabbed_items.erase(it);
            items_weight -= item->get_weight();
            return;
        }
        it++;
    }
}

//################ GAME LOGIC ##################

void Bodypart::get_deformation(uint32_t& stress, Item& item) {
// TODO (koval4#1#08/13/2015): add condition reduction

    //small replacements to improve readability
#define YIELD get_material().yield_strength
#define FRACTURE get_material().fracture

    uint32_t strain = 0;
    double strain_cost = 1 + ( ((double)this->YIELD + this->FRACTURE)/((double)item.YIELD + item.FRACTURE) * 0.1);    //strain cost is a relation between plastic deformation of this item to passed item
    stress /= strain_cost;  //stress is divided to strain cost
    #ifdef _DEBUG_
    std::cout << "Deformation of " << name << std::endl;
    std::cout << "this->YIELD : " << this->YIELD << " | this->FRACTURE : " << this->FRACTURE << std::endl;
    std::cout << "item.YIELD : " << item.YIELD << " | item.FRACTURE : " << item.FRACTURE << std::endl;
    std::cout << "stress : " << stress << std::endl;
    std::cout << "strain_cost : " << strain_cost << std::endl;
    std::cout << "stress : " << stress << std::endl;
    //std::cout << "condition before: " << condition << std::endl;
    #endif // _DEBUG_

    if (stress > FRACTURE) {
        //condition = 0;
        this->get_material().name = "Tattered " + this->get_material().name;
        strain = (stress * stress) * this->get_material().strain_at_stress;
    } else if (stress < this->YIELD) {
        strain = stress * this->get_material().strain_at_stress;
        stress = 0;
        //uint16_t rvalue = rand()%5;
        //if (condition > rvalue)
        //    condition -= rvalue;
        //else this->get_material().name = "Tattered " + this->get_material().name;
    } else {
        strain = (stress * stress) * this->get_material().strain_at_stress;
        stress = (((double)this->FRACTURE - this->YIELD)/100) * (stress - this->YIELD);
        uint16_t cond_reduction = ((stress - this->YIELD) * 100)/((double)this->FRACTURE - this->YIELD);
        //if (condition > cond_reduction)
        //    condition -= cond_reduction;
        //else this->get_material().name = "Tattered " + this->get_material().name;
    }
    pain += strain / this->get_material().strain_at_stress;
    #ifdef _DEBUG_
    //std::cout << "condition after: " << condition << std::endl;
    std::cout << "strain : " << strain << std::endl;
    std::cout << "pain : " << pain << std::endl;
    #endif // _DEBUG_
}

void Bodypart::get_hit(uint32_t& stress, Item& item) {
    for (int i = equipped_items.size() - 1; i >= 0; i--) {
        equipped_items[i]->get_deformation(stress, item);
        if (stress == 0)
            return;
    }
    get_deformation(stress, item);
}
