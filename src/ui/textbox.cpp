#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "common.h"
#include "gui.h"
#include "textbox.h"

//################## STATIC VARIABLES ########################

std::string Textbox::default_back_txtr = "";            /// path to texture used as default for background
std::string Textbox::default_font_path = "";            /// path to font used as default
int Textbox::default_font_size = 0;                     /// size used as default parameter for font size
SDL_Color Textbox::default_font_color = {0, 0, 0, 0};   /// font color used as a default

//###################### CONSTRUCTORS ##########################

/**
* @brief Textbox -- default constructor, creates empty textbox
*/
Textbox::Textbox() : Widget(), Scrollable() {
    font = nullptr;
    text_txtr = Texture::ptr(nullptr);
}

/**
* @brief Textbox -- constuctor used to make normal textbox
* @param form -- size and position of textbox
* @param margin -- margins inside textbox, defaulted to no margins
* @param multiline -- does text displays in multiple lines or in single line
* @param font_color -- color used for text rendering, defaulted to black
* @param font_size -- size of font used to render text, uses default static value
*/
Textbox::Textbox(SDL_Rect form, Padding margin, bool multiline, SDL_Color font_color, uint16_t font_size)
    : Widget(default_back_txtr, form, margin), Scrollable()
    , multiline(multiline), font_color(font_color) {
    pos = 0;
    font = TTF_OpenFont(default_font_path.c_str(), font_size);
    text_txtr = Texture::ptr(nullptr);
}

//######################## DESTRUCTOR ##############################

Textbox::~Textbox() {
    TTF_CloseFont(font);
}

//#################### PRIVATE FUNCTIONS ##########################

/**
 * @brief Textbox::text_to_texture -- converts text from std::string to Texture
 */
void Textbox::text_to_texture() {
    if (multiline) {
        //creating new text texture with wrapping width of (form - margins)
        text_txtr = Texture::ptr(new Texture(text, (form.w - margin.left - margin.right)
                                             , font, font_color
                                             , form.x + margin.left, form.y + margin.top));
    } else {
        //creating new one-line text texture
        text_txtr = Texture::ptr(new Texture(text, font, font_color
                                             , form.x + margin.left, form.y + margin.top));
    }
    redraw_text();
}

/**
 * @brief Textbox::scroll_to_end sets pos to text texture height - (form - margins)
 */
void Textbox::scroll_to_end() {
    if (text_txtr->get_height() <= (form.h - margin.bottom - margin.top)) {
        pos = 0;
        return;
    }
    pos = text_txtr->get_height() - (form.h - margin.bottom - margin.top);
    if (slidebar != nullptr)
        slidebar->set_slider_pos(pos / (text_txtr->get_height()/100));
    GUI::inst().update();
}

/**
 * @brief Textbox::redraw_text changes part of text to render
 */
void Textbox::redraw_text() {
    SDL_Rect text_form = text_txtr->get_form();
    text_form.w = text_txtr->get_width();
    text_form.h = text_txtr->get_height();
    if (multiline) {
        //if texture is bigger than (form - margins) =>
        if (text_form.h > (form.h - margin.bottom - margin.top)) {
            //rendering text that fits into (form - margins) from pos
            text_txtr->set_part_to_render({0, pos, text_form.w
                                          , (form.h - margin.top - margin.bottom)
                                          });
            text_txtr->set_form({text_form.x, text_form.y, text_form.w
                                , (form.h - margin.top - margin.bottom)
                                });
        //else render full texture
        } else text_txtr->set_part_to_render({0, 0, text_form.w, text_form.h});
    } else {
        //if texture is bigger than (form - margins) =>
        if (text_form.w > (form.w - margin.left - margin.right)) {
            //rendering all text from end that fits into (form -margins)
            text_txtr->set_part_to_render({(text_form.w - form.w - margin.left - margin.right)
                                           , 0, (form.w - margin.left - margin.right), text_form.h
                                          });
            text_txtr->set_form({text_form.x, text_form.y
                                 , (form.w - margin.left - margin.right), text_form.h});
        //else render full texture
        } else text_txtr->set_part_to_render({0, 0, text_form.w, text_form.h});
    }
    update_bar();
    GUI::inst().update();
}

//################# SCROLLABLE #####################

void Textbox::update_bar() {
    if (slidebar == nullptr)
        return;
    if (!text_txtr) {
        slidebar->set_bar_size(1);
        slidebar->set_slider_size(1);
    }
    int text_height = text_txtr->get_height();
    if (text_height < 1)
        text_height = 1;
    int visible_height = text_txtr->get_part_to_render().h;
    if (visible_height < 1)
        visible_height = 1;
    // percent of visible text
    int visible_percent = (visible_height * 100)/text_height;
    if (visible_percent < 1)
        visible_percent = 1;
    //setting sizes in percent
    slidebar->set_bar_size(100);
    slidebar->set_slider_size(visible_percent);
    GUI::inst().update();
}

//################# GETTERS ####################

std::string Textbox::get_text() const {
    return text;
}

uint16_t Textbox::get_height() const {
    return form.h;
}

uint16_t Textbox::get_text_height() const {
    return text_txtr->get_part_to_render().h;
}

/**
 * @brief Textbox::get_textures used for widget rendering
 * @return all textures in textbox
 */
std::vector<Texture*> Textbox::get_textures() const {
    std::vector<Texture*> textures = {background.get()};
    if (text_txtr)
        textures.push_back(text_txtr.get());
    return textures;
}

std::vector<const Listener*> Textbox::get_listeners() const {
    return {};
}

//################## SETTERS #####################

void Textbox::set_text(std::string text) {
    this->text = text;
    text_to_texture();
}

void Textbox::set_multiline(bool multiline) {
    this->multiline = multiline;
    text_to_texture();
}

void Textbox::set_font(std::string font_path, int font_size) {
    TTF_CloseFont(font);
    font = TTF_OpenFont(font_path.c_str(), font_size);
    text_to_texture();
}

void Textbox::set_font_color(SDL_Color font_color) {
    this->font_color = font_color;
    text_to_texture();
}

/**
 * @brief Textbox::set_scroll_pos sets position in text to begin rendering from it
 * @param pos -- new position to start text rendering from
 */
void Textbox::set_scroll_pos(uint16_t pos) {
    this->pos = pos;
    redraw_text();
}

void Textbox::set_default_back_path(std::string path) {
    default_back_txtr = path;
}

void Textbox::set_default_font_path(std::string path) {
    default_font_path = path;
}

void Textbox::set_default_font_size(int size) {
    default_font_size = size;
}

void Textbox::set_default_font_color(SDL_Color color) {
    default_font_color = color;
}

//################# TEXT OPERATIONS #####################

/**
 * @brief Textbox::add_text wrapper of this->text += text; redraws text into textbox
 * @param text -- new text added to existing one
 */
void Textbox::add_text(std::string text) {
    this->text += text;
    text_to_texture();
    scroll_to_end();
    redraw_text();
}

/**
 * @brief Textbox::clear_text clears text string and texture
 */
void Textbox::clear_text() {
    this->text.clear();
    redraw_text();
}

//##################### SCROLLABLE ###########################

void Textbox::update_pos() {
    if (slidebar == nullptr)
        return;
    pos = slidebar->get_pos() * (text_txtr->get_height()/100);
    redraw_text();
}

