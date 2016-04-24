#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "scrollable.h"

/**
 * @brief The Textbox class describes widget for text input/output,
 * contains string with text
 */
class Textbox : public Widget, public Scrollable {
    private:
        std::string text;                       /// text stored in textbox
        Texture::ptr text_txtr;                 /// hardware text
        bool multiline;                         /// texbox contains multiple lines or not
        bool input_enabled;                     /// could user input text into textbox
        TTF_Font* font;                         /// font used for text rendering
        SDL_Color font_color;                   /// color used for text rendering

        static std::string default_back_txtr;   /// path to texture used as default for background
        static std::string default_font_path;   /// path to font used as default
        static int default_font_size;           /// size used as default parameter for font size
        static SDL_Color default_font_color;    /// font color used as a default

        void text_to_texture();                 /// converts text from std::string to Texture
        void scroll_to_end();                   /// sets pos to text texture height - (form - margins)
        void redraw_text();                     /// changes part of text to render

        //################## SCROLLABLE #########################
        void update_bar() override;
    public:
        typedef std::unique_ptr<Textbox> ptr;

        //####################### CONSTRUCTORS #########################
        /**
         * @brief Textbox -- default constructor, creates empty textbox
         */
        Textbox();
        /**
         * @brief Textbox -- constuctor used to make normal textbox
         * @param form -- size and position of textbox
         * @param margin -- margins inside textbox, defaulted to no margins
         * @param multiline -- does text displays in multiple lines or in single line
         * @param font_color -- color used for text rendering, uses default static value
         * @param font_size -- size of font used to render text, uses default static value
         */
        Textbox( SDL_Rect form
               , Padding margin = {}
               , bool multiline = false
               , SDL_Color font_color = default_font_color
               , uint16_t font_size = default_font_size);

        //####################### DESTRUCTOR ###########################
        ~Textbox();

        //####################### GETTERS #########################
        std::string get_text() const;               /// returns text stored in textbox
        uint16_t get_height() const;                /// returns height of form without margin
        uint16_t get_text_height() const;           /// returns height of text texture
        /// used for textbox rendering, returns textures with text and background
        virtual std::vector<Texture*> get_textures() const override;
        virtual std::vector<const Listener*> get_listeners() const override;

        //###################### SETTERS ############################
        void set_text(std::string text);                        /// sets text to store in textbox
        void set_multiline(bool multiline);
        void set_font(std::string font_path, int font_size);
        void set_font_color(SDL_Color font_color);
        void set_scroll_pos(uint16_t pos);                      /// sets position in text to begin rendering from it
        static void set_default_back_path(std::string path);
        static void set_default_font_path(std::string path);
        static void set_default_font_size(int size);
        static void set_default_font_color(SDL_Color color);

        //####################### TEXT OPERATIONS ####################
        void add_text(std::string text);    /// wrapper of this->text += text
        void clear_text();                  /// wrapper of this->text.clear()

        //##################### SCROLLABLE #######################
        void update_pos() override;
};

#endif // TEXTBOX_H
