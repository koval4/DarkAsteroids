#ifndef MAPDRAWER_H
#define MAPDRAWER_H

#include "ui/screen.h"
#include "ui/gui.h"
#include "tile.h"
#include "map.h"

class MapDrawer {
    private:
        const uint8_t tiles_horizntally;
        const uint8_t tiles_vertically;

    public:
        MapDrawer();
        void draw_map(Tile::ptr center) const;
};

#endif // MAPDRAWER_H
