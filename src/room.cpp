#include "room.h"

Room::Room(Rectangle area)
    : area(area) {}

void Room::connect_with(const std::shared_ptr<Room>& other) {
    connected_rooms.push_back(other);
    other->connected_rooms.push_back(shared_from_this());
}

Rectangle Room::get_area() const {
    return area;
}
