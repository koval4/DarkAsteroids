#ifndef RACE_H
#define RACE_H

#include <string>
#include <unordered_map>
#include "bodypart.h"

class Race {
    private:
        std::string name;
        std::unordered_map<std::string, Bodypart> bodymap;
        uint16_t max_body_size;
        uint16_t min_body_size;

    public:
        Race();
        Race(const Race& other);

        //############ DATABASE ###################
        static std::unordered_map<std::string, Race> RACES_LIST;

        static void read_races_txt();

        //############### GETTERS ##################
        std::string gat_name() const;
        uint16_t get_rand_body_size() const;
        std::unordered_map<std::string, Bodypart> get_bodymap() const;
};

#endif // RACE_H
