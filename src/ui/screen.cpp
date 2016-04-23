#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "common.h"
#include "texture.h"
#include "screen.h"

Screen::Screen(){
    window = nullptr;
    renderer = nullptr;
    //hardcoded, should change it
    screen_width = 800;
    screen_height = 600;
    //initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_AUDIO) != 0){
        log_file << "Unable to initialize SDL!" << std::endl;
        std::cout << "Unable to initialize SDL!" << std::endl;
        return;
    }
    //creating window
    window = SDL_CreateWindow("Dark Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        log_file << "Unable to create window." << std::endl;
        std::cout << "Unable to create window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        log_file << "Unable to create renderer." << std::endl;
        std::cout << "Unable to create renderer." << std::endl;
        return;
    }

    //initializing SDL extensions

    if (!IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG)){
        log_file << "Unable to initialize SDL_img!" << std::endl;
        std::cout << "Unable to initialize SDL_img!" << std::endl;
    }

    /*if (!Mix_Init(MIX_INIT_FLAC)){
        log_file << "Unable to initialize SDL_mixer! " << Mix_GetError() << std::endl;
        std::cout << "Unable to initialize SDL_mixer! " << Mix_GetError() << std::endl;
        return;
    }*/

    if (!TTF_Init()){
        log_file << "Unable to initialize SDL_ttf! " << TTF_GetError() << std::endl;
        std::cout << "Unable to initialize SDL_ttf! " << TTF_GetError() << std::endl;
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

///d-tor
Screen::~Screen(){
    //destroying pointers
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Screen::init() {
    inst(); // call to create an instance of screen and to initialize SDL
}

///function that returns reference to singleton object
Screen& Screen::inst(){
    static Screen instance;
    return instance;
}

///renderer getter
SDL_Renderer* Screen::get_rend(){
    return renderer;
}

uint16_t Screen::get_width() const {
    return screen_width;
}

uint16_t Screen::get_height() const {
    return screen_height;
}
