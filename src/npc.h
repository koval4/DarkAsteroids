#ifndef NPC_H
#define NPC_H

#include "actor.h"
#include <string>
#include <vector>
#include <memory>
#include "tile.h"

class NPC : public Actor {
    private:
        std::vector<Tile::ptr> get_tiles_in_fov() const;
        std::vector<Actor::ptr> get_actors_in_fov() const;

    public:
        //############### CONSTRUCTORS ##############
        NPC(std::string texture,
            std::string name,
            std::string description,
            Params params,
            Skills skills,
            Race race,
            uint16_t level,
            uint16_t experience);
        NPC(const NPC& obj);

        //############# GAME LOGIC #############
        virtual void make_turn() override;
        virtual void pick_item() override;
        virtual void attack(Coord target) override;
};

#endif // NPC_H
