#include "EnigmaMachine.h"
#include <iostream>
#include <algorithm>

void showMenu() {
    std::cout << "\n=== ENIGMA MENU ===" << std::endl;
    std::cout << "1. Setup Enigma" << std::endl;
    std::cout << "2. Encrypt/Decrypt text" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choose option: ";
}

int main() {
    EnigmaMachine enigma;
    std::string text;
    
    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string rotorPos, plugPairs;
                std::cout << "Enter initial rotor positions (3 letters, e.g., ABC): ";
                std::getline(std::cin, rotorPos);
                
                std::cout << "Enter Plugboard pairs (e.g., AB CD EF): ";
                std::getline(std::cin, plugPairs);
                
                enigma.setup(rotorPos, plugPairs);
                break;
            }
                
            case 2: {
                if (!enigma.isReady()) {
                    std::cout << "Error: Enigma not configured!" << std::endl;
                    break;
                }
                
                std::cout << "Enter text (type 'stop' to pause, 'exit' to end):\n";
                while (true) {
                    std::getline(std::cin, text);
                    std::transform(text.begin(), text.end(), text.begin(), ::toupper);

                    if (text == "STOP") break;
                    if (text == "EXIT") return 0;
                    
                    std::cout << "Result: " << enigma.process(text) << std::endl;
                }
                break;
            }
                
            case 3:
                std::cout << "Goodbye!" << std::endl;
                return 0;
                
            default:
                std::cout << "Invalid option!" << std::endl;
        }
    }
}