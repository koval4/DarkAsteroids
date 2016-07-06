#include "race.h"
#include <string>
#include <unordered_map>
#include "bodypart.h"

std::unordered_map<std::string, Race> Race::RACES_LIST;

Race::Race()
    : name()
    , bodymap()
    , max_body_size(0)
    , min_body_size(0) {}

Race::Race(const Race& other)
    : name(other.name)
    , bodymap(other.bodymap)
    , max_body_size(other.max_body_size)
    , min_body_size(other.min_body_size) {}

void Race::read_races_txt() {
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
            RACES_LIST.insert({temp_race.name, Race(temp_race)});
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
                temp_race.bodymap.insert({bpart.get_name(), bpart});
            }
        }
    }
}

std::string Race::gat_name() const {
    return name;
}

uint16_t Race::get_rand_body_size() const {
    if (min_body_size == max_body_size)
        return  min_body_size;
    else return min_body_size + (rand() % (max_body_size - min_body_size));
}

std::unordered_map<std::string, Bodypart> Race::get_bodymap() const {
    return bodymap;
}
