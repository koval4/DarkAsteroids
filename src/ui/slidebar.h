#ifndef SLIDER_H
#define SLIDER_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "widget.h"
#include "texture.h"
#include "button.h"
#include "scrollable.h"
#include "listener.h"

class Scrollable;

/**
 * @brief The Slidebar class describes widget used for sliding through other widgets
 */
class Slidebar : public Widget {
    private:
        /**
         * @brief The Slider struct describes part that moves inside slidebar
         * this part represents position into other widget
         */
        struct Slider {
            Texture::ptr texture;   /// texture of slider inside slidebar
            uint16_t size;          /// shows how much lines on screen represents slider
            uint16_t pos;           /// shows position of slider (upper side), from 0 to (this->size - slider.size)

            /**
             * @brief Slider -- default c-tor
             */
            Slider();
            /**
             * @brief Slider -- constructor that uses Texture object to set slider data
             * @param texture -- texture object used to set view of slider
             * @param size -- how much lines represents slider
             * @param pos -- position which represents top of slider
             */
            Slider(Texture::ptr texture, uint16_t size, uint16_t pos = 0);
            /**
             * @brief Slider -- constructor that uses path to texture for texture construction
             * @param txtr_path -- path to file with texture
             * @param size -- how much lines represents slider
             * @param pos -- position which represents top of slider
             */
            Slider(std::string txtr_path, SDL_Rect form, uint16_t size, uint16_t pos = 0);

            /**
             * @brief set_pos -- changes position of slider
             * @param parent -- parent Slidebar of this slider
             */
            void set_pos(Slidebar* parent);
        } slider;

        uint16_t size;              /// shows how much lines slidebar represents
        Button::ptr up;
        Button::ptr down;
        Scrollable* scrollable;     /// object that scrolled with this slidebar
        Listener wheel_listener;    /// checks mousewheel for scrolling

        static std::string default_back_path;
        static std::string default_slider_path;
        static std::string default_up_path;
        static std::string default_down_path;
    public:
        typedef std::unique_ptr<Slidebar> ptr;

        //################ CONSTRUCTORS ###############################
        /**
         * @brief Slidebar -- default constructor
         */
        Slidebar();
        /**
         * @brief Slidebar -- constuctor to set complete Slidebar
         * @param form -- size and position ofslidebar in pixels
         * @param lines_total -- how much lines represents full slidebar
         * @param lines_on_screen -- how much lines represents slider
         */
        Slidebar(SDL_Rect form, uint16_t lines_total
                 , uint16_t lines_on_screen);

        //################# DESTRUCTOR #############################
        ~Slidebar();

        //################# GETTERS ###################
        virtual std::vector<Texture*> get_textures() const override;
        virtual std::vector<const Listener*> get_listeners() const override;
        /**
         * @brief get_pos -- returns position of slide (upper side)
         * @return -- slider.pos
         */
        uint16_t get_pos() const;

        //################# SETTERS ###################
        void set_bar_size(uint16_t size);
        void set_slider_size(uint16_t size);
        void set_slider_pos(uint16_t pos);

        static void set_default_back_txtr(std::string path);
        static void set_default_slider_txtr(std::string path);
        static void set_default_up_txtr(std::string path);
        static void set_default_down_txtr(std::string path);

        //############### SCROLLABLE #######################
        void bind_to(Scrollable* scrollable);
};

#endif // SLIDER_H
