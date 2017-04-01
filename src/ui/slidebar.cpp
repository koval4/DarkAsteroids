#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "ui/widget.h"
#include "ui/texture.h"
#include "ui/common.h"
#include "ui/slidebar.h"
#include "ui/gui.h"

std::string Slidebar::default_back_path = "";
std::string Slidebar::default_slider_path = "";
std::string Slidebar::default_up_path = "";
std::string Slidebar::default_down_path = "";

//####################### SLIDER STRUCT CONSTRUCTORS ############################

/**
* @brief Slider -- default c-tor
*/
Slidebar::Slider::Slider() {
}

/**
* @brief Slider -- constructor that uses Texture object to set slider data
* @param texture -- texture object used to set view of slider
* @param size -- how much lines represents slider
* @param pos -- position which represents top of slider
*/
Slidebar::Slider::Slider(Texture::ptr texture, uint16_t size, uint16_t pos)
    : texture(std::move(texture)), size(size), pos(pos) {

}

/**
* @brief Slider -- constructor that uses path to texture for texture construction
* @param txtr_path -- path to file with texture
* @param size -- how much lines represents slider
* @param pos -- position which represents top of slider
*/
Slidebar::Slider::Slider(std::string txtr_path, SDL_Rect form, uint16_t size, uint16_t pos)
    : texture(new Texture(txtr_path, form.x, form.y, form.w, form.h))
    , size(size), pos(pos) {

}

/**
* @brief set_pos -- changes position of slider
* @param parent -- parent Slidebar of this slider
*/
void Slidebar::Slider::set_pos(Slidebar* parent) {
    uint16_t item_h = parent->form.h / parent->size;
    uint16_t slider_y = parent->form.y + (pos * item_h);
    SDL_Rect slider_form = texture->get_form();
    slider_form.y = slider_y;
    texture->set_form(slider_form);
    GUI::inst().update();
}

//############################ CONSTRUCTORS #######################################

/**
* @brief Slidebar -- default constructor
*/
Slidebar::Slidebar()
    : Widget()
    , wheel_listener(SDL_MOUSEWHEEL) {}

/**
* @brief Slidebar -- constuctor to set complete Slidebar
* @param form -- size and position ofslidebar in pixels
* @param lines_total -- how much lines represents full slidebar
* @param lines_on_screen -- how much lines represents slider
*/
Slidebar::Slidebar(std::string access_name, SDL_Rect form, uint16_t lines_total, uint16_t lines_on_screen)
    : Widget(access_name, default_back_path, form)
    , size(lines_total)
    , slider(default_slider_path
        , {form.x, form.y, form.w, form.h / lines_total * lines_on_screen}
        , lines_on_screen)
    , scrollable(nullptr)
    , wheel_listener(SDL_MOUSEWHEEL) {
    uint16_t btn_side_len = (form.w > form.h) ? form.h : form.w;
    SDL_Rect up_form = {form.x, form.y, btn_side_len, btn_side_len};
    up = std::make_unique<Button>("", default_up_path, up_form, Padding(), Button::icon);
    uint16_t down_y = form.y + form.h - btn_side_len;
    SDL_Rect down_form = {form.x, down_y, btn_side_len, btn_side_len};
    down = std::make_unique<Button>("", default_down_path, down_form, Padding(), Button::icon);
    form = {form.x, form.y - btn_side_len, form.w, form.h - (2*btn_side_len)};

    //setting listeners on up and down buttons
    up->on_click([this] () -> void {
        if (slider.pos > 0) {
            slider.pos--;
            slider.set_pos(this);
            if (scrollable != nullptr)
                scrollable->update_pos();
        }
    });
    down->on_click([this] () -> void {
        if (slider.pos < (this->size - slider.size)) {
            slider.pos++;
            slider.set_pos(this);
            if (scrollable != nullptr)
                scrollable->update_pos();
        }
    });
    wheel_listener.set_handler([this] (SDL_Event& event) -> void {
        // ERROR DOWN HERE IN CONDITION
        int mouse_x = 0;
        int mouse_y = 0;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        if (!(is_point_in({mouse_x, mouse_y})
              /*|| scrollable->is_point_in({event.button.x, event.button.y})*/))
            return;
        event.type = 0;
        if (event.wheel.y > 0) {
            if (slider.pos > 0) {
                slider.pos--;
                slider.set_pos(this);
                if (scrollable != nullptr)
                    scrollable->update_pos();
            }
        } else if (event.wheel.y < 0) {
            if (slider.pos < (this->size - slider.size)) {
                slider.pos++;
                slider.set_pos(this);
                if (scrollable != nullptr)
                    scrollable->update_pos();
            }
        }
    });
}

//############################ DESTRUCTOR ################################

Slidebar::~Slidebar() {

}

//########################### GETTERS ################################

std::vector<Texture*> Slidebar::get_textures() const {
    std::vector<Texture*> textures = {background.get(), slider.texture.get()};
    for (auto& it : up->get_textures())
        textures.push_back(it);
    for (auto& it : down->get_textures())
        textures.push_back(it);
    return textures;
}

std::vector<const Listener*> Slidebar::get_listeners() const {
    std::vector<const Listener*> listeners { &wheel_listener };
    listeners.reserve(4);
    auto btn_listeners = up->get_listeners();
    listeners.insert(listeners.end(), btn_listeners.begin(), btn_listeners.end());
    return listeners;
}

uint16_t Slidebar::get_pos() const {
    return slider.pos;
}

//########################## SETTERS ##############################

void Slidebar::set_bar_size(uint16_t size) {
    this->size = size;
    SDL_Rect slider_form = slider.texture->get_form();
    if (this->size > slider.size) {
        slider_form.h = (float)form.h * slider.size / this->size;
        if (slider_form.h < 10)
            slider_form.h = 10;
    } else slider_form.h = this->form.h;
    slider.texture->set_form(slider_form);
    GUI::inst().update();
}

void Slidebar::set_slider_size(uint16_t size) {
    slider.size = size;
    SDL_Rect slider_form = slider.texture->get_form();
    if (this->size > slider.size) {
        slider_form.h = (float)form.h * slider.size / this->size;
        if (slider_form.h < 10)
            slider_form.h = 10;
    } else slider_form.h = this->form.h;
    slider.texture->set_form(slider_form);
    GUI::inst().update();
}

void Slidebar::set_slider_pos(uint16_t pos) {
    slider.pos = pos;
    slider.set_pos(this);
    GUI::inst().update();
}

void Slidebar::set_default_back_txtr(std::string path) {
    default_back_path = path;
}

void Slidebar::set_default_slider_txtr(std::string path) {
    default_slider_path = path;
}

void Slidebar::set_default_up_txtr(std::string path) {
    default_up_path = path;
}

void Slidebar::set_default_down_txtr(std::string path) {
    default_down_path = path;
}

//################### SCROLLABLE ##########################

void Slidebar::bind_to(Scrollable* scrollable) {
    this->scrollable = scrollable;
    scrollable->make_binded_to(this);
}

