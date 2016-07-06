#ifndef NPC_H
#define NPC_H

#include "actor.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include "tile.h"

class NPC : public Actor {
    public:
        typedef std::unique_ptr<NPC> ptr;
        static std::unordered_map<std::string, NPC> NPC_LIST;
    private:
        std::vector<Tile::ptr> get_tiles_in_fov() const;
        std::vector<Actor::ptr> get_actors_in_fov() const;

    public:
        //############### CONSTRUCTORS ##############
        NPC(const NPC& obj);

        //############### DATABASE ###############
        static void read_npc_txt();

        //############# GAME LOGIC #############
        virtual void make_turn() override;
        virtual void pick_item() override;
        virtual void attack(Coord target) override;
};

#endif // NPC_H
