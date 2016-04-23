#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include "texture.h"
#include "../data.h"

/** abstract class that describes base gui element*/
class Widget{
    protected:
        Texture::ptr background;
        SDL_Rect form;  ///position and size of widget
        Margin margin;  ///margin of widget

        bool visible;   ///is object visible
        bool enabled;   ///is object enabled

        static uint16_t obj_counter;    ///counter of Widget objects, used for memory leaks control

        bool is_point_in(SDL_Point point);  /// checks if point belongs form rectangle
    public:
        typedef std::unique_ptr<Widget> ptr;

        Widget();   ///base c-tor, creates empty widget
        Widget(Texture::ptr background, SDL_Rect form, Margin margin = {});
        Widget(std::string back_path, SDL_Rect form, Margin margin = {});
        virtual ~Widget();  ///d-tor

        //####################### GETTERS ##########################
        virtual std::vector<Texture*> get_textures() const = 0;    ///returns textures of widget
        SDL_Rect get_form();
        Margin get_margin();

        //############### SETTERS ##################
        void set_visible(bool visible);
        void set_enabled(bool enabled);
        void set_background(Texture::ptr background);
        void set_background(std::string back_path);
        void set_form(SDL_Rect form);
};

#endif // WIDGET_H_INCLUDED
