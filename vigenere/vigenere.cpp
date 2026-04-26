#include "vigenere.h"
#include <string>

using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.!?- ";
const int N = ALPHABET.size();

int charToIndex(char c) {
    size_t pos = ALPHABET.find(c);
    if (pos == string::npos) return -1;
    return (int)pos;
}

char indexToChar(int i) {
    return ALPHABET[i % N];
}

string buildRepeatKey(const string& text, const string& key) {
    string gamma;
    for (size_t i = 0; i < text.size(); i++) {
        gamma += key[i % key.size()];
    }
    return gamma;
}

string encrypt(const string& text, const string& key, GammaMode mode) {
    string result;

    for (size_t i = 0; i < text.size(); i++) {
        int p = charToIndex(text[i]);

        if (p == -1) {
            result += text[i];
            continue;
        }

        char g;

        // ===== ГЕНЕРАЦИЯ ГАММЫ =====
        if (mode == REPEAT_KEY) {
            g = buildRepeatKey(text, key)[i];
        }
        else if (mode == AUTOKEY_PLAINTEXT) {
            if (i == 0)
                g = key[0];
            else
                g = text[i - 1];
        }
        else { // AUTOKEY_CIPHERTEXT
            if (i == 0)
                g = key[0];
            else
                g = result[i - 1];
        }

        int k = charToIndex(g);
        int c = (p + k) % N;

        result += indexToChar(c);
    }

    return result;
}

string decrypt(const string& text, const string& key, GammaMode mode) {
    string result;

    for (size_t i = 0; i < text.size(); i++) {
        int c = charToIndex(text[i]);

        if (c == -1) {
            result += text[i];
            continue;
        }

        char g;

        // ===== ГЕНЕРАЦИЯ ГАММЫ =====
        if (mode == REPEAT_KEY) {
            g = buildRepeatKey(text, key)[i];
        }
        else if (mode == AUTOKEY_PLAINTEXT) {
            if (i == 0)
                g = key[0];
            else
                g = result[i - 1];
        }
        else { // AUTOKEY_CIPHERTEXT
            if (i == 0)
                g = key[0];
            else
                g = text[i - 1];
        }

        int k = charToIndex(g);
        int p = (c - k + N) % N;

        result += indexToChar(p);
    }

    return result;
}

string modeToString(GammaMode mode) {
    switch (mode) {
        case REPEAT_KEY: return "REPEAT_KEY";
        case AUTOKEY_PLAINTEXT: return "AUTOKEY_PLAINTEXT";
        case AUTOKEY_CIPHERTEXT: return "AUTOKEY_CIPHERTEXT";
    }
    return "";
}