#ifndef ARMOR_H
#define ARMOR_H

#include "item.h"
#include <map>
#include <memory>
#include <string>

class Armor : public Item {
    public:
        typedef std::unique_ptr<Armor> ptr;
        static std::map<std::string, Armor> LIST;
    private:
    public:
        //########### CONSTRUCTORS #############
        Armor();
        Armor(const Armor& obj);

        //############## PTRS #################
        static Item::ptr make(std::string prototype);
        Item::ptr clone() override;

        //########### DATABASE ###############
        static void read_armor_txt();

        //########### ITEM LOGIC ##############
        std::string describe() override;
};

#endif // ARMOR_H
