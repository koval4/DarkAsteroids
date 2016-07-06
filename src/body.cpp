#include "body.h"
#include <unordered_map>
#include <string>
#include <memory>
#include "bodypart.h"
#include "race.h"

Body::Body(const Race& race, uint16_t max_weight)
    : body_size(race.get_rand_body_size()) {
    for (auto& it : race.get_bodymap()) {
        parts.insert({it.first, std::make_shared<Bodypart>(it.second)});
        parts.at(it.first)->set_max_weight(max_weight / (body_size / parts.at(it.first)->get_volume()));
    }
}

std::unordered_map<std::string, std::shared_ptr<Bodypart>> Body::get_parts() const {
    return parts;
}

const std::shared_ptr<Bodypart> Body::get_random_part() const {
    auto it = parts.begin();
    for (size_t i = 0; i < rand() % parts.size(); i++)
        it++;
    return  it->second;
}

void Body::feel_pain() {

}
