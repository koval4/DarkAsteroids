#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <iostream>
#include "ui/screen.h"
#include "ui/texture.h"

uint16_t Texture::obj_counter = 0;

///====================CONSTRUCTORS================================

/** base c-tor, creates empty texture*/
Texture::Texture() {
    obj_counter++;
    texture = nullptr;
    path = "";
    form = {0, 0, 0, 0};
    part_to_render = {0, 0, 0, 0};
}

//------------------texture---------------------------------
/** c-tor used for texture creation
    path -- path to file with texture
    x, y, w, h -- position and size of texture
    w, h defaulted to 0 (means they'll use texture w and h)*/
Texture::Texture(std::string path,
                 uint16_t x,
                 uint16_t y,
                 uint16_t w,
                 uint16_t h
                 )
    : form({x, y, w, h})
    , path(path) {
    obj_counter++;
    if (path == "") {
        texture = nullptr;
        form = {};
        part_to_render = {};
        width = 0;
        height = 0;
        return;
    }
    //loading surface
    make_texture(IMG_Load(path.c_str()));
}

//---------------------------text---------------------------------
/** c-tor used for creation text using TTF
    text -- text, used for texture cration
    font -- contains font and size of text
    color -- color of text
    x, y, w, h -- position and size of texture
    w, h defaulted to 0 (means they'll use texture w and h)*/
Texture::Texture(std::string text, TTF_Font* font, SDL_Color text_color,
                 uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    : form( {
    x, y, w, h
}) {
    obj_counter++;
    path = "text";
    //making surface with text
    make_texture(TTF_RenderText_Solid(font, text.c_str(), text_color));
}

//--------------------wrapped-text----------------------------------
/** c-tor used for creation wrapped text using TTF
    text -- text, used for texture cration
    font -- contains font and size of text
    color -- color of text
    x, y, w, h -- position and size of texture
    w, h defaulted to 0 (means they'll use texture w and h)*/
Texture::Texture(std::string text,
                 uint16_t wrap_width,
                 TTF_Font* font,
                 SDL_Color text_color,
                 uint16_t x,
                 uint16_t y
                 )
    : form({x, y, 0, 0}) {
    obj_counter++;
    path = "text";
    //making surface with text
    make_texture(TTF_RenderText_Blended_Wrapped(font, text.c_str(), text_color, wrap_width));
}

/**
 * @brief Texture::Texture -- copy constructor
 * @param obj -- original object
 */
Texture::Texture(const Texture& obj)
    : texture(obj.texture)
    , form(obj.form)
    , part_to_render(obj.part_to_render)
    , path(obj.path) {
    //+1 object using this texture
    obj_counter++;
}

///====================DESTRUCTOR==================================

/** d-tor, removes texture from loaded_txtrs if texture unused*/
Texture::~Texture() {
    obj_counter--;
    SDL_DestroyTexture(texture);
}

//################# PRIVATE FUNCTIONS ####################

void Texture::make_texture(SDL_Surface* surface) {
    if (surface != nullptr) {
        //if surface made => converting it to texture
        texture = SDL_CreateTextureFromSurface(Screen::inst().get_rend(), surface);
        if (form.w == 0 || form.h == 0) {
            form.w = surface->w;
            form.h = surface->h;
        }
    } else {
        //if error happened =>
        std::cout << "Error! Unable to make texture!" << std::endl;
    }
    //getting texture width and height
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    // setting to render full texture
    part_to_render = {0, 0, surface->w, surface->h};
    SDL_FreeSurface(surface);
}

//#################### GETTERS ##############################

int Texture::get_width() const {
    return width;
}

int Texture::get_height() const {
    return height;
}

SDL_Rect Texture::get_form() const {
    return form;
}

SDL_Rect Texture::get_part_to_render() const {
    return part_to_render;
}

//###################### SETTERS #################################

void Texture::set_form(SDL_Rect form) {
    this->form = form;
}

void Texture::set_part_to_render(SDL_Rect part_to_render) {
    this->part_to_render = part_to_render;
}

///=================RENDERER=OPERATIONS========================

/** copies texture to renderer*/
void Texture::draw(SDL_Renderer* renderer) {
    if (texture && renderer)
        SDL_RenderCopy(renderer, texture, &part_to_render, &form);
}
