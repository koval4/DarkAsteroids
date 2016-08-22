#ifndef NPC_H
#define NPC_H

#include "actor.h"
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include "tile.h"
#include "attack.h"

class NPC : public Actor {
    private:
        std::queue<std::function<void(void)> > actions;
        std::vector<Coord> path;

        std::vector<Tile::ptr> get_tiles_in_fov() const;
        std::vector<Actor::ptr> get_actors_in_fov() const;
        std::shared_ptr<Attack> choose_attack();

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

        std::function<void(void)> get_action();
};

#endif // NPC_H
