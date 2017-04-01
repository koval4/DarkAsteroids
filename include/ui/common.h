#ifndef COMMON_H
#define COMMON_H

#include <fstream>
#include <ostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using EventType = decltype(std::declval<SDL_Event>().type);

struct Padding {
    uint16_t top;
    uint16_t left;
    uint16_t right;
    uint16_t bottom;

    Padding() : top(0), left(0), right(0), bottom(0) {}

    Padding(uint16_t value) : top(value), left(value), right(value), bottom(value) {}

    Padding(uint16_t top, uint16_t left, uint16_t right, uint16_t bottom) :
        top(top), left(left), right(right), bottom(bottom) {}
};

extern std::ofstream log_file;

#endif // COMMON_H
