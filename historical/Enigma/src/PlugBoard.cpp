#include "PlugBoard.h"
#include <iostream>
#include <sstream>  
#include <algorithm>
#include <cctype>

bool PlugBoard::connect (char a, char b) {
    a = toupper(a);
    b = toupper(b);

    if (a < 'A' || a > 'Z' || b < 'A' || b > 'Z') {
        std::cout << "Error: Only letters A-Z are allowed!\n";
        return false;
    }

    if (connections.count(a) || connections.count(b)) {
        std::cout << "Error: Letter '" << a << "' or '" << b << "' is already used!\n";
        return false;
    }

    connections[a] = b;
    connections[b] = a;
    return true;
}

void PlugBoard::set_pairs(const std::string& pairs) {
    connections.clear();
    std::istringstream iss(pairs);
    std::string pair;
    while (iss >> pair) {
        if (pair.size() == 2) {
            if (!connect(pair[0], pair[1])) {
                std::cout << "Pair " << pair << " was skipped due to conflict.\n";
            }
        } 
        else {
            std::cout << "Skipping invalid pair: " << pair << " (must be 2 letters)\n";
        }
    }
}

char PlugBoard::plug(char c) const {
    if (!isalpha(c)) return c; 
    c = toupper(c);
    auto it = connections.find(c);
    return (it != connections.end()) ? it->second : c;
}

void PlugBoard::print_pairs() const {
    std::cout << "Plugboard pairs: ";
    for (const auto& pair : connections) {
        if (pair.first < pair.second) {
            std::cout << pair.first << pair.second << " ";
        }
    }
    std::cout << "\n";
}

