#include "npc.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "data.h"

std::unordered_map<std::string, NPC> NPC::NPC_LIST;

std::vector<Tile::ptr> NPC::get_tiles_in_fov() const {
    std::vector<Tile::ptr> tiles;

    return  tiles;
}

std::vector<Actor::ptr> NPC::get_actors_in_fov() const {
    std::vector<Actor::ptr> actors;

    for (const auto& tile : get_tiles_in_fov()) {
        auto actor = tile->get_actor();
        if (actor)
            actors.push_back(actor);
    }

    return  actors;
}

NPC::NPC(const NPC& obj) : Actor(obj) {}

void NPC::read_npc_txt() {
//    std::ifstream file("npc.txt");
//    std::string line = "";

//    if (!file.is_open()) {
//        log_file << "Unable to open materials.txt" << std::endl;
//        std::cerr << "Unable to open materials.txt" << std::endl;
//        return;
//    }

//    NPC* npc = nullptr;
//    while (std::getline(file, line)) {
//        // comment line
//        if (line.front() == '#') continue;
//        // empty line
//        if (line.find_first_not_of(" \n\t\r") == std::string::npos)
//            continue;
//        // append to line until semicolon is readen
//        while (line.find(";") == std::string::npos) {
//            std::string buffer_line;
//            std::getline(file, buffer_line);
//            line += buffer_line;
//        }
//        line.erase(line.find(";"));
//        trimr_string(line);
//        //----------processing-line--------------
//        if (line == "NEW_NPC")
//            npc = new NPC();
//        else if (line == "PUSH_NPC") {
//            NPC_LIST.insert(std::pair<std::string, NPC>(npc->name, *npc));
//            log_file << "Added new NPC to NPC_LIST: " << npc->name << std::endl;
//            delete npc;
//        } else if (line.find("name") != std::string::npos)
//            npc->name = take_value(line);
//        else if (line.find("description") != std::string::npos)
//            npc->description = take_value(line);
//        else if (line.find("texture") != std::string::npos)
//            npc->texture = take_value(line);
//        else if (line.find("race") != std::string::npos)
//            npc->race = RACES_LIST.at(take_value(line));
//    }

//    file.close();
}

void NPC::make_turn() {

}

void NPC::pick_item() {

}

void NPC::attack(Coord target) {

}
