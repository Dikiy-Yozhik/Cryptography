#include "EnigmaMachine.h"
#include <stdexcept>

EnigmaMachine::EnigmaMachine() : isConfigured(false) {
    reflector = Reflector(EnigmaUtils::REFLECTOR_B);
}

EnigmaMachine::EnigmaMachine(const PlugBoard& pb, 
                           const std::vector<Rotor>& rs, 
                           const Reflector& ref)
    : plugboard(pb), rotors(rs), reflector(ref), isConfigured(true) {
    if (rotors.size() != 3) {
        throw std::invalid_argument("Enigma requires exactly 3 rotors");
    }
}

void EnigmaMachine::setup(const std::string& rotorPositions,
                         const std::string& plugboardPairs,
                         const std::vector<int>& rotorTypes,
                         int reflectorType) {
    // 1. Проверка входных данных
    if (!EnigmaUtils::validateRotorPositions(rotorPositions) || 
        !EnigmaUtils::validatePlugboardPairs(plugboardPairs)) {
        throw std::invalid_argument("Invalid configuration parameters");
    }

    // 2. Настройка Plugboard
    plugboard = PlugBoard();
    plugboard.set_pairs(plugboardPairs);

    // 3. Настройка роторов
    rotors.clear();
    for (int i = 0; i < 3; ++i) {
        int type = rotorTypes[i];
        rotors.emplace_back(
            EnigmaUtils::ROTOR_WIRINGS[type],
            EnigmaUtils::ROTOR_NOTCHES[type],
            toupper(rotorPositions[i]) - 'A'
        );
    }

    // 4. Настройка рефлектора
    reflector = Reflector(reflectorType == 0 ? 
                EnigmaUtils::REFLECTOR_B : 
                EnigmaUtils::REFLECTOR_C);

    isConfigured = true;
}

void EnigmaMachine::setRotorPositions(const std::string& positions) {
    if (!isConfigured) throw std::logic_error("Machine not configured");
    if (positions.length() != 3) {
        throw std::invalid_argument("Need exactly 3 positions");
    }
    
    for (int i = 0; i < 3; ++i) {
        rotors[i].setPosition(toupper(positions[i]) - 'A');
    }
}

void EnigmaMachine::rotateRotors() {
    bool shouldRotateNext = rotors[0].rotate();
    for (int i = 1; i < 3 && shouldRotateNext; ++i) {
        shouldRotateNext = rotors[i].rotate();
    }
}

char EnigmaMachine::processChar(char c) {
    if (!isConfigured) throw std::logic_error("Machine not configured");
    if (!isalpha(c)) return c;
    
    rotateRotors();
    c = plugboard.plug(toupper(c));
    
    for (auto& rotor : rotors) c = rotor.encryptForward(c);
    c = reflector.reflect(c);
    for (auto it = rotors.rbegin(); it != rotors.rend(); ++it) c = it->encryptBackward(c);
    
    return plugboard.plug(c);
}

std::string EnigmaMachine::process(const std::string& text) {
    std::string result;
    for (char c : text) {
        result += processChar(c);
    }
    return result;
}