// utils.h
#pragma once
#include <string>
#include <vector>

class EnigmaUtils {
public:
    // Объявления (без инициализации)
    static const std::vector<std::string> ROTOR_WIRINGS;
    static const std::vector<int> ROTOR_NOTCHES;
    static const std::string REFLECTOR_B;
    static const std::string REFLECTOR_C;
    
    // Методы остаются здесь
    static bool validateRotorPositions(const std::string& positions);
    static bool validatePlugboardPairs(const std::string& pairs);
    static std::string toUpper(const std::string& s);
};