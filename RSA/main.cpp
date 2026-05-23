#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "rsa.hpp"
#include "utils.hpp"

using namespace utils;

void printBanner() {
    std::cout << "========================================\n";
    std::cout << "           RSA CRYPTOSYSTEM\n";
    std::cout << "========================================\n";
}

void showHelp() {
    std::cout << "\nCommands:\n";
    std::cout << "  gen <bits>           Generate RSA keys\n";
    std::cout << "  save <file>          Save keys\n";
    std::cout << "  load <file>          Load keys\n";
    std::cout << "  show                 Show current keys\n";
    std::cout << "  enc <in> <out>       Encrypt file\n";
    std::cout << "  dec <in> <out>       Decrypt file\n";
    std::cout << "  help                 Show help\n";
    std::cout << "  exit                 Exit program\n";
}

std::string truncateString(const std::string& str, size_t maxLen = 80) {
    if (str.length() <= maxLen) {
        return str;
    }

    return str.substr(0, maxLen / 2) +
           "..." +
           str.substr(str.length() - maxLen / 2);
}

int main() {
    RSA rsa;

    KeyPair currentKeys;
    bool keysLoaded = false;

    printBanner();
    showHelp();

    std::string command;

    while (true) {
        std::cout << "\n> ";
        std::cin >> command;

        try {

            // =====================================================
            // GENERATE KEYS
            // =====================================================

            if (command == "gen") {
                int bits;
                std::cin >> bits;

                std::cout << "Generating " << bits << "-bit RSA keys...\n";

                rsa.generateKeys(bits);

                currentKeys = {
                    rsa.getPublicKey().first,
                    rsa.getPrivateKey().first,
                    rsa.getPublicKey().second
                };

                keysLoaded = true;

                std::cout << "Keys generated successfully.\n";
            }

            // =====================================================
            // SAVE KEYS
            // =====================================================

            else if (command == "save") {

                if (!keysLoaded) {
                    std::cout << "No keys loaded.\n";
                    continue;
                }

                std::string filename;
                std::cin >> filename;

                saveKeysToFile(filename, currentKeys);

                std::cout << "Keys saved to " << filename << "\n";
            }

            // =====================================================
            // LOAD KEYS
            // =====================================================

            else if (command == "load") {

                std::string filename;
                std::cin >> filename;

                currentKeys = loadKeysFromFile(filename);

                keysLoaded = true;

                std::cout << "Keys loaded from " << filename << "\n";
            }

            // =====================================================
            // SHOW KEYS
            // =====================================================

            else if (command == "show") {

                if (!keysLoaded) {
                    std::cout << "No keys loaded.\n";
                    continue;
                }

                std::cout << "\n=== RSA KEYS ===\n";

                std::cout << "n = "
                          << truncateString(currentKeys.n.toString())
                          << "\n";

                std::cout << "e = "
                          << currentKeys.e.toString()
                          << "\n";

                std::cout << "d = "
                          << truncateString(currentKeys.d.toString())
                          << "\n";
            }

            // =====================================================
            // ENCRYPT FILE
            // =====================================================

            else if (command == "enc") {

                if (!keysLoaded) {
                    std::cout << "No keys loaded.\n";
                    continue;
                }

                std::string inputFile;
                std::string outputFile;

                std::cin >> inputFile >> outputFile;

                std::cout << "Reading plaintext...\n";

                std::vector<uint8_t> plaintext =
                    readFileToBytes(inputFile);

                std::cout << "Splitting into blocks...\n";

                std::vector<std::vector<uint8_t>> plainBlocks =
                    splitBytesIntoBlocks(plaintext, currentKeys.n);

                std::cout << "Encrypting "
                          << plainBlocks.size()
                          << " blocks...\n";

                std::ofstream out(outputFile);

                if (!out.is_open()) {
                    throw std::runtime_error(
                        "Cannot open output file"
                    );
                }

                for (const auto& blockBytes : plainBlocks) {

                    BigInt message =
                        bytesToBigInt(blockBytes);

                    BigInt cipher =
                        message.modPow(
                            currentKeys.e,
                            currentKeys.n
                        );

                    out << bigIntToHex(cipher) << "\n";
                }

                out.close();

                std::cout << "Encryption complete.\n";
            }

            // =====================================================
            // DECRYPT FILE
            // =====================================================

            else if (command == "dec") {

                if (!keysLoaded) {
                    std::cout << "No keys loaded.\n";
                    continue;
                }

                std::string inputFile;
                std::string outputFile;

                std::cin >> inputFile >> outputFile;

                std::ifstream in(inputFile);

                if (!in.is_open()) {
                    throw std::runtime_error(
                        "Cannot open encrypted file"
                    );
                }

                std::cout << "Reading encrypted blocks...\n";

                std::vector<uint8_t> recoveredBytes;

                std::string line;

                while (std::getline(in, line)) {

                    if (line.empty()) {
                        continue;
                    }

                    BigInt cipher =
                        hexToBigInt(line);

                    BigInt message =
                        cipher.modPow(
                            currentKeys.d,
                            currentKeys.n
                        );

                    std::vector<uint8_t> blockBytes =
                        bigIntToBytesFixed(
                            message,
                            getPlainBlockSize(currentKeys.n)
                        );

                    recoveredBytes.insert(
                        recoveredBytes.end(),
                        blockBytes.begin(),
                        blockBytes.end()
                    );
                }

                in.close();

                std::cout << "Removing padding...\n";

                removePadding(recoveredBytes);

                std::cout << "Writing plaintext...\n";

                writeBytesToFile(
                    outputFile,
                    recoveredBytes
                );

                std::cout << "Decryption complete.\n";
            }

            // =====================================================
            // HELP
            // =====================================================

            else if (command == "help") {
                showHelp();
            }

            // =====================================================
            // EXIT
            // =====================================================

            else if (
                command == "exit" ||
                command == "quit"
            ) {
                std::cout << "Goodbye!\n";
                break;
            }

            // =====================================================
            // UNKNOWN COMMAND
            // =====================================================

            else {
                std::cout
                    << "Unknown command. Type 'help'.\n";
            }

        }
        catch (const std::exception& e) {

            std::cout << "ERROR: "
                      << e.what()
                      << "\n";
        }
    }

    return 0;
}