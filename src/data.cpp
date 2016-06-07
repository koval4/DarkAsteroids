#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <ostream>
#include <iostream>
#include <limits>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "data.h"

std::string txtr_path = "textures/";
std::ofstream log_file;

/** checks is string integer number or not*/
bool is_number(std::string & str){
    if(str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) return false ;
    //iterating through all chars
    for (uint16_t i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))   //if char isn't number => return false
            return false;
    return true;
}

/** splits string into vector of substrings, gets string to be splitted and delimination char*/
std::vector<std::string> split_string(std::string line, char delim = ','){
    std::vector<std::string> result;    //vector, returned by function
    std::string buffer_str = "";

    if (line.find(delim) == std::string::npos){ //if line does not contain delimination chars => return full line
        result.push_back(line);
        return result;
    }

    do {
        triml_string(line); //erasing whitespaces from begin
        buffer_str = line.substr(0, line.find(delim));  //buffer string = _word_
        result.push_back(buffer_str);
        buffer_str = "";
        line.erase(0, line.find(delim) + 1);
    } while (line.find(delim) != std::string::npos);

    if (line != ""){ //if something left in line => add it to vector
        triml_string(line);
        result.push_back(line);
    }

    return result;
}

/** removes whitespace from the beginning*/
void triml_string(std::string& line){
    if (line.find_first_not_of(" \t") != 0)
        line.erase(0, line.find_first_not_of(" \t"));
}

/** removes whitespace from the end*/
void trimr_string(std::string& line){
    if (line.find_last_not_of(" \t\r") != 0)
        line.erase(line.find_last_not_of(" \t\r") + 1, line.size());
}

/** replaces keyword in string to some other word
    line - string with main text
    keyword - symbols to replace
    word - symbols after replacement*/
void replace_keyword(std::string& line, std::string keyword, std::string word){
    while (line.find(keyword) != std::string::npos){
        line.replace(line.find(keyword), keyword.size(), word);
    }
}

size_t find_match_char(std::string line, size_t index, char lchar, char rchar) {
    uint16_t left_parnths_num = 1;
    uint16_t right_parnths_num = 0;
    size_t i = index + 1;
    for (; left_parnths_num != right_parnths_num; i++) {
        if (line[i] == rchar)
            right_parnths_num++;
        else if (line[i] == lchar)
            left_parnths_num++;
    }
    return i;
}

std::vector<std::string> split_with_match_chars(std::string line) {
    std::vector<std::string> parts;
    while (!line.empty()) {
        size_t delimiter_pos = line.find_first_of(",({");
        if (line[delimiter_pos] == '(')
            delimiter_pos = line.find(',', find_match_char(line, delimiter_pos));
        else if (line[delimiter_pos] == '{')
            delimiter_pos = line.find(',', find_match_char(line, delimiter_pos, '{', '}'));
        if (delimiter_pos == std::string::npos) {
            triml_string(line);
            trimr_string(line);
            parts.push_back(line);
            break;
        }
        std::string part = line.substr(0, delimiter_pos);
        line.erase(0, delimiter_pos + 1);
        triml_string(part);
        trimr_string(part);
        parts.push_back(part);
    }
    return parts;
}

std::string take_value(std::string line) {
    line.erase(0, line.find("=") + 1);
    triml_string(line);
    // taking value from ()
    if (line.find("(") != std::string::npos)
        line = line.substr(line.find("(") + 1, line.find(")") - 1);
    return line;
}

/** calculates momentum using item's mass, force and time*/
int32_t get_momentum(float mass, int16_t force, uint16_t time){
    return (mass * (((double)force/mass) * time));
}

/** calculates force using momentum and time*/
uint32_t get_force(uint32_t momentum, uint16_t time){
    return (momentum/time);
}

/** fuck you, it's obvious*/
uint32_t get_stress(uint32_t force, uint32_t area){
    return (force/area);
}
