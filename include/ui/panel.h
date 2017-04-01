#ifndef PANEL_H
#define PANEL_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "listener.h"
#include "texture.h"
#include "widget.h"

/**
 * @brief The Panel class describes container for other widgets
 */
class Panel : public Widget {
    private:
        std::vector<Widget::ptr> widgets;   /// widgets placed on panel

        static std::string default_back_path;   /// path to texture used as default for background
    public:
        typedef std::shared_ptr<Panel> ptr;

        //################ CONSTRUCTORS ##################
        /**
         * @brief Panel -- default constructor, doing nothing
         */
        Panel();
        /**
         * @brief Panel -- constructor that creates just panel widthout widgets
         * @param access_name -- name used for identification of element
         * @param form -- size and position of panel
         * @param margin -- margins inside panel
         */
        Panel(std::string access_name, SDL_Rect form, Padding padding);

        //################# DESTRUCTOR #######################
        ~Panel();

        //################## GETTERS ###########################
        virtual std::vector<Texture*> get_textures() const override;   /// returns textures of all stored widgets
        virtual std::vector<const Listener*> get_listeners() const override;

        //#################### SETTERS ############################
        void set_widgets(std::vector<Widget::ptr> widgets);
        static void set_default_background(std::string path);

        //#################### WIDGETS OPERATIONS ##################
        void add_widget(const Widget::ptr widget);
        void remove_widget(const Widget::ptr widget);
        void clear_widgets();
};

#endif // PANEL_H
