#include "tile.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "common.h"
#include "actor.h"
#include "item.h"

uint16_t Tile::size     = 100;  /// size of tile's side in pixels
uint16_t Tile::volume   = 1500; /// volume of space in tile

Tile::Tile()
    : pos()
    , passable(false)
    , texture()
    , actor(nullptr) {}

Tile::Tile(Coord pos, bool passable, std::string texture)
    : pos(pos)
    , passable(passable)
    , texture(texture)
    , actor(nullptr) {}

Tile::~Tile() {
    adjacent.clear();
}

Coord Tile::get_pos() const {
    return pos;
}

bool Tile::is_wall() const {
    return !passable;
}

std::string Tile::get_texture() const {
    if (actor)
        return actor->get_texture();
    else return texture;
}

std::vector<Item::ptr> Tile::get_items() const {
    std::vector<Item::ptr> items_ref;
    for (auto& it : items)
        items_ref.push_back(it);
    return items_ref;
}

OptRef<Actor> Tile::get_actor() {
    return actor;
}

std::vector<Tile::ptr> Tile::get_adjacent() const {
    std::vector<Tile::ptr> tiles;
    for (auto& it : adjacent) {
        tiles.push_back(it.second);
    }
    return tiles;
}

Tile::ptr Tile::get_tile(Coord coord) const {
    if (adjacent.find(coord) != adjacent.end())
        return adjacent.at(coord);
    else return ptr(nullptr);
}

void Tile::set_texture(std::string texture) {
    this->texture = texture;
}

void Tile::set_passable(bool passable) {
    this->passable = passable;
}

Item::ptr Tile::take_item(Item::ptr& item) {
    auto it = items.begin();
    while (it != items.end()) {
        if (*it == item) {
            Item::ptr itm = *it;
            items.erase(it);
            return itm;
        }
        it++;
    }
    return nullptr;
}

void Tile::put_item(Item::ptr item) {
    items.push_back(std::move(item));
}

Actor* Tile::remove_actor() {
    return actor;
}

void Tile::place_actor(Actor* const actor) {
    this->actor = actor;
    this->actor->set_pos(pos);
}

void Tile::connect(Tile::ptr tile) {
    adjacent[tile->pos] = tile;
    tile->adjacent[pos] = shared_from_this();
}

bool Tile::is_visible_from(const Tile& other, uint16_t range) const {
    if (range == 0)
        return false;
    float dx = other.pos.x - pos.x;
    float dy = other.pos.y - pos.y;
    float length = sqrt(dx*dx + dy*dy);
    Coord next_pos = {
        static_cast<uint8_t>(other.pos.x + round(dx / length)),
        static_cast<uint8_t>(other.pos.y + round(dy / length))
    };
    auto next_tile = other.adjacent.at(next_pos);
    if (next_tile.get() == this)
        return true;
    else return is_visible_from(*next_tile, range - 1);
}
