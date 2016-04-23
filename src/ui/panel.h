#ifndef PANEL_H
#define PANEL_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"

/**
 * @brief The Panel class describes container for other widgets
 */
class Panel : public Widget {
    private:
        std::vector<Widget*> widgets;   /// widgets placed on panel

        static std::string default_back_path;   /// path to texture used as default for background
    public:
        typedef std::unique_ptr<Panel> ptr;

        //################ CONSTRUCTORS ##################
        /**
         * @brief Panel -- default constructor, doing nothing
         */
        Panel();
        /**
         * @brief Panel -- constructor that creates just panel widthout widgets
         * @param form -- size and position of panel
         * @param margin -- margins inside panel
         */
        Panel(SDL_Rect form, Margin margin);

        //################# DESTRUCTOR #######################
        ~Panel();

        //################## GETTERS ###########################
        std::vector<Texture*> get_textures() const;   /// returns textures of all stored widgets

        //#################### SETTERS ############################
        void set_widgets(std::vector<Widget*> widgets);
        static void set_default_background(std::string path);

        //#################### WIDGETS OPERATIONS ##################
        void add_widget(Widget* widget);
        void remove_widget(Widget* widget);
        void clear_widgets();
};

#endif // PANEL_H
