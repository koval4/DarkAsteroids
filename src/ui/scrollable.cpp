#include "scrollable.h"

Scrollable::Scrollable() {
    slidebar = nullptr;
    pos = 0;
}

Scrollable::~Scrollable() {

}

void Scrollable::make_binded_to(Slidebar* slidebar) {
    this->slidebar = slidebar;
}
