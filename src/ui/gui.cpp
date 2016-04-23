#include "gui.h"
#include <vector>
#include <algorithm>
#include "screen.h"
#include "widget.h"

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
        if (widget == nullptr)
            continue;
        auto widget_textures = widget->get_textures();
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
        if (widget == nullptr)
            continue;
        for (auto& texture : widget->get_textures())
            all_textures.push_back(texture);
    }
    return all_textures;
}

void GUI::set_widgets(std::vector<Widget*> widgets) {
    this->widgets.clear();
    this->widgets = widgets;
}

void GUI::add_widget(Widget* widget) {
    widgets.push_back(widget);
}

void GUI::remove_widget(Widget* widget) {
    for (size_t i = 0; i < widgets.size(); i++) {
        if (widgets[i] == widget) {
            widgets.erase(widgets.begin() + i);
            return;
        }
    }
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
    buffer.push_back(Texture::ptr(new Texture(path, x, y, width, height)));
}

void GUI::flush() {
    textures.clear();
    textures = std::move(buffer);
    buffer.clear();
}

void GUI::clear_textures() {
    textures.clear();
}
