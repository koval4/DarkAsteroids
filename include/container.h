#ifndef CONTAINER_H
#define CONTAINER_H

#include "item.h"
#include <vector>
#include <memory>
#include <map>

class Container : public Item {
    public:
        typedef std::shared_ptr<Container> ptr;
        static std::map<std::string, Container> LIST;   /// database with prototypes for containers

    private:
        std::vector<Item::ptr> content; /// items stored tin this container

    public:
        //############## CONSTRUCTORS ##############
        Container();
        Container(const Container& obj);

        //############## PTRS #################
        static Item::ptr make(std::string prototype);
        Item::ptr clone() override;

        //############ DATABASE #############
        /**
         * @brief read_containers_txt -- fills database with prototypes from containers.txt file
         */
        static void read_containers_txt();

        //########### ITEM LOGIC #############
        std::string describe() override;

        //############ CONTENT INTERFACE ############
        /**
         * @brief add_item -- adds item to container's content
         * @param item     -- item that will be put in container
         */
        void add_item(Item::ptr item);
        /**
         * @brief remove_item -- removes item from container's content
         * @param item        -- item that will be removed from container
         */
        void remove_item(Item* item);
        /**
         * @brief take_item -- moves item from container
         * @param item      -- item that will be taken from container
         * @return          -- item that was taken from container
         */
        Item::ptr take_item(Item* item);
        /**
         * @brief open_container -- returns collection with content of this container
         * @return               -- content of this container
         */
        std::vector<Item*> open_container();
};

#endif // CONTAINER_H
