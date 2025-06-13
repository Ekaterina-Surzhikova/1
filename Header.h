#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Узел для хранения одной цифры числа
struct DigitNode {
    int digit;
    DigitNode* next;

    DigitNode(int d) : digit(d), next(nullptr) {}
};

// Класс для работы с большими числами
class BigNumber {
private:
    DigitNode* head;
    DigitNode* tail;
    int length;

    // Рекурсивное сложение с переносом
    static int addWithCarry(DigitNode* a, DigitNode* b, BigNumber& result, int carry);

public:
    BigNumber() : head(nullptr), tail(nullptr), length(0) {}

    // Добавление цифры в конец числа
    void append(int digit);
    // Загрузка числа из строки
    void fromString(const string& s);
    // Преобразование числа в строку
    string toString() const;

    // Сложение двух больших чисел
    static BigNumber add(const BigNumber& a, const BigNumber& b);
    // Проверка на ведущие нули
    bool hasLeadingZeros() const;
    // Деструктор для освобождения памяти
    ~BigNumber();
};

class CircularNumberRing {
private:
    DigitNode* head;  // Начало кольца
    DigitNode* tail;  // Конец кольца (связывает с head)
    int length;       // Количество цифр

public:
    CircularNumberRing() : head(nullptr), tail(nullptr), length(0) {}

    // Добавление цифры в кольцо
    void append(int digit);

    // Загрузка кольца из строки
    void fromString(const string& s);
    // Получение подпоследовательности цифр из кольца
    BigNumber getSubNumber(DigitNode* start, int len) const;
    // Поиск решения в кольце
    string solve() const;
    // Деструктор для освобождения памяти
    ~CircularNumberRing();

};
// Очистка буфера ввода
void clearInput();
// Вывод главного меню
void showMenu();
// Обработка нецифровых символов
bool handleInvalidInput(const string& input, const string& clean);
// Обработка файлового ввода
void processFile();
// Обработка ручного ввода
void processManual();
