#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "gui.h"
#include "common.h"
#include "listbox.h"

//##################### STATIC VARIABLES ############################

std::string Listbox::default_back_txtr = "";            /// path to texture used as default for background
std::string Listbox::default_item_back_txtr = "";       /// path to texture used as default for list items background
std::string Listbox::default_font_path = "";            /// path to font used as default
int Listbox::default_font_size = 0;                     /// size used as default parameter for font size
SDL_Color Listbox::default_font_color = {0, 0, 0, 0};   /// default color used for text in list items
uint16_t Listbox::default_item_height = 0;              /// default height in pixels used for list item form
uint16_t Listbox::default_spacing = 0;                  /// default spacing between visible items
Padding Listbox::default_item_margin = {};               /// default margins used in items

//########################### LIST ITEM ################################


/**
* @brief List_Item -- constructor of single visible item
* @param text -- text in item, used for text_txtr creation
* @param back_path -- path to background texture
* @param form -- size and position of item
* @param margin -- margins in item
*/
Listbox::List_Item::List_Item(Listbox* parent, std::string text, SDL_Rect form)
    : text(text)
    , form(form)
    , margin(parent->item_margin)
    , listener(SDL_MOUSEBUTTONDOWN) {
    text_txtr = Texture::ptr(new Texture(text, parent->items_font, parent->items_color, form.x + parent->item_margin.left, form.y + parent->item_margin.top));
    background = Texture::ptr(new Texture(parent->item_background, form.x, form.y, form.w, form.h));

    listener.run([this, parent] (SDL_Event& event) -> void {
        //checking if click was inside list item
        if (event.button.x < this->form.x || event.button.x > (this->form.x + this->form.w))
            return;
        if (event.button.y < this->form.y || event.button.y > (this->form.y + this->form.h))
            return;
        // check if item_clicked handler is defined
        if (parent->item_clicked_handler) {
            event.type = 0;
            parent->item_clicked_handler(this->text);
        }
    });
}

//###################### PRIVATE FUNCTIONS ################################

/**
 * @brief Listbox::draw_items -- makes visible_items from items
 */
void Listbox::draw_items() {
    uint16_t max_items_visible = (form.h - margin.top - margin.bottom) / (item_height + spacing);
    visible_items.clear();
    SDL_Rect item_form = {form.x + margin.left, form.y + margin.top
                          , form.w - margin.left - margin.right
                          , item_height};
    for (uint16_t i = 0; i < max_items_visible && i < items.size(); i++) {
        visible_items.push_back(List_Item::ptr(new List_Item(this, items[i], item_form)));
        item_form.y += item_height + spacing;
    }
    GUI::inst().update();
}

//########################## CONSTRUCTORS ##################################


/**
* @brief Listbox -- default constructor
*/
Listbox::Listbox() : Widget() {}


/**
* @brief Listbox -- constructor for complete listbox construction without items
* @param form -- size and position of listbox
* @param margin -- margins inside listbox, defaulted to no margins
* @param items_color -- color used for items rendering, defaulted to black
*/
Listbox::Listbox(SDL_Rect form, Padding margin, uint16_t item_height)
    : Widget(default_back_txtr, form, margin)
    , pos(0)
    , item_height(item_height)
    , spacing(default_spacing)
    , items_color(default_font_color)
    , item_background(default_item_back_txtr)
    , item_margin(default_item_margin) {
    items_font = TTF_OpenFont(default_font_path.c_str(), default_font_size);
}

//########################### DESTRUCTOR ####################################

Listbox::~Listbox() {
    TTF_CloseFont(items_font);
}

//############################## GETTERS ##################################

uint16_t Listbox::get_items_count() const {
    return items.size();
}

uint16_t Listbox::get_visible_items_count() const {
    return visible_items.size();
}

std::vector<Texture*> Listbox::get_textures() const {
    std::vector<Texture*> textures = {background.get()};
    for (auto& it : visible_items) {
        textures.push_back(it->background.get());
        textures.push_back(it->text_txtr.get());
    }
    return textures;
}

//######################### SETTERS #############################

void Listbox::set_items(std::vector<std::string> items) {
    this->items = items;
    draw_items();
}

void Listbox::set_font(std::string font_path, int font_size) {
    TTF_CloseFont(items_font);
    TTF_OpenFont(font_path.c_str(), font_size);
    draw_items();
}

void Listbox::set_color(SDL_Color colour) {
    this->items_color = colour;
    draw_items();
}

void Listbox::set_item_height(uint16_t item_height) {
    this->item_height = item_height;
    draw_items();
}

void Listbox::set_spacing(uint16_t spacing) {
    this->spacing = spacing;
    draw_items();
}

void Listbox::set_scroll_pos(uint16_t scroll_pos) {
    this->pos = scroll_pos;
    draw_items();
}

void Listbox::set_default_background(std::string path) {
    default_back_txtr = path;
}

void Listbox::set_default_item_back(std::string path) {
    default_item_back_txtr = path;
}

void Listbox::set_default_font(std::string path) {
    default_font_path = path;
}

void Listbox::set_default_font_size(int font_size) {
    default_font_size = font_size;
}

void Listbox::set_default_font_color(SDL_Color color) {
    default_font_color = color;
}

void Listbox::set_default_item_height(uint16_t item_height) {
    default_item_height = item_height;
}

void Listbox::set_default_spacing(uint16_t spacing) {
    default_spacing = spacing;
}

void Listbox::set_default_item_margin(Padding item_margin) {
    default_item_margin = item_margin;
}

//###################### ITEMS OPERATIONS ############################

/**
 * @brief Listbox::add_item adds single item in textbox
 * @param item -- item that will be added to listbox
 */
void Listbox::add_item(std::string item) {
    items.push_back(item);
    draw_items();
}

/**
 * @brief Listbox::remove_item removes single item from textbox
 * @param item -- item that will be removed from listbox
 */
void Listbox::remove_item(std::string item) {
    for (uint16_t i = 0; i < items.size(); i++) {
        if (items[i] == item) {
            items.erase(items.begin() + i);
            break;
        }
    }
    draw_items();
}

/**
 * @brief Listbox::clear_items removes all items from listbox
 */
void Listbox::clear_items() {
    items.clear();
    visible_items.clear();
    draw_items();
}

//######################### EVENTS ##############################

void Listbox::on_item_clicked(std::function<void (std::string)> handler) {
    item_clicked_handler = handler;
}
