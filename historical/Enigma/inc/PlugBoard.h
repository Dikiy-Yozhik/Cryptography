#pragma once

#include <unordered_map>
#include <string>

class PlugBoard {
private:
    std::unordered_map <char, char> connections;
public:
    void set_pairs(const std::string& pairs);
    bool connect(char a, char b);
    char plug(char c) const;
    void print_pairs() const;
};