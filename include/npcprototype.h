#ifndef NPCPROTOTYPE_H
#define NPCPROTOTYPE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "race.h"

class NPCPrototype {
    private:
        std::string name;
        std::string description;
        std::string texture;
        Race race;

    public:
        static std::unordered_map<std::string, NPCPrototype> PROTOTYPES;

        NPCPrototype();

        static void read_from_file(std::string filename);
        void parse_param_from_line(std::string&& line);

        std::string get_name() const;
        std::string get_description() const;
        std::string get_texture() const;
        Race get_race() const;
};

#endif
