#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "listener.h"
#include "texture.h"
#include "widget.h"

/**
 * @brief The Label class describes widget that holds constant line of text without background
 */
class Label : public Widget {
    private:
        std::string text;       /// text in label
        Texture::ptr txtr;      /// texture made from text string
        TTF_Font* font;         /// font used for text rendering
        int font_size;          /// size of font used for text rendering
        SDL_Color font_color;   /// color of text used for text rendering

        static std::string default_font_path;   /// default font for text in label
        static int default_font_size;           /// default size of font
        static SDL_Color default_font_color;    /// default color of text in label
    public:
        typedef std::unique_ptr<Label> ptr;

        //################ CONSTRUCTORS ###################
        /**
         * @brief Label default constructor, makes empty label
         */
        Label();
        /**
         * @brief Label constructor creates label with passed text at passed position
         * @param text -- text inside label
         * @param form -- size and position of label
         */
        Label(std::string text, SDL_Rect form);

        //################## DESTRUCTOR #####################
        ~Label();

        //#################### GETTERS #######################
        virtual std::vector<Texture*> get_textures() const override;
        virtual std::vector<const Listener*> get_listeners() const override;

        //#################### SETTERS #########################
        void set_font(std::string font_path, int font_size);
        void set_font_color(SDL_Color color);
        void set_text(std::string text);
        static void set_default_font(std::string path);
        static void set_default_font_size(int size);
        static void set_default_font_color(SDL_Color color);
};

#endif // LABEL_H
