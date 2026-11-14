#include <iostream>
#include <string>
#include <limits>
#include "vigenere.h"

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void showMenu() {
    cout << "\n========== VIGENERE ==========\n";
    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "3. Enter key\n";
    cout << "4. Choose gamma mode\n";
    cout << "5. Exit\n";
    cout << "==============================\n";
    cout << "Choice: ";
}

GammaMode chooseMode() {
    clearScreen();

    cout << "Choose gamma mode:\n";
    cout << "1. REPEAT_KEY\n";
    cout << "2. AUTOKEY_PLAINTEXT\n";
    cout << "3. AUTOKEY_CIPHERTEXT\n";
    cout << "Select: ";

    int c;
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (c) {
        case 1: return REPEAT_KEY;
        case 2: return AUTOKEY_PLAINTEXT;
        case 3: return AUTOKEY_CIPHERTEXT;
        default: return REPEAT_KEY;
    }
}

int main() {
    string key = "";
    GammaMode mode = REPEAT_KEY;

    while (true) {
        clearScreen();

        showMenu();

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreen();

        if (choice == 1) {
            if (key.empty()) {
                cout << "ERROR: Set key first!\n";
                pause();
                continue;
            }

            string text;
            cout << "Enter text to encrypt:\n";
            getline(cin, text);

            cout << "\nEncrypted text:\n";
            cout << encrypt(text, key, mode) << "\n";

            pause();
        }

        else if (choice == 2) {
            if (key.empty()) {
                cout << "ERROR: Set key first!\n";
                pause();
                continue;
            }

            string text;
            cout << "Enter cipher text:\n";
            getline(cin, text);

            cout << "\nDecrypted text:\n";
            cout << decrypt(text, key, mode) << "\n";

            pause();
        }

        else if (choice == 3) {
            cout << "Enter key: ";
            cin >> key;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Key set successfully!\n";
            pause();
        }

        else if (choice == 4) {
            mode = chooseMode();
            cout << "Mode set to: " << modeToString(mode) << "\n";
            pause();
        }

        else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        }

        else {
            cout << "Invalid option!\n";
            pause();
        }
    }

    return 0;
}