#include "map.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "data.h"
#include "ui/screen.h"
#include "ui/gui.h"
#include "npc.h"

//############# STATIC VARIABLES #################

uint16_t Map::Tile::tile_size = 100;  /// size of tile's side in pixels
Map* Map::curr_map = nullptr;	/// pointer to current game map

Map::Tile::Tile()
    : passable(false)
    , visible(true)
    , texture(img("tile_wall.png"))
    , actor(nullptr) {}

//################### CONSTRUCTORS ###################

Map::Map() {
    viewpoint = {0, 0};
}

//################### MAP OPERATIONS ##################

void Map::generate_map(uint16_t width, uint16_t height, std::vector<Actor::ptr>& players) {
    // allocating memory for map
    for (uint16_t i = 0; i < height; i++) {
        map.push_back(std::vector<Tile>(width));
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
                map[i][j].passable = true;
                map[i][j].texture = img("tile_floor.png");
                map[i][j].items.push_back(Item::ptr(new Item(Item::ITEMS_LIST.at("Rock"))));
            }
        }
        map[it.x][it.y].actor = Actor::ptr(new NPC(NPC::NPC_LIST.at("Pirate")));
    }
    Coord first_wp = waypoints[0];
    players[0]->set_map(this);
    players[0]->set_pos(first_wp);
    map[first_wp.x][first_wp.y].actor = std::move(players[0]);
    actors.push_back(map[first_wp.x][first_wp.y].actor.get());

    // connecting rooms lineary
    for (uint16_t i = 0; i < waypoints.size() - 2; i++) {
        Coord curr_coord = waypoints[i];
        while (curr_coord.x != waypoints[i + 1].x
               && curr_coord.y != waypoints[i + 1].y) {
            //
            map[curr_coord.x][curr_coord.y].passable = true;
            map[curr_coord.x][curr_coord.y].texture = img("tile_floor.png");
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

#ifdef _DEBUG_
    for (auto& it_row : map) {
        for (auto& it_cell : it_row) {
            if (it_cell.passable)
                std::cout << ".";
            else std::cout << "#";
        }
        std::cout << std::endl;
    }
#endif
}

void Map::draw_map() {
    uint16_t tiles_v = Screen::inst().get_height() / Tile::tile_size;
    uint16_t tiles_h = Screen::inst().get_width() / Tile::tile_size;
    Coord start = { (uint16_t)(viewpoint.x - (tiles_h/2))
                  , (uint16_t)(viewpoint.y - (tiles_v/2))
                  };

    for (uint16_t i = 0; i < tiles_v; i++) {
        for (uint16_t j = 0; j < tiles_h; j++) {
            Tile* this_tile = &map[j + start.x][i + start.y];
            if (this_tile->visible && this_tile->texture != "") {
                std::string texture;
                if (this_tile->actor)
                    texture = this_tile->actor->get_texture();
                else texture = this_tile->texture;
                GUI::inst().add_texture(texture
                        , j * Tile::tile_size, i *Tile::tile_size
                        , Tile::tile_size, Tile::tile_size);
            }
        }
    }
    GUI::inst().flush();
    GUI::inst().update();
}

Map::Tile& Map::at(Coord pos) {
    return map[pos.x][pos.y];
}

//################## SETTERS #####################

void Map::set_viewpoint(Coord viewpoint) {
    uint16_t tiles_v = Screen::inst().get_height() / Tile::tile_size;
    uint16_t tiles_h = Screen::inst().get_width() / Tile::tile_size;
    if (viewpoint.x < tiles_h/2)
        viewpoint.x = tiles_h/2;
    if (viewpoint.x > (map.size() - tiles_v/2))
        viewpoint.x = (map.size() - tiles_v/2);
    if (viewpoint.y < tiles_v/2)
        viewpoint.y = tiles_v/2;
    if (viewpoint.y > (map[viewpoint.x].size() - tiles_h/2))
        viewpoint.y = (map[viewpoint.x].size() - tiles_h/2);
    this->viewpoint = viewpoint;
    draw_map();
}

//################# GETTERS ###################

Coord Map::get_viewpoint() const {
    return viewpoint;
}

std::vector<Actor*> Map::get_actors() const {
    return actors;
}

bool Map::is_in_sight(Coord start, Coord end) {
    if (!map[end.x][end.y].visible)
        return false;
    int32_t dx = start.x - end.x;
    int32_t dy = start.y - end.y;
    int32_t adx = abs(dx);
    int32_t ady = abs(dy);
    int8_t sign_x = dx > 0 ? 1 : -1;
    int8_t sign_y = dy > 0 ? 1 : -1;
    int32_t x = start.x;
    int32_t y = start.y;
    if (adx > ady) {
        int32_t t = ady*2 - adx;
        do {
            if (t >= 0) {
                y += sign_y;
                t -= adx*2;
            }
            x += sign_x;
            t += ady*2;
            if (end.x == x && end.y == y)
                return true;
        } while (map[x][y].visible);
        return false;
    } else {
        int32_t t = adx*2 - ady;
        do {
            if (t >= 0) {
                x += sign_x;
                t -= ady*2;
            }
            y += sign_y;
            t += adx*2;
            if (end.x == x && end.y == y)
                return true;
        } while (map[x][y].visible);
        return false;
    }
}
