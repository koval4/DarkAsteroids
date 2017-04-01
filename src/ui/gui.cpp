#include "ui/gui.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
#include "ui/screen.h"
#include "ui/widget.h"

GUI::GUI() {

}

GUI& GUI::inst() {
    static GUI instance;
    return instance;
}

void GUI::update() {
    std::vector<Texture*> all_textures;
    all_textures.reserve(textures.size() + widgets.size());
    for (auto& texture : textures) {
        if (texture)
            all_textures.push_back(texture.get());
    }
    for (auto& widget : widgets) {
        if (!widget.second)
            continue;
        auto widget_textures = widget.second->get_textures();
        std::copy(widget_textures.begin(), widget_textures.end(), std::back_inserter(all_textures));
    }
    Screen::inst().update(all_textures);
}

std::vector<Texture*> GUI::get_textures() {
    std::vector<Texture*> all_textures;
    for (auto& texture : textures) {
        if (texture)
            all_textures.push_back(texture.get());
    }
    for (auto& widget : widgets) {
        if (!widget.second)
            continue;
        for (auto& texture : widget.second->get_textures())
            all_textures.push_back(texture);
    }
    return all_textures;
}

Widget::ptr GUI::get_widget(std::string access_name) const {
    // return nullptr if widget doesn't exists
    if (widgets.find(access_name) == widgets.end())
        return nullptr;
    return widgets.at(access_name);
}

//##################### WIDGETS OPERATIONS #####################

void GUI::remove_widget(std::string access_name) {
    widgets.erase(access_name);
}

void GUI::clear_widgets() {
    widgets.clear();
}

//##################### TEXTURES OPERATIONS #####################

void GUI::add_texture( std::string path
                     , uint16_t x
                     , uint16_t y
                     , uint16_t width
                     , uint16_t height
                     ) {
    buffer.push_back(std::make_unique<Texture>(path, x, y, width, height));
}

void GUI::flush() {
    textures.clear();
    textures = std::move(buffer);
    buffer.clear();
}

void GUI::clear_textures() {
    textures.clear();
}
