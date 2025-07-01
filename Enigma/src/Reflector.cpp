#include "Reflector.h"
#include <stdexcept>
#include <algorithm>

Reflector::Reflector(const std::string& wiring) : wiring(wiring) {
    if (!isValidWiring(wiring)) {
        throw std::invalid_argument("Invalid reflector wiring!");
    }

    // Строим хэш-таблицу отражений
    for (int i = 0; i < 26; ++i) {
        char from = 'A' + i;
        char to = wiring[i];
        reflectionMap[from] = to;
        reflectionMap[to] = from;  // Обратное отображение
    }
}

char Reflector::reflect(char c) const {
    c = toupper(c);
    auto it = reflectionMap.find(c);
    if (it == reflectionMap.end()) {
        throw std::invalid_argument("Invalid character for reflection!");
    }
    return it->second;
}

bool Reflector::isValidWiring(const std::string& wiring) const {
    if (wiring.length() != 26) return false;

    // Проверяем, что каждая буква отражается ровно один раз
    for (int i = 0; i < 26; ++i) {
        char current = wiring[i];
        if (wiring[i] - 'A' != i && wiring[wiring[i] - 'A'] != 'A' + i) {
            return false;
        }
    }
    return true;
}