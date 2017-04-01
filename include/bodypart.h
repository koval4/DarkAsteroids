#ifndef BODYPART_H
#define BODYPART_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "common.h"
#include "item.h"

/**
 * @brief The Bodypart class
 */
class Bodypart : public Item {
    public:
        typedef std::shared_ptr<Bodypart> ptr;

    private:
        std::vector<Bodypart::ptr> inner_parts; /// inner bodyparts
        Bodypart* connection;                   /// actual bodypart to which is attached this bodypart
        std::vector<Bodypart*> connected_parts; /// bodyparts attached to this
        std::string str_connection;             /// name of connection, used for body generation
        std::vector<Item::ptr> equipped_items;  /// items that covers bodypart
        std::vector<Item::ptr> grabbed_items;   /// items grabbed by this bodypart
        int items_weight;                       /// sum of *_items weight
        int max_weight;                         /// maximal sum of equipped items' weight
        bool grasp;                             /// can bodypart grasp something or not
        bool vital;                             /// is organ vital for actor
        int pain;                               /// how much it hurts

        /**
         * @brief make_inner_parts
         * @param line
         */
        void make_inner_parts(std::string line);

    public:
        static std::unordered_map<std::string, Bodypart> LIST;

        //########## CONSTRUCTORS ############
        Bodypart();
        Bodypart(const Bodypart& obj);  /// copy constructor
        /**
         * @brief Bodypart
         * @param descr
         */
        Bodypart(std::string descr);

        virtual Item::ptr clone() override;

        //############ DATABASE #############
        static void read_bodyparts_txt();

        //########## GETTERS ################
        bool is_grasp() const;
        int get_pain() const;
        int get_max_weight() const;
        int get_items_weight() const;
        std::vector<Item::ptr> get_items();
        std::vector<Item::ptr> get_grabbed_items();

        //########### SETTERS ################
        void set_max_weight(int max_weight);
        /**
         * @brief set_relative_size
         * @param abs_volume
         * @param percent
         */
        void set_relative_size(int abs_volume, int percent);
        void connect_to(Bodypart* connection);
        
        //############# ITEMS OPERATIONS #############
        void add_item(Item::ptr item);      /// equipped_items.push_back() wrapper
        void remove_item(Item::ptr item);   /// equipped_items.erase() wrapper
        void grab_item(Item::ptr item);     /// grabbed_items.push_back() wrapper
        void drop_item(Item::ptr item);     /// grabbed_items.erase() wrapper

        //############ GAME LOGIC #############
        /**
         * @brief get_deformation
         * @param stress
         * @param item
         */
        virtual void get_deformation(uint32_t& stress, Item& item) override;
        /**
         * @brief get_hit
         * @param stress
         * @param item
         */
        void get_hit(uint32_t& stress, Item& item);

};

#endif // BODYPART_H
