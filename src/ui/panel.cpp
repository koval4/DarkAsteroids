#include "panel.h"
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "gui.h"

//#################### STATIC VARIABLES ####################

std::string Panel::default_back_path = "";  /// path to texture used as default for background

//#################### CONSTRUCTORS ###################

/**
* @brief Panel -- default constructor, doing nothing
*/
Panel::Panel() : Widget() {}

/**
* @brief Panel -- constructor that creates just panel widthout widgets
* @param access_name -- name used for identification of element
* @param form -- size and position of panel
* @param margin -- margins inside panel
*/
Panel::Panel(std::string access_name,
             SDL_Rect form,
             Padding padding)
    : Widget(access_name, default_back_path, form, padding) {}

//###################### DESTRUCTOR ########################

Panel::~Panel() {}

//##################### GETTERS ########################

/**
 * @brief Panel::get_textures returns textures of all stored widgets
 * @return background + widgets textures
 */
std::vector<Texture*> Panel::get_textures() const {
    std::vector<Texture*> textures = {background.get()};
    for (auto& it_widget : widgets)
        for (auto& it_texture : it_widget->get_textures())
            textures.push_back(it_texture);
    return textures;
}

std::vector<const Listener*> Panel::get_listeners() const {
    std::vector<const Listener*> listeners;
    for (auto& widget : widgets) {
        auto w_listeners = widget->get_listeners();
        listeners.reserve(listeners.capacity() + w_listeners.size());
        listeners.insert(listeners.end(), w_listeners.begin(), w_listeners.end());
    }
    return listeners;
}

//##################### SETTERS #########################

void Panel::set_widgets(std::vector<Widget::ptr> widgets) {
    this->widgets = widgets;
}

void Panel::set_default_background(std::string path) {
    default_back_path = path;
}

//################# WIDGETS OPERATIONS #####################

void Panel::add_widget(const Widget::ptr widget) {
    widgets.push_back(widget);
}

void Panel::remove_widget(const Widget::ptr widget) {
    auto it = widgets.begin();
    while (it != widgets.end()) {
        if (widget == *it) {
            widgets.erase(it);
            return;
        } else it++;
    }
}

void Panel::clear_widgets() {
    widgets.clear();
}
