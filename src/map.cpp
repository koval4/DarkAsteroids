#include "map.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "data.h"
#include "tile.h"
#include "ui/screen.h"
#include "ui/gui.h"
#include "npc.h"

//############# STATIC VARIABLES #################

Map* Map::curr_map = nullptr; /// pointer to current game map

//################### CONSTRUCTORS ###################

Map::Map() {}

//################### MAP OPERATIONS ##################

void Map::generate_map(uint16_t width, uint16_t height, std::vector<Actor::ptr>& players) {
    // allocating memory for map
    map.reserve(height);
    for (size_t i = 0; i < width; i++) {
        map.push_back(std::vector<Tile::ptr>(width));
        for (size_t j = 0; j < height; j++) {
            map[i][j] = Tile::ptr(new Tile{ {i, j}, false, img("tile_wall.png") });
        }
    }

    // connecting tiles
    for (size_t i = 1; i < width - 1; i++) {
        for (size_t j = 1; j < height - 1; j++) {
            map[i][j]->connect(map[i+1][j]);
            map[i][j]->connect(map[i-1][j]);
            map[i][j]->connect(map[i][j+1]);
            map[i][j]->connect(map[i][j-1]);
        }
    }

    // making rooms in map
    std::vector<Coord> waypoints;
    for (uint16_t i = 0; i < (width / 3); i++) {
        int waypoint_x = rand()%(height - 2) + 1;
        int waypoint_y = rand()%(width - 2) + 1;
        waypoints.push_back({waypoint_x, waypoint_y});
    }
    for (auto& it : waypoints) {
        uint16_t radius = rand()%2 + 1;
        uint16_t start_x = (it.x > radius) ? it.x - radius : it.x;
        uint16_t start_y = (it.y > radius) ? it.y - radius : it.y;
        uint16_t end_x = (it.x + radius < width - 1)  ? it.x + radius : it.x;
        uint16_t end_y = (it.y + radius < height - 1) ? it.y + radius : it.y;
        for (uint16_t i = start_x; i < end_x; i++) {
            for (uint16_t j = start_y; j < end_y; j++) {
                map[i][j]->set_texture(img("tile_floor.png"));
                map[i][j]->set_passable(true);
                map[i][j]->put_item(Item::make("Rock"));
            }
        }
        map[it.x][it.y]->place_actor(Actor::ptr(new NPC(NPC::NPC_LIST.at("Pirate"))));
    }
    Coord first_wp = waypoints[0];
    players[0]->set_map(this);
    players[0]->set_pos(first_wp);
    map[first_wp.x][first_wp.y]->place_actor(std::move(players[0]));
    actors.push_back(map[first_wp.x][first_wp.y]->get_actor().get());

    // connecting rooms lineary
    for (uint16_t i = 0; i < waypoints.size() - 2; i++) {
        Coord curr_coord = waypoints[i];
        while (curr_coord.x != waypoints[i + 1].x
               && curr_coord.y != waypoints[i + 1].y) {
            //
            map[curr_coord.x][curr_coord.y]->set_texture(img("tile_floor.png"));
            map[curr_coord.x][curr_coord.y]->set_passable(true);
            int16_t dx = curr_coord.x - waypoints[i + 1].x;
            int16_t dy = curr_coord.y - waypoints[i + 1].y;
            int16_t x = 0;
            int16_t y = 0;

            if (dx != 0)
                x = (waypoints[i].x < waypoints[i + 1].x) ? 1 : -1;
            if (dy != 0)
                y = (waypoints[i].y < waypoints[i + 1].y) ? 1 : -1;
            if (x != 0 && y != 0) {
                if (rand()%2)
                    x = 0;
                else y = 0;
            }
            curr_coord.x += x;
            curr_coord.y += y;
        }
    }
}

Tile::ptr Map::at(Coord pos) const {
    return map[pos.x][pos.y];
}

//################## SETTERS #####################

//################# GETTERS ###################

std::vector<Actor*> Map::get_actors() const {
    return actors;
}

bool Map::is_in_sight(Coord start, Coord end) {
    return false;
//    if (!map[end.x][end.y].is_visible())
//        return false;
//    int32_t dx = start.x - end.x;
//    int32_t dy = start.y - end.y;
//    int32_t adx = abs(dx);
//    int32_t ady = abs(dy);
//    int8_t sign_x = dx > 0 ? 1 : -1;
//    int8_t sign_y = dy > 0 ? 1 : -1;
//    int32_t x = start.x;
//    int32_t y = start.y;
//    if (adx > ady) {
//        int32_t t = ady*2 - adx;
//        do {
//            if (t >= 0) {
//                y += sign_y;
//                t -= adx*2;
//            }
//            x += sign_x;
//            t += ady*2;
//            if (end.x == x && end.y == y)
//                return true;
//        } while (map[x][y].is_visible());
//        return false;
//    } else {
//        int32_t t = adx*2 - ady;
//        do {
//            if (t >= 0) {
//                x += sign_x;
//                t -= ady*2;
//            }
//            y += sign_y;
//            t += adx*2;
//            if (end.x == x && end.y == y)
//                return true;
//        } while (map[x][y].is_visible());
//        return false;
//    }
}
