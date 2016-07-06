#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "listener.h"

/**
 * @brief The Button class
 */
class Button : public Widget {
    private:
        Texture::ptr txtr;           /// texture that contains text or icon that will be placed inside button
        TTF_Font* font;         /// font used for text rendering
        SDL_Color font_color;   /// color used for text rendering

        Listener listener;
        std::function<void()> on_click_handler; /// handler that will be called if button is clicked

        static std::string default_back_path;   /// path to texture that will be used as default background
        static std::string default_font_path;   /// path to font that will be used as default
        static int default_font_size;           /// size of default font
        static SDL_Color default_font_color;    /// default color of text
    public:
        typedef std::shared_ptr<Button> ptr;

        /**
         * @brief The Content enum used to mark content that will be placed inside button
         */
        enum Content {
            icon,
            text
        };
        //############### CONSTRUCTORS ###############
        /**
         * @brief Button -- default constructor, doing nothing
         */
        Button();
        /**
         * @brief Button -- constructor to make complete button
         * @param access_name -- name used for identification of element
         * @param txtr -- contains text to render or path to icon; will be placed in center of button
         * @param form  -- size and position of button
         * @param margin -- margins inside button
         * @param content -- marks content that will be placed in button (text, icon), defaulted to text
         */
        Button(std::string access_name, std::string txtr, SDL_Rect form, Padding padding, Content content = text);

        //############## DESTRUCTOR #################
        ~Button();

        //############### GETTERS ###################
        virtual std::vector<Texture*> get_textures() const override;
        virtual std::vector<const Listener*> get_listeners() const override;

        //############## SETTERS #####################
        void set_font(std::string font_path, int font_size);
        //--------------default-setters----------------
        static void set_default_background(std::string path);
        static void set_default_font(std::string path);
        static void set_default_font_size(int size);
        static void set_default_font_color(SDL_Color color);

        //################ EVENTS #####################
        void on_click(std::function<void()> handler);
};

#endif // BUTTON_H
