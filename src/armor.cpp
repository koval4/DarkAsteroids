#include "armor.h"
#include "item.h"
#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>

std::map<std::string, Armor> Armor::LIST;

Armor::Armor()
    : Item() {
    type = armor;
}

Armor::Armor(const Armor& obj)
    : Item(obj) {}

Item::ptr Armor::make(std::string prototype) {
    Item::ptr item(new Armor(Armor::LIST.at(prototype)));
    return item;
}

Item::ptr Armor::clone() {
    return Item::ptr(new Armor(*this));
}

void Armor::read_armor_txt() {
    std::ifstream file("armor.txt");
    std::string line = "";

    if (!file.is_open()) {
        log_file << "Unable to open armor.txt" << std::endl;
        std::cerr << "Unable to open armor.txt" << std::endl;
        return;
    }

    Armor* armor = nullptr;
    while (std::getline(file, line)) {
        if (line.front() == '#') continue;
        if (line.find_first_not_of(" \n\t\r") == std::string::npos) continue;   //if line is empty
        while (line.find(";") == std::string::npos){    //line ends with semicolon
            std::string buffer_line = "";
            std::getline(file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line); //erasing unnecessary from end
        if (line == "NEW_ITEM")
            armor = new Armor();
        else if (line == "PUSH_ITEM") {
            LIST.insert(std::pair<std::string, Armor>(armor->name, *armor));
            log_file << "Added new item to Armor::LIST: " << armor->name << std::endl;
            delete armor;
        } else armor->read_txt_properties(line);
    }
    file.close();
}

std::string Armor::describe() {
    std::string line = name + "\n";
    line += description + "\n";
    line += "Weight: " + std::to_string(weight) + "\n";
    line += "Price: " + std::to_string(price) + "\n";
    return line;
}
