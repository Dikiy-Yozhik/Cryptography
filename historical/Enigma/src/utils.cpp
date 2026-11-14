// utils.cpp
#include "utils.h"
#include <map>
#include <sstream>  
#include <algorithm>

// Определения статических членов
const std::vector<std::string> EnigmaUtils::ROTOR_WIRINGS = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", // Rotor I
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", // Rotor II
    "BDFHJLCPRTXVZNYEIWGAKMUSQO", // Rotor III
    "ESOVPZJAYQUIRHXLNFTGKDCMWB", // Rotor IV
    "VZBRGITYUPSDNHLXAWMJQOFECK", // Rotor V
    "JPGVOUMFYQBENHZRDKASXLICTW", // Rotor VI
    "NZJHGRCXMYSWBOUFAIVLPEKQDT", // Rotor VII
    "FKQHTLXOCBJSPDZRAMEWNIUYGV"  // Rotor VIII
};

const std::vector<int> EnigmaUtils::ROTOR_NOTCHES = {
    16, 4, 21, 9, 25, 12, 12, 12
};

const std::string EnigmaUtils::REFLECTOR_B = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
const std::string EnigmaUtils::REFLECTOR_C = "FVPJIAOYEDRZXWGCTKUQSBNMHL";

// Реализации методов
bool EnigmaUtils::validateRotorPositions(const std::string& positions) {
    if (positions.length() != 3) return false;
        for (char c : positions) {
            if (!isalpha(c)) return false;
        }
    return true;
}

bool EnigmaUtils::validatePlugboardPairs(const std::string& pairs) {
    std::map<char, bool> usedChars;
        std::istringstream iss(pairs);
        std::string pair;
        
        while (iss >> pair) {
            if (pair.length() != 2) return false;
            if (!isalpha(pair[0]) || !isalpha(pair[1])) return false;
            if (usedChars[pair[0]] || usedChars[pair[1]]) return false;
            
            usedChars[pair[0]] = true;
            usedChars[pair[1]] = true;
        }
    return true;
}

std::string EnigmaUtils::toUpper(const std::string& s) {
    std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
}