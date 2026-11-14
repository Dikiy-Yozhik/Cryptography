#include "Rotor.h"
#include <stdexcept>
#include <algorithm>

Rotor::Rotor(const std::string& wiring, int notch, int startPos, int ring) 
    : wiring(wiring), notchPosition(notch), currentPosition(startPos), ringSetting(ring) {
    if (wiring.length() != 26) {
        throw std::invalid_argument("Wiring must have exactly 26 characters");
    }
    generateInverseWiring();
}

void Rotor::generateInverseWiring() {
    inverseWiring.resize(26);
    for (int i = 0; i < 26; ++i) {
        char encrypted = wiring[i];
        inverseWiring[encrypted - 'A'] = 'A' + i;
    }
}

void Rotor::setPosition(int pos) {
    currentPosition = pos % 26;
}

char Rotor::encryptForward(char c) const {
    int input = (toupper(c) - 'A' + currentPosition - ringSetting + 26) % 26;
    char encrypted = wiring[input];
    return 'A' + (encrypted - 'A' - currentPosition + ringSetting + 26) % 26;
}

char Rotor::encryptBackward(char c) const {
    int input = (toupper(c) - 'A' + currentPosition - ringSetting + 26) % 26;
    char encrypted = inverseWiring[input];
    return 'A' + (encrypted - 'A' - currentPosition + ringSetting + 26) % 26;
}

bool Rotor::rotate() {
    bool shouldRotateNext = isAtNotch();
    currentPosition = (currentPosition + 1) % 26;
    return shouldRotateNext;
}

bool Rotor::isAtNotch() const {
    return currentPosition == notchPosition;
}