#include "container.h"
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include "item.h"
#include "stack.h"

std::map<std::string, Container> Container::LIST;   /// database with prototypes for containers

Container::Container()
    : Item() {
    type = container;
}

Container::Container(const Container& obj)
    : Item(obj) {
    for (auto& item : obj.content)
        this->content.push_back(item->clone());
}

Item::ptr Container::make(std::string prototype) {
    Item::ptr item(new Container(LIST.at(prototype)));
    return item;
}

Item::ptr Container::clone() {
    return Item::ptr(new Container(*this));
}

void Container::read_containers_txt() {
    std::ifstream file("containers.txt");
    std::string line = "";

    if (!file.is_open()) {
        log_file << "Unable to open containers.txt" << std::endl;
        std::cerr << "Unable to open containers.txt" << std::endl;
        return;
    }

    Container* container;
    while (std::getline(file, line)) {
        if (line.front() == '#') continue;
        if (line.find_first_not_of(" \n\t\r") == std::string::npos) continue;   //if line is empty
        while (line.find(";") == std::string::npos) {   //line ends with semicolon
            std::string buffer_line = "";
            std::getline(file, buffer_line);
            line += buffer_line;
        }
        line.erase(line.find(";"));
        trimr_string(line); //erasing unnecessary from end
        if (line == "NEW_ITEM")
            container = new Container();
        else if (line == "PUSH_ITEM") {
            LIST.insert({container->name, *container});
            log_file << "Added new item to Container::LIST: " << container->name << std::endl;
            delete container;
        } else if (line.find("wearable") != std::string::npos) {
          line = take_value(line);
          if (line == "true")
              container->type = Item::wear_cont;
          else container->type = Item::container;
        } else container->read_txt_properties(line);
    }
    file.close();
}

std::string Container::describe() {
    std::string line = name + "\n";
    line += description + "\n";
    line += "Weight: " + std::to_string(weight) + "\n";
    line += "Price: " + std::to_string(price) + "\n";
    if (content.empty())
        line += "It's empty.";
    else {
        line += "It contains ";
        auto item = content.begin();
        while (item != --content.end()) {
            line += (*item)->get_name() + ", ";
            item++;
        };
        line += (*item)->get_name() + ".";
    }

    return line;
}

void Container::add_item(Item::ptr item) {
    auto it = std::find_if(content.begin(), content.end(), [&item] (Item::ptr& a) -> bool {
        return (a->get_name().find(item->get_name()) != std::string::npos);
    });
    if (it == content.end())
        content.push_back(std::move(item));
    else {
        if (item->get_name() == (*it)->get_name()) {
            Item::ptr stacked(new Stack(std::move(item), 2));
            content.erase(it);
            content.push_back(std::move(stacked));
        } else {
            dynamic_cast<Stack*>(it->get())->add(std::move(item));
        }
    }
}

void Container::remove_item(Item* item) {
    auto it = content.begin();
    while (item != it->get())
        it++;
    content.erase(it);
}

Item::ptr Container::take_item(Item* item) {
    auto it = content.begin();
    while (item != it->get())
        it++;
    Item::ptr taken_item = std::move(*it);
    content.erase(it);
    return taken_item;
}

std::vector<Item*> Container::open_container() {
    std::vector<Item*> items;
    for (auto& item : content)
        items.push_back(item.get());
    return items;
}
