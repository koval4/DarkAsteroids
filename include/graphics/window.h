#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include <string_view>
#include "graphics/sdlwrappers.h"

class Window {
public:
    Window(std::string_view name, int width, int height);

    int get_width() const;
    int get_height() const;

private:
    SdlWindow impl;
};

#endif
