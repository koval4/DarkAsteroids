#include "graphics/window.h"

Window::Window(std::string_view name, int width, int height)
    : impl{make_sdl_window(name, width, height)} {}

int Window::get_width() const {
    int width = 0;
    SDL_GetWindowSize(impl.get(), &width, nullptr);
    return width;
}

int Window::get_height() const {
    int height = 0;
    SDL_GetWindowSize(impl.get(), nullptr, &height);
    return height;
}
