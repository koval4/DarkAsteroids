#include "npcprototype.h"

std::unordered_map<std::string, NPCPrototype> NPCPrototype::PROTOTYPES;

NPCPrototype::NPCPrototype() {}

void NPCPrototype::read_from_file(std::string filename) {
    std::ifstream file(filename);
    std::string line = "";

    if (!file.is_open()) {
        log_file << "Unable to open materials.txt" << std::endl;
        std::cerr << "Unable to open materials.txt" << std::endl;
        return;
    }

    NPCPrototype prototype;
    while (std::getline(file, line)) {
        // comment line
        if (line.front() == '#') continue;
        // empty line
        if (line.find_first_not_of(" \n\t\r") == std::string::npos)
            continue;
        // append to line until semicolon is readen
        while (line.find(";") == std::string::npos) {
            std::string buffer_line;
            std::getline(file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line);
        //----------processing-line--------------
        if (line == "NEW_NPC")
            prototype = {};
        else if (line == "PUSH_NPC") {
            PROTOTYPES.insert({prototype.name, prototype});
            log_file << "Added new NPCPrototype to NPC_LIST: " << prototype.get_name() << std::endl;
        } else prototype.parse_param_from_line(std::move(line));
    }
}

void NPCPrototype::parse_param_from_line(std::string&& line) {
        if (line.find("name") != std::string::npos)
            name = take_value(line);
        else if (line.find("description") != std::string::npos)
            description = take_value(line);
        else if (line.find("texture") != std::string::npos)
            texture = take_value(line);
        else if (line.find("race") != std::string::npos)
            race = Race::RACES_LIST.at(take_value(line));
}

std::string NPCPrototype::get_name() const {
    return name;
}


std::string NPCPrototype::get_description() const {
    return description;
}

std::string NPCPrototype::get_texture() const {
    return texture;
}

Race NPCPrototype::get_race() const {
    return race;
}
