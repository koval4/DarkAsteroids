#ifndef GUI_H
#define GUI_H

#include <vector>
#include <memory>
#include "widget.h"
#include "screen.h"

class GUI {
    private:
        std::vector<Widget*> widgets;
        std::vector<Texture::ptr> textures; /// stores textures that will be rendered
        std::vector<Texture::ptr> buffer;   /// stores textures in temporary buffer

        //############## CONSTRUCTORS ###############
        GUI();
    public:
        /**
         * @brief update renders all textures and widgets
         */
        void update();

        //################ GETTERS ###################
        static GUI& inst();
        std::vector<Texture*> get_textures();

        //################ SETTERS ####################
        void set_widgets(std::vector<Widget*> widgets);

        //############ WIDGETS OPERATIONS ##############
        /**
         * @brief add_widget
         * @param widget
         */
        void add_widget(Widget* widget);
        /**
         * @brief remove_widget
         * @param widget
         */
        void remove_widget(Widget* widget);
        /**
         * @brief clear_widgets
         */
        void clear_widgets();

        //############# TEXTURES OPERATINS #############
        void add_texture(std::string path, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
        void flush();  /// moves textures from buffer to textures vector
        void clear_textures();
};

#endif // GUI_H
