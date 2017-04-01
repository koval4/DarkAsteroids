#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "tile.h"

class Room : public std::enable_shared_from_this<Room> {
    public:
        using ptr = std::shared_ptr<Room>;

    private:
        Rectangle area;
        std::vector<Tile::ptr> tiles;
        std::vector<std::weak_ptr<Room>> connected_rooms;

    public:
        Room(Rectangle area);

        void connect_with(const std::shared_ptr<Room>& other);

        Rectangle get_area() const;
};

#endif // ROOM_H
