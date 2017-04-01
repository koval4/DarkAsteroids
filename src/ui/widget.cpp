#include <vector>
#include <string>
#include <typeinfo>
#include <SDL2/SDL.h>
#include "ui/texture.h"
#include "ui/common.h"
#include "ui/widget.h"

uint16_t Widget::obj_counter = 0;

///================CONSTRUCTORS=====================

Widget::Widget() {
    obj_counter++;
    form = {};
    padding = {};
    visible = false;
    enabled = false;
}

Widget::Widget(std::string access_name,
               Texture::ptr background,
               SDL_Rect form,
               Padding margin)
    : access_name(access_name)
    , background(std::move(background))
    , form(form)
    , padding(margin)
    , visible(true)
    , enabled(true) {
    obj_counter++;
}

Widget::Widget(std::string access_name,
               std::string back_path,
               SDL_Rect form,
               Padding margin)
    : access_name(access_name)
    , form(form)
    , padding(margin)
    , visible(true)
    , enabled(true)
    , background(std::make_unique<Texture>(back_path, form.x, form.y, form.w, form.h)) {
    obj_counter++;
}

///===================DESTRUCTOR========================

Widget::~Widget() {
    obj_counter--;
}

std::string Widget::get_access_name() const {
    return access_name;
}

//################ PRIVATE FUNCTIONS ######################

bool Widget::is_point_in(SDL_Point point) {
    if (point.x < form.x || point.x > (form.x + form.w))
        return false;
    if (point.y < form.y || point.y > (form.y + form.h))
        return false;
    return true;
}

//#################### GETTERS ############################

SDL_Rect Widget::get_form() const {
    return form;
}

Padding Widget::get_padding() const {
    return padding;
}

bool Widget::is_enabled() const{
    return enabled;
}

///==================SETTERS======================

void Widget::set_visible(bool visible) {
    this->visible = visible;
}

void Widget::set_enabled(bool enabled) {
    this->enabled = enabled;
}

void Widget::set_background(Texture::ptr background) {
    this->background = Texture::ptr(new Texture(*background));
}

void Widget::set_background(std::string back_path) {
    this->background = Texture::ptr(new Texture(back_path, (uint16_t)form.x, (uint16_t)form.y, (uint16_t)form.w, (uint16_t)form.h));
}

void Widget::set_form(SDL_Rect form) {
    this->form = form;
    this->background->set_form(this->form);
}
