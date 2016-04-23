#include "stack.h"

Stack::Stack()
    : Item()
    , size(0) {}

Stack::Stack(Item::ptr item, size_t size)
    : Item(*item)
    , items(std::move(item))
    , size(size) {
    name += " x" + std::to_string(size);
}

Stack::Stack(const Stack& obj)
    : Item(obj)
    , items(new Item(*obj.items))
    , size(obj.size) {}

void Stack::update_name() {
    name = items->get_name() + " x" + std::to_string(size);
}

size_t Stack::get_size() const {
    return size;
}

void Stack::add(Item::ptr item) {
    items = std::move(item);
    size++;
    update_name();
}

Item::ptr Stack::take() {
    size--;
    update_name();
    return Item::ptr(new Item(*items));
}
