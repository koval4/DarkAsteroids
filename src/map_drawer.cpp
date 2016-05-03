#include "map_drawer.h"
#include <vector>
#include <unordered_set>
#include "ui/screen.h"
#include "ui/gui.h"
#include "tile.h"
#include "map.h"

MapDrawer::MapDrawer()
    : tiles_horizntally(Screen::inst().get_width() / Tile::size)
    , tiles_vertically(Screen::inst().get_height() / Tile::size) {}

void MapDrawer::draw_map(Tile::ptr center) const {
    Coord viewpoint = center->get_pos();
    Coord first = { viewpoint.x - (tiles_horizntally / 2)
                  , viewpoint.y - (tiles_vertically / 2) };
    Coord last = { viewpoint.x + (tiles_horizntally / 2)
                 , viewpoint.y + (tiles_vertically / 2) };
    std::vector<Tile::ptr> tiles;
    std::unordered_set<Tile::ptr> drawn_tiles;
    tiles.reserve(tiles_horizntally * tiles_vertically);
    tiles.push_back(center);

    while (!tiles.empty()) {
        Tile::ptr tile = *(tiles.end() - 1);
        tiles.pop_back();
        if (drawn_tiles.find(tile) != drawn_tiles.end())
            continue;
        drawn_tiles.insert(tile);
        Coord pos = tile->get_pos();
        if (pos.x < first.x || pos.x > last.x || pos.y < first.y || pos.y > last.y)
            continue;

        uint16_t x = (pos.x - first.x) * Tile::size;
        uint16_t y = (pos.y - first.y) * Tile::size;
        GUI::inst().add_texture(tile->get_texture(), x, y, Tile::size, Tile::size);

        if (!tile->is_passable() && !tile->get_actor())
            continue;
        std::vector<Tile::ptr> adjacent = tile->get_adjacent();
        tiles.insert(tiles.end(), adjacent.begin(), adjacent.end());
    }
    GUI::inst().flush();
    GUI::inst().update();
}
