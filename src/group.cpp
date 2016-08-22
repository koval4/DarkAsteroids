#include "group.h"
#include "groupsmanager.h"

Group::Group() {}

Group::Group(const std::string& name)
    : name(name) {}


std::string Group::get_name() const {
    return name;
}

uint8_t Group::get_relation_to(const Group& other) const {
    return GroupsManager::inst().get_relation_between(*this, other);
}
