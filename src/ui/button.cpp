#include "button.h"
#include <string>
#include <vector>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "gui.h"
#include "listener.h"

//################ STATIC VARIABLES DECLARATION ######################

std::string Button::default_back_path = "";             /// path to texture that will be used as default background
std::string Button::default_font_path = "";             /// path to font that will be used as default
int Button::default_font_size = 0;                      /// size of default font
SDL_Color Button::default_font_color = {0, 0, 0, 0};    /// default color of text

//###################### CONSTRUCTORS #########################

/**
* @brief Button -- default constructor, doing nothing
*/
Button::Button()
    : Widget()
    , listener(SDL_MOUSEBUTTONDOWN) {}

/**
* @brief Button -- constructor to make complete button
* @param txtr -- contains text to render or path to icon; will be placed in center of button
* @param form  -- size and position of button
* @param margin -- margins inside button
* @param content -- marks content that will be placed in button (text, icon), defaulted to text
*/
Button::Button(std::string txtr, SDL_Rect form, Margin margin, Button::Content content)
    : Widget(default_back_path, form , margin)
    , font_color(default_font_color)
    , listener(SDL_MOUSEBUTTONDOWN) {
    font = TTF_OpenFont(default_font_path.c_str(), default_font_size);
    // creating texture with text if content marked as text
    if (content == text) {
        this->txtr = Texture::ptr(new Texture(txtr, font, font_color, form.x + margin.left, form.y + margin.top));
    // else creating texture with image if content marked as icon
    } else if (content == icon) {
        this->txtr = Texture::ptr(new Texture(txtr, form.x + margin.left, form.y + margin.top));
    }
    // centering content of button
    SDL_Rect txtr_form = this->txtr->get_form();
    int centered_w = txtr_form.w;
    if (centered_w > (form.w - margin.left - margin.right))
        centered_w = (form.w - margin.left - margin.right);
    int centered_h = txtr_form.h;
    if (centered_h > (form.h - margin.top - margin.bottom))
        centered_h = (form.h - margin.top - margin.bottom);
    int centered_x = form.x + ((form.w - margin.left - margin.right)/2 - (centered_w / 2));
    int centered_y = form.y + ((form.h - margin.top - margin.bottom)/2 - (centered_h / 2));
    this->txtr->set_form({centered_x, centered_y, centered_w, centered_h});

    listener.run([this] (SDL_Event& event) -> void {
        // if clicked this button and on_click_handler is defined
        if (is_point_in({event.button.x, event.button.y}) && on_click_handler) {
            on_click_handler();
        }
    });
}

//##################### DESTRUCTOR ########################

Button::~Button() {
    TTF_CloseFont(font);
}

//###################### GETTERS #########################

std::vector<Texture*> Button::get_textures() const {
    return {background.get(), txtr.get()};
}

//######################### SETTERS ############################

void Button::set_font(std::string font_path, int font_size) {
    TTF_CloseFont(font);
    font = TTF_OpenFont(font_path.c_str(), font_size);
    GUI::inst().update();
}

//-------------------default-setters------------------------

void Button::set_default_background(std::string path) {
    default_back_path = path;
}

void Button::set_default_font(std::string path) {
    default_font_path = path;
}

void Button::set_default_font_size(int size) {
    default_font_size = size;
}

void Button::set_default_font_color(SDL_Color color) {
    default_font_color = color;
}

void Button::on_click(std::function<void ()> handler) {
    on_click_handler = handler;
}
