#include "graphics/sdlwrappers.h"

SdlWindow make_sdl_window(std::string_view name, int width, int height) noexcept {
    SDL_Window* ptr = SDL_CreateWindow(
        name.data(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL
    );
    return SdlWindow { ptr, &SDL_DestroyWindow };
}

SdlContext make_sdl_context(const SdlWindow& window) noexcept {
    SDL_GLContext ptr = SDL_GL_CreateContext(window.get());
    return SdlContext { ptr, &SDL_GL_DeleteContext };
}

SdlRenderer make_sdl_renderer(const SdlWindow& window) noexcept {
    SDL_Renderer* ptr = SDL_CreateRenderer(
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return SdlRenderer { ptr, &SDL_DestroyRenderer };
}

SdlSurface make_sdl_texture(std::string_view filename) noexcept {
    SDL_Surface* ptr = IMG_Load(filename.data());
    return SdlSurface { ptr, &SDL_FreeSurface };
}

