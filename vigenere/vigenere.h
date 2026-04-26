#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

enum GammaMode {
    REPEAT_KEY,
    AUTOKEY_PLAINTEXT,
    AUTOKEY_CIPHERTEXT
};

std::string encrypt(const std::string& text, const std::string& key, GammaMode mode);
std::string decrypt(const std::string& text, const std::string& key, GammaMode mode);

std::string modeToString(GammaMode mode);

#endif