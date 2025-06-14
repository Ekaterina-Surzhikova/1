#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <windows.h>

using namespace std;

// Класс для проверки и очистки пользовательского ввода
class InputValidator {
private:
    string originalInput;
    string cleanedInput;
    bool hasNonDigits;

public:

    // Конструктор, принимающий входную строку и очищающий её
    InputValidator(const string& input);

    // Проверяет, содержит ли строка минимальное количество цифр
    bool hasMinimumDigits(int minDigits = 3) const;
    
    // Проверяет наличие нецифровых символов
    bool containsNonDigits() const;
    
    // Возвращает очищенную строку (только цифры)
    string getCleanedInput() const;
    
    // Возвращает исходную строку
    string getOriginalInput() const;
    
    // Статический метод для быстрой проверки валидности строки
    static bool isValid(const string& input, int minDigits = 3);
};

// Узел для хранения цифры в связном списке
struct DigitNode {
    int digit;
    DigitNode* next;
    DigitNode(int d);
};

// Класс для работы с большими числами
class BigNumber {
private:
    DigitNode* head;
    DigitNode* tail;
    int length;

    // Внутренний метод для сложения двух чисел, представленных связными списками
    static BigNumber addLists(DigitNode* a, DigitNode* b);

public:

    // Конструктор по умолчанию
    BigNumber();

    // Добавляет цифру в конец числа (младший разряд)
    void append(int digit);

    // Создает число из строки (цифры хранятся в обратном порядке)
    void fromString(const string& s);
   
    // Возвращает строковое представление числа
    string toString() const;
    
    // Статический метод для сложения двух больших чисел
    static BigNumber add(const BigNumber& a, const BigNumber& b);
    
    // Проверяет наличие ведущих нулей
    bool hasLeadingZeros() const;
    
    // Деструктор - освобождает память
    ~BigNumber();
};

// Класс для работы с числовым кольцом
class CircularNumberRing {
private:
    DigitNode* head;
    DigitNode* tail;
    int length;

public:

    // Конструктор по умолчанию
    CircularNumberRing();
    
    // Добавляет цифру в кольцо
    void append(int digit);
    
    // Создает кольцо из строки цифр
    void fromString(const string& s);
    
    // Получает подчисло из кольца, начиная с указанного узла
    BigNumber getSubNumber(DigitNode* start, int len) const;
    
    // Решает задачу поиска комбинации A+B=C в числовом кольце
    string solve() const;
    
    // Деструктор - освобождает память
    ~CircularNumberRing();
};

// Очищает буфер ввода
void clearInput();

// Обрабатывает невалидный ввод, предлагая пользователю выбор
bool handleInvalidInput(const string& input, const string& clean);

// Обрабатывает ввод из файла
void processFile();

// Обрабатывает ручной ввод пользователя
void processManual();

// Отображает главное меню программы
void showMenu();
