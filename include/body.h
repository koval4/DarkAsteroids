#ifndef BODY_H
#define BODY_H

#include <unordered_map>
#include <string>
#include <memory>
#include "bodypart.h"
#include "race.h"

class Body {
    private:
        std::unordered_map<std::string, std::shared_ptr<Bodypart>> parts;
        uint16_t body_size;         /// total size of a body

    public:
        Body(const Race& race, uint16_t max_weight);

        std::unordered_map<std::string, std::shared_ptr<Bodypart>> get_parts() const;
        const std::shared_ptr<Bodypart> get_random_part() const;

        void feel_pain();
};

#endif // BODY_H
