#include "dungeongenerator.h"
#include <cmath>
#include <vector>
#include <list>
#include <memory>
#include "data.h"
#include "tile.h"
#include "map.h"
#include "room.h"
#include "roomgenerator.h"
#include "npc.h"
#include "npcfactory.h"

DungeonGenerator::DungeonGenerator(const Map::ptr& map, const ActorManager::ptr& actor_manager)
    : map(map)
    , actor_manager(actor_manager) {
    room_generator = std::make_shared<RoomGenerator>(map, actor_manager);
}

DungeonGenerator::Area DungeonGenerator::make_area(Rectangle rect) {
    Area area;
    auto rect_width = (rect.last.x - rect.first.x);
    auto rect_height = (rect.last.y - rect.first.y);
    area.reserve(rect_width * rect_height);
    for (uint8_t i = rect.first.x; i <= rect.last.x; i++)
        for (uint8_t j = rect.first.y; j <= rect.last.y; j++)
            area.push_back(map->at({i, j}));
    return area;
}

bool DungeonGenerator::is_overlapping_rooms(Rectangle rect) {
    for (auto& area : rooms)
        if (rect.is_overlapping(area))
            return true;
    return false;
}

bool DungeonGenerator::can_be_passable(Tile::ptr tile) {
    for (auto& adjacent : tile->get_adjacent())
        if (!adjacent->is_wall())
            return false;
    return true;
}

void DungeonGenerator::make_tunnel(Tile::ptr current, Area& area) {
    auto previous = current;
    do {
        // make passable
        current->set_passable(true);
        current->set_texture(img("tile_floor.png"));
        area.push_back(current);

        // removing tiles, which cannot be passable
        auto adjacent = current->get_adjacent();
        auto tile_iter = adjacent.begin();
        while (tile_iter != adjacent.end()) {
            auto tile_pos = (*tile_iter)->get_pos();
            // if tile isn't surrounded by walls, except current tile
            if ([&tile_iter, &current] () -> bool {
                for (const auto& it : (*tile_iter)->get_adjacent())
                    if (!it->is_wall() && it != current)
                        return true;
                return false;
            }()) adjacent.erase(tile_iter);
            // restriction of map borders
            else if ( tile_pos.x == 0 || tile_pos.x == map->get_width() - 1 ||
                      tile_pos.y == 0 || tile_pos.y == map->get_height() - 1)
                adjacent.erase(tile_iter);
            else if (*tile_iter == previous)
                adjacent.erase(tile_iter);
            else tile_iter++;
        }

        if (adjacent.empty())
            return;

        // continuing tunnel in random direction
        auto next = adjacent[rand()%adjacent.size()];
        previous = current;
        current = next;
    } while (true);
}

bool DungeonGenerator::is_in_area(Tile::ptr tile, const DungeonGenerator::Area& area) {
    for (auto& it : area)
        if (it == tile)
            return true;
    return false;
}

DungeonGenerator::AreaIterator DungeonGenerator::find_area(Tile::ptr tile) {
    auto area = areas.begin();
    while (area != areas.end()) {
        for (const auto& it : *area)
            if (tile == it)
                return  area;
        area++;
    }
    return area;
}

void DungeonGenerator::merge_areas(AreaIterator fst, AreaIterator snd) {
    fst->reserve(fst->size() + snd->size());
    fst->insert(fst->end(), snd->begin(), snd->end());
    areas.erase(snd);
}

void DungeonGenerator::make_rooms(uint16_t attempts, uint8_t min_size, uint8_t max_size) {
    rooms.reserve(attempts);
    areas.reserve(attempts);
    for (uint16_t i = 0; i < attempts; i++) {
        Rectangle room_square;
        // statatic casts just to silence -Wc++11-narrowing issue
        uint8_t width = rand()%max_size + min_size;
        uint8_t height = rand()%max_size + min_size;
        room_square.first = {
            static_cast<uint8_t>( 1 + rand()%(map->get_width() - width - 1)),
            static_cast<uint8_t>( 1 + rand()%(map->get_height() - height - 1))
        };
        room_square.last = {
            static_cast<uint8_t>(room_square.first.x + width),
            static_cast<uint8_t>(room_square.first.y + height)
        };

        Rectangle check_rect = {
            {static_cast<uint8_t>(room_square.first.x - 1), static_cast<uint8_t>(room_square.first.y - 1)},
            {static_cast<uint8_t>(room_square.last.x + 1), static_cast<uint8_t>(room_square.last.y + 1)}
        };
        if (is_overlapping_rooms(check_rect))
            continue;

        rooms.push_back(room_square);
        areas.push_back(make_area(room_square));

        // making room
        room_generator->generate(room_square);
    }
}

void DungeonGenerator::make_maze() {
    for (uint8_t i = 1; i < map->get_height() - 2; i++)
        for (uint8_t j = 1; j < map->get_width() - 2; j++)
            if (can_be_passable(map->at({j, i}))) {
                Area tunnel;
                make_tunnel(map->at({j, i}), tunnel);
                areas.push_back(tunnel);
            }
}

void DungeonGenerator::connect_areas(uint8_t connection_chance) {
    // abstraction for tile that can connect two areas
    struct Connector {
        Tile::ptr tile;
        std::vector<Tile::ptr> adjacent;

        explicit Connector(Tile::ptr tile)
            : tile(tile) {
            adjacent.reserve(4);
        }

        bool is_connector() {
            if (!tile->is_wall())
                return false;
            for (const auto& it : tile->get_adjacent())
                if (!it->is_wall())
                    adjacent.push_back(it);
            return adjacent.size() == 2;
        }
    };

    std::vector<Connector> connectors;
    for (uint8_t i = 1; i < map->get_height() - 2; i++) {
        for (uint8_t j = 1; j < map->get_width() - 2; j++) {
            Connector tile { map->at({j, i}) };
            if (tile.is_connector()) connectors.push_back(tile);
        }
    }
    // merging areas
    for (const auto& connector : connectors) {
        // connector size must be equal 2
        auto first_area = find_area(connector.adjacent[0]);
        auto second_area = find_area(connector.adjacent[1]);
        // if areas different => zettai merging
        // else merging with chance
        // if both areas rooms => connect it
        if (first_area != second_area) {
            // make passable
            connector.tile->set_passable(true);
            // getting texture of adjacent floor tile
            connector.tile->set_texture(connector.adjacent[0]->get_texture());
            merge_areas(first_area, second_area);
        } else if (connection_chance > 1 + rand()%100) {
            // make passable
            connector.tile->set_passable(true);
            // getting texture of adjacent floor tile
            connector.tile->set_texture(connector.adjacent[0]->get_texture());
        }
    }
}

void DungeonGenerator::remove_deadends() {
    bool done = false;
    while (!done) {
        done = true;
        for (uint8_t i = 0; i < map->get_height() - 1; i++) {
            for (uint8_t j = 0; j < map->get_width() - 1; j++) {
                auto tile = map->at({j, i});
                auto adjacent = tile->get_adjacent();
                if (tile->is_passable() && [&tile, &adjacent] () -> bool {
                    uint8_t counter = 0;
                    for (const auto& it : adjacent)
                        if (it->is_wall())
                            counter++;
                    return counter == 3;
                }()) {
                    done = false;
                    // make non-passable
                    tile->set_passable(false);
                    // getting texture of adjacent wall
                    auto it = adjacent.begin();
                    while (!(*it)->is_wall())
                        it++;
                    tile->set_texture((*it)->get_texture());
                }
            }
        }
    }
}

void DungeonGenerator::place_npc(uint8_t quantity, uint8_t min_squad_size, uint8_t max_squad_size) {
    NPCFactory factory {actor_manager};
    std::string type = "Pirate";
    // getting random starting room
    auto room = rooms[rand()%rooms.size()];
    // until quantity == 0
    do {
        // in each adjacent room place npc squad
        auto squad_size = min_squad_size + (rand() % (max_squad_size - min_squad_size));
        std::vector<std::shared_ptr<NPC>> squad = factory.get_squad(type, squad_size);
        place_actors({begin(squad), end(squad)}, room);
        quantity -= squad_size;
        // choose next room from adjacent to last selected
        room = rooms[rand()%rooms.size()];
    } while (quantity > 0);
}

void DungeonGenerator::place_actors(std::vector<std::shared_ptr<Actor>>&& actors, Rectangle area) const {
    // forming square with actors
    uint8_t placement_width = ceil(sqrt(actors.size()));
    Coord placement_start = {
        static_cast<uint8_t>(area.first.x + rand()% (area.get_width() - placement_width)),
        static_cast<uint8_t>(area.first.y + rand()% (area.get_height() - placement_width))
    };
    Rectangle placement = {
        placement_start, {
            static_cast<uint8_t>(placement_start.x + placement_width),
            static_cast<uint8_t>(placement_start.y + placement_width)
        }
    };

    // placing each actor
    for (uint8_t i = placement.first.x; i <= placement.last.x; i++) {
        for (uint8_t j = placement.first.y; j <= placement.last.y; j++) {
            // if all actors placed => return
            if (actors.empty())
                return;
            auto actor = *actors.begin();
            // TODO: add check if actor can be placed on this tile
            // if actor cannot be placed => search for tile where he could be placed
            actor_manager->place_at({i, j}, actor);
            actors.erase(actors.begin());
        }
    }
}

void DungeonGenerator::generate() {
    make_rooms(30, 3, 5);
    make_maze();
    connect_areas(5);
    remove_deadends();
    place_npc(30, 3, 6);
}

void DungeonGenerator::place_players(std::list<Player::ptr> players) {
    Rectangle room;
    // check if all players can fit in this room
    // if players can't fit in this room => choose next one
    uint8_t counter;
    do {
        counter = 0;
        room = rooms[rand()%rooms.size()];
        for (uint8_t i = room.first.x; i <= room.last.x; i++)
            for (uint8_t j = room.first.y; j <= room.last.y; j++)
                if (map->at({i, j})->is_passable())
                    counter++;
    } while (counter < players.size());

    place_actors({begin(players), end(players)}, room);
}
