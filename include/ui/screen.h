#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include "texture.h"

/** singleton class that contains information about window and renderer*/
class Screen {
    private:
        uint16_t screen_width;  ///width of window
        uint16_t screen_height; ///height of window
        SDL_Window* window;
        SDL_Renderer* renderer;

        Screen();   ///c-tor
    public:
        ~Screen();                  ///d-tor
        static void init();         /// initializes SDL and creates screen instance
        static Screen& inst();      ///function that returns reference to singleton object
        inline void update(const std::vector<Texture*>& textures);

        //################ GETTERS ##################

        uint16_t get_width() const;
        uint16_t get_height() const;
        SDL_Renderer* get_rend();   ///renderer getter
};

void Screen::update(const std::vector<Texture*>& textures) {
    //clearing renderer
    SDL_RenderClear(renderer);
    //getting vector with textures to render and iterating through it
    for (auto& it : textures)
        it->draw(renderer); //copying textures to renderer
    //rendering all
    SDL_RenderPresent(renderer);
}

#endif // SCREEN_H_INCLUDED
