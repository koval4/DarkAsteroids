#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>

class Group;

class GroupsManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<Group> > groups;
        std::unordered_map<std::string, std::unordered_map<std::string, uint8_t> > relations;

        GroupsManager();

    public:
        static GroupsManager& inst();
        static void from_file(std::string&& filename);

        void add_group(const std::shared_ptr<Group>& group);
        std::shared_ptr<Group> get_group(const std::string& name) const;
        uint8_t get_relation_between(const Group& first, const Group& second) const;
        void set_relation_between(const Group& first, const Group& second, uint8_t relation);
};

#endif
