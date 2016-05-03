#include "ranged_attack.h"
#include <string>
#include "attack.h"
#include "weapon.h"
#include "tile.h"
#include "map.h"

RangedAttack::RangedAttack()
    : Attack()
    , ammo_usage(0)
    , accuracy(0)
    , ammo_type()
    , projectile() {}

RangedAttack::RangedAttack(const RangedAttack& obj)
    : Attack(obj)
    , ammo_usage(obj.ammo_usage)
    , accuracy(obj.accuracy)
    , ammo_type(obj.ammo_type)
    , projectile(obj.projectile) {}

RangedAttack::RangedAttack(Weapon* parent, std::string line)
    : Attack(parent, line) {
    for (auto& it : split_with_match_chars(line)) {
        if (it.find("ammo_usage") != std::string::npos) {
            ammo_usage = std::stoi(take_value(it));
        } else if (it.find("accuracy") != std::string::npos) {
            accuracy = std::stoi(take_value(it));
        } else if (it.find("ammo_type") != std::string::npos) {
            ammo_type = take_value(it);
        } else if (it.find("projectile") != std::string::npos) {
            projectile = take_value(it);
        }
    }
}

uint8_t RangedAttack::get_ammo_usage() const {
    return ammo_usage;
}

uint8_t RangedAttack::get_accuracy() const {
    return accuracy;
}

std::string RangedAttack::get_ammo_type() const {
    return ammo_type;
}

std::string RangedAttack::get_projectile() const {
    return projectile;
}

Item::ptr RangedAttack::spawn_projectile() {
    if (projectile == "ammo") {
        return parent->get_ammo_info().at(ammo_type).ammo->clone();
    } else {
        projectile = projectile.substr(projectile.find("{") + 1, projectile.find("}") - 1);
        Item::ptr proj(new Item());
        for (auto& parameter : split_with_match_chars(projectile))
            proj->read_txt_properties(parameter);
        return proj;
    }
}

void RangedAttack::make_attack(Actor& user
                               , Coord start
                               , Coord end
                               , Actor* victim
                               , Bodypart* target
                               ) {
    // calculating vector of direction
    float_t dx = (float_t)end.x - (float_t)start.x;
    float_t dy = (float_t)end.y - (float_t)start.y;
    float_t length = sqrt(dx*dx + dy*dy);
    // reducing length to 1;
    dx /= length;
    dy /= length;

    for (uint8_t i = 0; i < ammo_usage; i++) {
        // (x, y) -- projectile position on map, float for accuracy
        float x = start.x;
        float y = start.y;
        Item::ptr projectile = spawn_projectile();
        // starting momentum of projectile
        int32_t momentum = get_momentum(projectile->get_weight(), start_force);
        while (momentum > 0) {
            Tile::ptr tile = Map::curr_map->at({(int)round(x),(int)round(y)});
            if (!tile->is_passable())
                break;

            // TODO: check for collision with items

            auto tile_actor = tile->get_actor();
            if (tile_actor && tile_actor.get() == victim)
                victim->get_wound(momentum, *projectile, contact_area, target);
            else if (tile_actor)
                tile_actor->get_wound(momentum, *projectile, contact_area, nullptr);

            x += dx;
            y += dy;
        }
    }
}
