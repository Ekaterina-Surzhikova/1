#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"

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
    static int addWithCarry(DigitNode* a, DigitNode* b, BigNumber& result, int carry) {
        if (!a && !b) return carry;
        int sum = carry + (a ? a->digit : 0) + (b ? b->digit : 0);
        result.append(sum % 10);
        return addWithCarry(a ? a->next : nullptr, b ? b->next : nullptr, result, sum / 10);
    }

public:
    BigNumber() : head(nullptr), tail(nullptr), length(0) {}

    // Добавление цифры в конец числа
    void append(int digit) {
        DigitNode* newNode = new DigitNode(digit);
        if (!tail) head = tail = newNode;
        else tail = tail->next = newNode;
        length++;
    }

    // Загрузка числа из строки
    void fromString(const string& s) {
        for (char c : s) if (isdigit(c)) append(c - '0');
    }

    // Преобразование числа в строку
    string toString() const {
        string result;
        for (DigitNode* curr = head; curr; curr = curr->next)
            result += to_string(curr->digit);
        return result.empty() ? "0" : result;
    }

    // Сложение двух больших чисел
    static BigNumber add(const BigNumber& a, const BigNumber& b) {
        BigNumber result;
        if (addWithCarry(a.head, b.head, result, 0)) result.append(1);
        return result;
    }

    // Проверка на ведущие нули
    bool hasLeadingZeros() const {
        return length > 1 && head->digit == 0;  // Число не должно начинаться с нуля, если его длина > 1
    }

    // Деструктор для освобождения памяти
    ~BigNumber() {
        while (head) {
            DigitNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Кольцевой список цифр
class CircularNumberRing {
private:
    DigitNode* head;  // Начало кольца
    DigitNode* tail;  // Конец кольца (связывает с head)
    int length;       // Количество цифр

public:
    CircularNumberRing() : head(nullptr), tail(nullptr), length(0) {}

    // Добавление цифры в кольцо
    void append(int digit) {
        DigitNode* newNode = new DigitNode(digit);
        if (!tail) {
            head = tail = newNode;
            tail->next = head;  // Замыкаем кольцо
        }
        else {
            tail->next = newNode;
            newNode->next = head;
            tail = newNode;
        }
        length++;
    }

    // Загрузка кольца из строки
    void fromString(const string& s) {
        for (char c : s) if (isdigit(c)) append(c - '0');
    }

    // Получение подпоследовательности цифр из кольца
    BigNumber getSubNumber(DigitNode* start, int len) const {
        BigNumber num;
        DigitNode* curr = start;
        for (int i = 0; i < len; ++i) {
            num.append(curr->digit);
            curr = curr->next;
        }
        return num;
    }

    // Поиск решения в кольце
    string solve() const {
        if (length < 3) return "No";  // Минимум 3 цифры для A, B и C

        // Перебираем все возможные начальные позиции и длины A и B
        DigitNode* start = head;
        for (int i = 0; i < length; ++i) {
            for (int lenA = 1; lenA <= length - 2; ++lenA) {
                for (int lenB = 1; lenB <= length - lenA - 1; ++lenB) {
                    int lenC = length - lenA - lenB;
                    if (lenC <= 0) continue;

                    // Получаем числа A, B и C из кольца
                    BigNumber A = getSubNumber(start, lenA);
                    DigitNode* bStart = start;
                    for (int j = 0; j < lenA; ++j) bStart = bStart->next;
                    BigNumber B = getSubNumber(bStart, lenB);
                    DigitNode* cStart = bStart;
                    for (int j = 0; j < lenB; ++j) cStart = cStart->next;
                    BigNumber C = getSubNumber(cStart, lenC);

                    // Проверяем ведущие нули
                    if (A.hasLeadingZeros() || B.hasLeadingZeros() || C.hasLeadingZeros())
                        continue;

                    // Проверяем, выполняется ли A + B = C
                    if (BigNumber::add(A, B).toString() == C.toString()) {
                        return A.toString() + "+" + B.toString() + "=" + C.toString();
                    }
                }
            }
            start = start->next;  // Переходим к следующей стартовой позиции
        }
        return "No";  // Решение не найдено
    }

    // Деструктор для освобождения памяти
    ~CircularNumberRing() {
        if (!head) return;
        DigitNode* curr = head->next;
        while (curr != head) {
            DigitNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        delete head;
    }
};

// Очистка буфера ввода
void clearInput() {
    cin.clear();
    while (cin.get() != '\n');
}

// Вывод главного меню
void showMenu() {
    cout << "\n=== Программа: Числовое кольцо ===\n"
        << "1. Чтение из файла\n"
        << "2. Ручное заполнение\n"
        << "3. Выход\n"
        << "Выбор: ";
}

// Обработка нецифровых символов
bool handleInvalidInput(const string& input, const string& clean) {
    cout << "\nНайдены нецифровые символы:\n"
        << "Исходная строка: " << input << "\n"
        << "Очищенная строка: " << clean << "\n\n"
        << "1. Использовать очищенную строку\n"
        << "2. Завершить программу\n"
        << "Выбор: ";

    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        clearInput();
        cout << "Введите 1 или 2: ";
    }
    clearInput();
    return choice == 1;
}

// Обработка файлового ввода
void processFile() {
    ifstream in("input.txt");
    ofstream out("output.txt", ios::app);

    if (!in) {
        cout << "Файл input.txt не найден!\n";
        return;
    }

    if (in.peek() == EOF) {
        cout << "Файл пустой!\n";
        return;
    }

    cout << "\nЧтение файла:\n";
    string line;
    bool hasValidData = false;

    while (getline(in, line)) {
        cout << "Строка: " << line << endl;

        string clean;
        for (char c : line) {
            if (isdigit(c)) clean += c;
        }

        if (clean.empty()) {
            cout << "Недопустимая строка (нет цифр)\n";
            continue;
        }

        if (clean.length() < 3) {
            cout << "Недопустимая строка (только " << clean.length()
                << " цифр): " << clean << "\n";
            continue;
        }

        if (line != clean && !handleInvalidInput(line, clean)) {
            cout << "Обработка отменена пользователем\n";
            break;
        }

        CircularNumberRing ring;
        ring.fromString(clean);
        string res = ring.solve();
        cout << "Результат: " << res << "\n\n";
        out << clean << " -> " << res << endl;
        hasValidData = true;
    }

    if (!hasValidData) {
        cout << "Файл не содержит подходящих данных (минимум 3 цифры в строке)!\n";
    }
    else {
        cout << "Результаты сохранены в output.txt\n";
    }
}

// Обработка ручного ввода
void processManual() {
    cout << "\nВведите строку: ";
    string input;
    getline(cin, input);

    string clean;
    for (char c : input) if (isdigit(c)) clean += c;

    if (input != clean && !handleInvalidInput(input, clean)) {
        cout << "Ввод отменен\n";
        return;
    }

    if (clean.length() < 3) {
        cout << "Нужно минимум 3 цифры!\n";
        return;
    }

    CircularNumberRing ring;
    ring.fromString(clean);
    string res = ring.solve();
    cout << "Результат: " << res << endl;

    ofstream out("output.txt", ios::app);
    out << clean << " -> " << res << endl;
    cout << "Сохранено в output.txt\n";
}
