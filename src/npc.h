#ifndef NPC_H
#define NPC_H

#include "actor.h"
#include <map>
#include <memory>

class NPC : public Actor {
    public:
        typedef std::unique_ptr<NPC> ptr;
        static std::map<std::string, NPC> NPC_LIST;
    private:
    public:
        //############### CONSTRUCTORS ##############
        NPC();
        NPC(const NPC& obj);

        //############### DATABASE ###############
        static void read_npc_txt();

        //############# GAME LOGIC #############
        virtual void make_turn() override;
        virtual void pick_item() override;
        virtual void attack(Coord target) override;
};

#endif // NPC_H
