#pragma once
#include "PlugBoard.h"
#include "Rotor.h"
#include "Reflector.h"
#include "utils.h"
#include <vector>

class EnigmaMachine {
private:
    PlugBoard plugboard;
    std::vector<Rotor> rotors;
    Reflector reflector;
    bool isConfigured;

    void rotateRotors();

public:
    // Конструкторы
    EnigmaMachine();
    EnigmaMachine(const PlugBoard& pb, const std::vector<Rotor>& rs, const Reflector& ref);
    
    // Настройка машины
    void setup(const std::string& rotorPositions, 
              const std::string& plugboardPairs,
              const std::vector<int>& rotorTypes = {0, 1, 2}, // I, II, III по умолчанию
              int reflectorType = 0); // 0 = B, 1 = C
    
    // Установка позиций роторов
    void setRotorPositions(const std::string& positions);
    
    // Шифрование/дешифрование
    char processChar(char c);
    std::string process(const std::string& text);
    
    // Проверка готовности
    bool isReady() const { return isConfigured; }
};