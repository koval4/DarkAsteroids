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
Panel::Panel() : Widget() {

}

/**
* @brief Panel -- constructor that creates just panel widthout widgets
* @param form -- size and position of panel
* @param margin -- margins inside panel
*/
Panel::Panel(SDL_Rect form, Padding margin) : Widget(default_back_path, form, margin) {

}

//###################### DESTRUCTOR ########################

Panel::~Panel() {

}

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

//##################### SETTERS #########################

void Panel::set_widgets(std::vector<Widget*> widgets) {
    this->widgets = widgets;
    GUI::inst().update();
}

void Panel::set_default_background(std::string path) {
    default_back_path = path;
}

//################# WIDGETS OPERATIONS #####################

void Panel::add_widget(Widget* widget) {
    widgets.push_back(widget);
    GUI::inst().update();
}

void Panel::remove_widget(Widget* widget) {
    auto it = widgets.begin();
    while (it != widgets.end()) {
        if (widget == *it) {
            widgets.erase(it);
            GUI::inst().update();
            return;
        } else it++;
    }
}

void Panel::clear_widgets() {
    widgets.clear();
    GUI::inst().update();
}
