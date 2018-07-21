#ifndef SDL_WRAPPERS_H
#define SDL_WRAPPERS_H

#include <memory>
#include <string_view>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

using SdlWindow = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

// SDL_GLContext is actually void* and SDL_GL_CreateContext returns SDL_GLContext
// soooo i should store in unique_ptr void to be void*, that's a bit messed up, but oh well...
using SdlContext = std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)>;

using SdlRenderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

using SdlSurface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

SdlWindow make_sdl_window(std::string_view name, int width, int height) noexcept;
SdlContext make_sdl_context(const SdlWindow& window) noexcept;
SdlRenderer make_sdl_renderer(const SdlWindow& window) noexcept;
SdlSurface make_sdl_texture(std::string_view filename) noexcept;

#endif // SDL_WRAPPERS_H
