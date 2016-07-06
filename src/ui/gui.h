#ifndef GUI_H
#define GUI_H

#include <vector>
#include <unordered_map>
#include <memory>
#include "widget.h"
#include "screen.h"

class GUI {
    private:
        std::unordered_map<std::string, Widget::ptr> widgets;
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
        template <typename T, typename... Args>
        typename T::ptr make_widget(Args&&... args);
        Widget::ptr get_widget(std::string access_name) const;
        template <typename T>
        typename T::ptr get_widget(std::string access_name) const;
        void remove_widget(std::string access_name);
        void clear_widgets();

        //############# TEXTURES OPERATINS #############
        void add_texture(std::string path, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
        void flush();  /// moves textures from buffer to textures vector
        void clear_textures();
};

template <typename T, typename... Args>
typename T::ptr GUI::make_widget(Args&&... args) {
    // used for type constraint
    static_assert(std::is_base_of<Widget, T>::value, "T must be derived from Widget");
    Widget::ptr widget = std::make_shared<T>(std::forward<Args>(args)...);
    // registering new widget
    widgets.insert({ widget->get_access_name(), widget });
    return  std::dynamic_pointer_cast<T>(widget);
}

template <typename T>
typename T::ptr GUI::get_widget(std::string access_name) const {
    // return nullptr if widget doesn't exists
    if (widgets.find(access_name) == widgets.end())
        return nullptr;
    return std::dynamic_pointer_cast<T>(widgets.at(access_name));
}

#endif // GUI_H
