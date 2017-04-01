#include "groupsmanager.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <tuple>
#include "group.h"
#include "common.h"
#include "filereader.h"

GroupsManager::GroupsManager() {}

GroupsManager& GroupsManager::inst() {
    static GroupsManager inst;
    return inst;
}

void GroupsManager::from_file(std::string&& filename) {
    std::ifstream file(filename);
    std::string line;

    auto factory = [] (std::string name) {
        return Group {name};
    };

    std::string buffer;
    while (std::getline(file, line)) {
        if (line.front() == '#')
            continue;
        if (line.find_first_not_of(" \t\n\r") == std::string::npos)
            continue;
            buffer += line;
    }
    auto groups = read_list<Group>(buffer, [factory] (std::string& source) {
        return read_object<Group>(source, factory, std::make_tuple(
            converter<std::string>{"name", read_string}
        ));
    });
    for (const auto& group : groups) {
        //add_group(std::make_shared<Group>(group));
    }
}

void GroupsManager::add_group(const std::shared_ptr<Group>& group) {
    groups.insert({group->get_name(), group});
    //relations.insert({group->get_name(), });
}

std::shared_ptr<Group> GroupsManager::get_group(const std::string& name) const {
    return groups.at(name);
}

uint8_t GroupsManager::get_relation_between(const Group& first, const Group& second) const {
    return relations.at(first.get_name()).at(second.get_name());
}


void GroupsManager::set_relation_between(const Group& first, const Group& second, uint8_t relation) {
    relations.at(first.get_name()).at(second.get_name()) = relation;
}
