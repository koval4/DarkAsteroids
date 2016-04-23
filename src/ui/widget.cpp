#include <vector>
#include <string>
#include <typeinfo>
#include <SDL2/SDL.h>
#include "texture.h"
#include "../data.h"
#include "widget.h"

uint16_t Widget::obj_counter = 0;

///================CONSTRUCTORS=====================

Widget::Widget() {
    obj_counter++;
    form = {};
    margin = {};
    visible = false;
    enabled = false;
}

Widget::Widget(Texture::ptr background, SDL_Rect form, Margin margin)
    : background(std::move(background)), form(form), margin(margin) {
    obj_counter++;
    visible = true;
    enabled = true;
}

Widget::Widget(std::string back_path, SDL_Rect form, Margin margin)
    : form(form), margin(margin), visible(true)
    , enabled(true), background(new Texture(back_path, form.x, form.y, form.w, form.h)) {
    obj_counter++;
}

///===================DESTRUCTOR========================

Widget::~Widget() {
    obj_counter--;
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

SDL_Rect Widget::get_form() {
    return form;
}

Margin Widget::get_margin() {
    return margin;
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
