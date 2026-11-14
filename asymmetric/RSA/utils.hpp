#pragma once

#include "big_int.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

namespace utils {

    // =====================================================
    // FILE INPUT / OUTPUT
    // =====================================================

    inline std::vector<uint8_t> readFileToBytes(
        const std::string& filename
    ) {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open file: " + filename
            );
        }

        return std::vector<uint8_t>(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }

    inline void writeBytesToFile(
        const std::string& filename,
        const std::vector<uint8_t>& data
    ) {
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot write file: " + filename
            );
        }

        file.write(
            reinterpret_cast<const char*>(data.data()),
            data.size()
        );
    }

    // =====================================================
    // KEY STORAGE
    // =====================================================

    struct KeyPair {
        BigInt e;
        BigInt d;
        BigInt n;
    };

        inline std::string bigIntToDecimal(BigInt num) {
        if (num == BigInt(0)) {
            return "0";
        }

        bool negative = false;
        if (num < BigInt(0)) {
            negative = true;
            num = -num;
        }

        std::string result;

        while (num > BigInt(0)) {
            BigInt digit = num % BigInt(10);
            result.push_back('0' + std::stoi(digit.toString(), nullptr, 16));
            num = num / BigInt(10);
        }

        if (negative) {
            result.push_back('-');
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    inline void saveKeysToFile(const std::string& filename, const KeyPair& keys) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Cannot save keys");
        }

        file << "n=" << bigIntToDecimal(keys.n) << "\n";
        file << "e=" << bigIntToDecimal(keys.e) << "\n";
        file << "d=" << bigIntToDecimal(keys.d) << "\n";
    }

    inline KeyPair loadKeysFromFile(
        const std::string& filename
    ) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot load keys"
            );
        }

        KeyPair keys;

        std::string line;

        while (std::getline(file, line)) {

            if (line.find("n=") == 0) {
                keys.n = BigInt(line.substr(2));
            }
            else if (line.find("e=") == 0) {
                keys.e = BigInt(line.substr(2));
            }
            else if (line.find("d=") == 0) {
                keys.d = BigInt(line.substr(2));
            }
        }

        return keys;
    }

    // =====================================================
    // BLOCK SIZE
    // =====================================================

   inline size_t getPlainBlockSize(const BigInt& n) {
        size_t k = 0;

        BigInt limit(1);

        while (limit < n) {
            limit = limit * BigInt(256);
            k++;
        }

        if (k == 0) {
            return 1;
        }

        size_t blockSize = k - 1;

        if (blockSize == 0) {
            return 1;
        }

        return blockSize;
    }

    // =====================================================
    // PADDING
    // =====================================================

    inline void applyPadding(
        std::vector<uint8_t>& block,
        size_t targetSize
    ) {
        while (block.size() < targetSize) {
            block.push_back(0x00);
        }
    }

    inline void removePadding(
        std::vector<uint8_t>& data
    ) {
        while (!data.empty() && data.back() == 0x00) {
            data.pop_back();
        }
    }

    // =====================================================
    // MESSAGE SPLITTING
    // =====================================================

    inline std::vector<std::vector<uint8_t>> splitBytesIntoBlocks(
        const std::vector<uint8_t>& data, const BigInt& n) {
        size_t blockSize = getPlainBlockSize(n);

        std::vector<std::vector<uint8_t>> blocks;

        for (size_t i = 0; i < data.size(); i += blockSize) {

            size_t end = std::min(i + blockSize, data.size());

            std::vector<uint8_t> block(data.begin() + i, data.begin() + end);

            applyPadding(block, blockSize);

            blocks.push_back(block);
        }

        return blocks;
    }

    // =====================================================
    // BYTES <-> BIGINT
    // =====================================================

    inline BigInt bytesToBigInt(
        const std::vector<uint8_t>& bytes
    ) {
        return BigInt(bytes);
    }

    inline std::vector<uint8_t> bigIntToBytes(
        const BigInt& num
    ) {
        std::vector<uint32_t> digits = num.getDigits();

        std::vector<uint8_t> result;

        for (uint32_t d : digits) {

            result.push_back(d & 0xFF);
            result.push_back((d >> 8) & 0xFF);
            result.push_back((d >> 16) & 0xFF);
            result.push_back((d >> 24) & 0xFF);
        }

        while (!result.empty() && result.back() == 0x00) {
            result.pop_back();
        }

        return result;
    }

    inline std::vector<uint8_t> bigIntToBytesFixed(
        const BigInt& num,
        size_t blockSize
    ) {
        std::vector<uint8_t> bytes = bigIntToBytes(num);

        if (bytes.size() > blockSize) {
            throw std::runtime_error(
                "Block size overflow"
            );
        }

        bytes.resize(blockSize, 0x00);

        return bytes;
    }

    // =====================================================
    // HEX <-> BIGINT
    // =====================================================

    inline std::string bigIntToHex(
        const BigInt& num
    ) {
        std::vector<uint8_t> bytes = bigIntToBytes(num);

        static const char* hexChars = "0123456789ABCDEF";

        std::string result;

        for (uint8_t b : bytes) {

            result += hexChars[(b >> 4) & 0x0F];
            result += hexChars[b & 0x0F];
        }

        return result;
    }

    inline BigInt hexToBigInt(
        const std::string& hex
    ) {
        if (hex.length() % 2 != 0) {
            throw std::runtime_error(
                "Invalid hex string"
            );
        }

        std::vector<uint8_t> bytes;

        for (size_t i = 0; i < hex.length(); i += 2) {

            std::string byteString = hex.substr(i, 2);

            uint8_t byte = static_cast<uint8_t>(
                std::stoul(byteString, nullptr, 16)
            );

            bytes.push_back(byte);
        }

        return bytesToBigInt(bytes);
    }

}
