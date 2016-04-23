#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <memory>

/**
 * @brief The Texture class -- Wrapper of SDL_Texture*
    contains texture, position and size of it
 */
class Texture {
    private:
        SDL_Texture* texture;     /// actual hardware texture
        SDL_Rect form;            /// position and size of texture
        int width;                /// actual width of texture
        int height;               /// actual height of texture
        SDL_Rect part_to_render;  /// represents part of texture to render
        std::string path;         /// path to texture

        static uint16_t obj_counter;  /// counter of Texture objects, used for memory leaks control

        void make_texture(SDL_Surface* surface);

    public:
        //################## TYPEDEFS ########################
        typedef std::unique_ptr<Texture> ptr;
        //################## CONSTRUCTORS ###############################
        Texture();  /// base c-tor, creates empty texture
        Texture(std::string path,
                uint16_t x,
                uint16_t y,
                uint16_t w = 0,
                uint16_t h = 0
               );  /// c-tor used for texture creation
        Texture(std::string text,
                TTF_Font* font,
                SDL_Color text_color,
                uint16_t x,
                uint16_t y,
                uint16_t w = 0,
                uint16_t h = 0
                );  /// c-tor used for creation text using TTF
        Texture(std::string text,
                uint16_t wrap_width,
                TTF_Font* font,
                SDL_Color text_color,
                uint16_t x,
                uint16_t y
                );  /// c-tor used for creation wrapped text using TTF
        Texture(const Texture& obj);  // copy constructor

        //################### DESTRUCTOR ###########################
        ~Texture();  /// d-tor

        //################### GETTERS #############################
        int get_width() const;
        int get_height() const;
        SDL_Rect get_form() const;
        SDL_Rect get_part_to_render() const;

        //################## SETTERS ###############################
        void set_form(SDL_Rect form);
        void set_part_to_render(SDL_Rect part_to_render);

        //################### TEXTURE OPERATIONS ###############################
        void draw(SDL_Renderer* renderer);  /// copies texture to renderer
};

#endif  // TEXTURE_H_INCLUDED
