#include "states/initstate.h"

#include "ui/screen.h"
#include "ui/label.h"
#include "ui/textbox.h"
#include "ui/button.h"
#include "ui/listbox.h"
#include "ui/panel.h"

#include "item.h"
#include "bodypart.h"
#include "weapon.h"
#include "container.h"
#include "armor.h"
#include "npcprototype.h"
#include "race.h"

void UILoader::load_ui() {
    Screen::init(); // call to create an instance of screen and to initialize SDL
    Textbox::set_default_back_path(img("textures/bordered_back.png"));
    Textbox::set_default_font_path("mplus-1m-regular.ttf");
    Textbox::set_default_font_size(12);
    Textbox::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Button::set_default_background(img("textures/bordered_back.png"));
    Button::set_default_font("mplus-1m-regular.ttf");
    Button::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Button::set_default_font_size(14);
    Listbox::set_default_background(img("textures/bordered_back.png"));
    Listbox::set_default_font("mplus-1m-regular.ttf");
    Listbox::set_default_font_size(12);
    Listbox::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Listbox::set_default_item_back("textures/bordered_back.png");
    Listbox::set_default_item_height(25);
    Listbox::set_default_item_margin({2, 6, 2, 5});
    Listbox::set_default_spacing(2);
    Slidebar::set_default_back_txtr("textures/slidebar.png");
    Slidebar::set_default_slider_txtr("textures/slider.png");
    Slidebar::set_default_up_txtr("textures/up_arrow.png");
    Slidebar::set_default_down_txtr("textures/down_arrow.png");
    Panel::set_default_background("textures/bordered_back.png");
    Label::set_default_font("mplus-1m-regular.ttf");
    Label::set_default_font_color({0xff, 0xff, 0xff, 0x00});
    Label::set_default_font_size(12);
}

void EntityLoader::load_entities() {
    Item::read_materials_txt();
    Item::read_item_txt();
    Bodypart::read_bodyparts_txt();
    Weapon::read_weapons_txt();
    Armor::read_armor_txt();
    Container::read_containers_txt();
    Race::read_races_txt();
    NPCPrototype::read_from_file("npc.txt");
}
