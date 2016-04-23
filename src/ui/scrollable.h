#ifndef SCROLLABLE_H
#define SCROLLABLE_H

#include "slidebar.h"

class Slidebar;

/**
 * @brief The Scrollable abstract class describes widget that can be scrolled with slidebar
 */
class Scrollable {
    protected:
        uint16_t pos;       /// current position in scrolled object
        Slidebar* slidebar; /// slidebar used for scrolling

        virtual void update_bar() = 0;  /// updates sizes of slidebar and slider
    public:
        Scrollable();
        virtual ~Scrollable();
        virtual void update_pos() = 0;  /// updates current position
        void make_binded_to(Slidebar* slidebar);    /// makes this object connected with slidebar
};

#endif // SCROLLABLE_H
