#ifndef STACK_H
#define STACK_H

#include <memory>
#include "item.h"

class Stack : public Item {
    public:
        typedef std::shared_ptr<Stack> ptr;

    private:
        Item::ptr items;
        size_t size;

        void update_name();

    public:
        Stack();
        Stack(Item::ptr item, size_t size);
        Stack(const Stack& obj);

        //########### GETTERS ##############
        size_t get_size() const;

        //########## STACK OPERATIONS #############
        void add(Item::ptr item);
        Item::ptr take();
};

#endif // STACK_H
