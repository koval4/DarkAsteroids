#include "label.h"
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "gui.h"

//###################### STATIC VARIABLES ##########################

std::string Label::default_font_path = "";          /// default font for text in label
int Label::default_font_size = 0;                   /// default size of font
SDL_Color Label::default_font_color = {0, 0, 0, 0}; /// default color of text in label

//###################### CONSTRUCTORS ##########################

/**
* @brief Label default constructor, makes empty label
*/
Label::Label() : Widget() {

}

/**
* @brief Label constructor creates label with passed text at passed position
* @param text -- text inside label
* @param form -- size and position of label
*/
Label::Label(std::string text, SDL_Rect form)
    : Widget("", form)
    , text(text)
    , font_color(default_font_color) {
    font = TTF_OpenFont(default_font_path.c_str(), default_font_size);
    txtr = Texture::ptr(new Texture(text, font, font_color, form.x, form.y));
}

//###################### DESTRUCTOR ###########################

Label::~Label() {
    TTF_CloseFont(font);
}

//###################### GETTERS ########################

std::vector<Texture*> Label::get_textures() const {
    return {txtr.get()};
}

std::vector<const Listener*> Label::get_listeners() const {
    return {};
}

//###################### SETTERS ######################

void Label::set_font(std::string font_path, int font_size) {
    TTF_CloseFont(font);
    TTF_OpenFont(font_path.c_str(), font_size);
    txtr = Texture::ptr(new Texture(text, font, font_color, form.x, form.y));
    GUI::inst().update();
}

void Label::set_font_color(SDL_Color color) {
    font_color = color;
    txtr = Texture::ptr(new Texture(text, font, font_color, form.x, form.y));
    GUI::inst().update();
}

void Label::set_text(std::string text) {
    txtr = Texture::ptr(new Texture(text, font, font_color, form.x, form.y));
    GUI::inst().update();
}

void Label::set_default_font(std::string path) {
    default_font_path = path;
}

void Label::set_default_font_size(int size) {
    default_font_size = size;
}

void Label::set_default_font_color(SDL_Color color) {
    default_font_color = color;
}
