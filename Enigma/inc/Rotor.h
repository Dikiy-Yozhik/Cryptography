#pragma once
#include <string>
#include <unordered_map>

class Rotor {
private:
    std::string wiring;         // Проводка типа "EKMFLGDQVZNTOWYHXUSPAIBRCJ"
    std::string inverseWiring;  // Обратная проводка (для обратного хода)
    int currentPosition;        // Текущая позиция (0-25)
    int notchPosition;          // Позиция выемки (например, 16 для 'Q')
    int ringSetting;            // Настройка кольца (доп.смещение)

    // Генерирует обратную проводку
    void generateInverseWiring();

public:
    // Конструктор (проводка, выемка, начальная позиция)
    Rotor(const std::string& wiring, int notch, int startPos = 0, int ring = 0);

    // Установка позиции
    void setPosition(int pos);

    // Шифрование в прямом направлении (клавиатура -> рефлектор)
    char encryptForward(char c) const;

    // Шифрование в обратном направлении (рефлектор -> клавиатура)
    char encryptBackward(char c) const;

    // Вращение ротора (возвращает true, если следующий ротор должен сдвинуться)
    bool rotate();

    // Проверка, находится ли ротор на выемке
    bool isAtNotch() const;

    // Получение текущей позиции
    char getPosition() const { return 'A' + currentPosition; }
};