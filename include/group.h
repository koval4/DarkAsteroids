#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <memory>
#include <unordered_map>

class Group {
    private:
        std::string name;

        static std::unordered_map<std::string, std::unordered_map<std::string, uint8_t>> relations;

    public:
        Group();
        explicit Group(const std::string& name);

        std::string get_name() const;
        uint8_t get_relation_to(const Group& other) const;
};

#endif
