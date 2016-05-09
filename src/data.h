#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <ostream>
#include <iostream>

//#define PAIRS(first, second) std::vector<std::pair<first, second>>
template<typename F, typename S>
using PAIRS = std::vector<std::pair<F, S>>;

extern std::string txtr_path;
#define img(name) txtr_path + std::string(name)

/**
 * @brief The Coord struct represents coordinates
 */
struct Coord {
        uint8_t x;
        uint8_t y;

        Coord() : x(0), y(0) {}
        Coord(int x, int y) : x(x), y(y) {}

        uint8_t get_length() const {
            return sqrt(x*x + y*y);
        }

        bool operator == (const Coord& right) const {
            return (this->x == right.x && this->y == right.y);
        }

        bool operator != (const Coord& right) const {
            return !(*this == right);
        }

        //for ordering
        /**
         * @brief operator < -- compares length of vectors
         * @param right -- value at right
         * @return is length of this less than length of right
         */
        bool operator < (const Coord& right) const {
            return (this->get_length() < right.get_length());
        }
};

        struct Rectangle {
            Coord first;
            Coord last;

            bool is_overlapping(const Rectangle& other) const {
    return (this->first.x < other.last.x && this->last.x > other.first.x &&
            this->first.y < other.last.y && this->last.y > other.first.y);
            }
        };

namespace std {
    template<>
    struct hash<Coord> {
        size_t operator () (const Coord& coord) const {
            return std::hash<int>()(coord.x) ^ std::hash<int>()(coord.y) ^ std::hash<int>()(coord.x);
        }
    };
}

extern std::ofstream log_file;

//####################### UTILITY #######################

bool is_number(std::string&);   ///checks is string integer number or not
std::vector<std::string> split_string(std::string, char); ///splits string into vector of substrings, gets string to be splitted and delimination char
void triml_string(std::string&); ///removes whitespace from the beginning
void trimr_string(std::string&); ///removes whitespace from the end
/**
 * @brief replace_keyword replaces keyword in string to some other word
 * @param line
 * @param keyword
 * @param word
 */
void replace_keyword(std::string& line, std::string keyword, std::string word);
/**
 * @brief find_match_char
 * @param line
 * @param index
 * @param lchar
 * @param rchar
 * @return
 */
size_t find_match_char( std::string line
                      , size_t index
                      , char lchar = '('
                      , char rchar = ')');
/**
 * @brief split_with_match_chars
 * @param line
 * @return
 */
std::vector<std::string> split_with_match_chars(std::string line);
/**
 * @brief get_value
 * @param line
 * @return
 */
std::string take_value(std::string line);

//############# PHYSICS ################

int32_t get_momentum(float mass, int16_t force, uint16_t time = 1);    ///calculates momentum using item's mass, force and time
uint32_t get_force(uint32_t momentum, uint16_t time = 1);   ///calculates force using momentum and time
uint32_t get_stress(uint32_t force, uint32_t area); ///fuck you, it's obvious

//############# FORWARD DECLARATIONS ###############
class Weapon;

#endif // DATA_H_INCLUDED
