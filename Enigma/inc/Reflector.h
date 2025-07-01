#pragma once
#include <string>
#include <unordered_map>

class Reflector {
private:
    std::string wiring;  // Строка с попарными отражениями (например, "YRUHQSLDPXNGOKMIEBFZCWVJAT")
    std::unordered_map<char, char> reflectionMap;  // Хэш-таблица для быстрого доступа

public:
    // Конструктор принимает строку проводки
    Reflector() = default;
    explicit Reflector(const std::string& wiring);

    // Отражение символа (A→Y, Y→A и т.д.)
    char reflect(char c) const;

    // Проверка корректности проводки
    bool isValidWiring(const std::string& wiring) const;
};