#ifndef LISTBOX_H
#define LISTBOX_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "widget.h"
#include "../data.h"
#include "listener.h"

/**
 * @brief The Listbox class describes widget that shows list of string items
 */
class Listbox : public Widget {
    private:
        /**
         * @brief The List_Item struct describes single visible item into listbox
         */
        struct List_Item {
            std::string text;           /// item text
            Texture::ptr text_txtr;     /// texture with item text
            Texture::ptr background;    /// background of item
            SDL_Rect form;              /// size and position of item
            Margin margin;              /// margins inside item

            Listener listener;          /// listener that checks if item is clicked

            typedef std::unique_ptr<List_Item> ptr;

            /**
             * @brief List_Item -- constructor of single visible item
             * @param text -- text in item, used for text_txtr creation
             * @param form -- size and position of item
             */
            List_Item(Listbox* const parent, std::string text, SDL_Rect form);
        };

        std::vector<std::string> items;             /// items stored in listbox
        std::vector<List_Item::ptr> visible_items;  /// textures of items visible for user
        uint16_t pos;                               /// number of item to start rendering from
        uint16_t item_height;                       /// height of each item in pixels
        uint16_t spacing;                           /// spacing in pixels between visible items
        TTF_Font* items_font;                       /// font used for text rendering in items
        SDL_Color items_color;                      /// colour of text used for rendering in item
        std::string item_background;                /// background of each item used for rendering
        Margin item_margin;                         /// margins inside each of items

        std::function<void(std::string)> item_clicked_handler;

        static std::string default_back_txtr;       /// path to texture used as default for background
        static std::string default_item_back_txtr;  /// path to texture used as default for list items background
        static std::string default_font_path;       /// path to font used as default
        static int default_font_size;               /// size used as default parameter for font size
        static SDL_Color default_font_color;        /// default color used for text in list items
        static uint16_t default_item_height;        /// default height in pixels used for list item form
        static uint16_t default_spacing;            /// default spacing between visible items
        static Margin default_item_margin;          /// default margins used in items

        void draw_items();  /// makes visible_items from items
    public:
        typedef std::unique_ptr<Listbox> ptr;

        //################# CONSTRUCTORS ##########################
        /**
         * @brief Listbox -- default constructor
         */
        Listbox();
        /**
         * @brief Listbox -- constructor for complete listbox construction without items
         * @param form -- size and position of listbox
         * @param margin -- margins inside listbox, defaulted to no margins
         * @param items_color -- color used for items rendering, defaulted to black
         */
        Listbox(SDL_Rect form, Margin margin = {}
                , uint16_t item_height = default_item_height);

        //################## DESTRUCTOR ##########################
        ~Listbox();

        //################## GETTERS ##############################
        uint16_t get_items_count() const;
        uint16_t get_visible_items_count() const;
        std::vector<Texture*> get_textures() const;

        //################## SETTERS ##############################
        void set_items(std::vector<std::string> items);
        void set_font(std::string font_path, int font_size);
        void set_color(SDL_Color colour);
        void set_item_height(uint16_t item_height);
        void set_spacing(uint16_t spacing);
        void set_scroll_pos(uint16_t scroll_pos);
        //-----------------default-setters---------------------
        static void set_default_background(std::string path);
        static void set_default_item_back(std::string path);
        static void set_default_font(std::string path);
        static void set_default_font_size(int font_size);
        static void set_default_font_color(SDL_Color color);
        static void set_default_item_height(uint16_t item_height);
        static void set_default_spacing(uint16_t spacing);
        static void set_default_item_margin(Margin item_margin);

        //################## ITEMS OPERATIONS #######################
        void add_item(std::string item);    /// adds single item in textbox
        void remove_item(std::string item); /// removes single item from textbox
        void clear_items();                 /// removes all items from listbox

        //######################## EVENTS #############################
        void on_item_clicked(std::function<void(std::string)> handler);
};

#endif // LISTBOX_H
