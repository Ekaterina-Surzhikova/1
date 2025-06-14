#include "Header.h"

// Реализация методов InputValidator
InputValidator::InputValidator(const string& input) {
    originalInput = input;
    cleanedInput = "";
    hasNonDigits = false;

    for (char c : originalInput) {
        if (isdigit(c)) {
            cleanedInput += c;
        }
        else {
            hasNonDigits = true;
        }
    }
}

// Проверяет, содержит ли строка минимальное количество цифр
bool InputValidator::hasMinimumDigits(int minDigits) const {
    return cleanedInput.length() >= minDigits;
}

// Проверяет наличие нецифровых символов
bool InputValidator::containsNonDigits() const {
    return hasNonDigits;
}

// Возвращает очищенную строку (только цифры)
string InputValidator::getCleanedInput() const {
    return cleanedInput;
}

// Возвращает исходную строку
string InputValidator::getOriginalInput() const {
    return originalInput;
}

// Статический метод для быстрой проверки валидности строки
bool InputValidator::isValid(const string& input, int minDigits) {
    int digitCount = 0;
    for (char c : input) {
        if (isdigit(c)) {
            digitCount++;
            if (digitCount >= minDigits) {
                return true;
            }
        }
    }
    return false;
}

// Реализация DigitNode
DigitNode::DigitNode(int d) : digit(d), next(nullptr) {}

// Реализация методов BigNumber
BigNumber::BigNumber() : head(nullptr), tail(nullptr), length(0) {}

// Добавляет цифру в конец числа (младший разряд)
void BigNumber::append(int digit) {
    DigitNode* newNode = new DigitNode(digit);
    if (!tail) head = tail = newNode;
    else {
        tail->next = newNode;
        tail = newNode;
    }
    length++;
}

// Создает число из строки (цифры хранятся в обратном порядке)
void BigNumber::fromString(const string& s) {
    for (int i = s.length() - 1; i >= 0; --i) {
        if (isdigit(s[i])) append(s[i] - '0');
    }
}

// Возвращает строковое представление числа
string BigNumber::toString() const {
    string result;
    for (DigitNode* curr = head; curr; curr = curr->next) {
        result += to_string(curr->digit);
    }
    reverse(result.begin(), result.end());
    return result.empty() ? "0" : result;
}

// Статический метод для сложения двух больших чисел
BigNumber BigNumber::add(const BigNumber& a, const BigNumber& b) {
    return addLists(a.head, b.head);
}

BigNumber BigNumber::addLists(DigitNode* a, DigitNode* b) {
    BigNumber result;
    int carry = 0;

    while (a || b || carry) {
        int sum = carry;
        if (a) {
            sum += a->digit;
            a = a->next;
        }
        if (b) {
            sum += b->digit;
            b = b->next;
        }

        result.append(sum % 10);
        carry = sum / 10;
    }

    return result;
}

// Проверяет наличие ведущих нулей
bool BigNumber::hasLeadingZeros() const {
    return length > 1 && tail->digit == 0;
}

// Деструктор - освобождает память
BigNumber::~BigNumber() {
    while (head) {
        DigitNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// Реализация методов CircularNumberRing
CircularNumberRing::CircularNumberRing() : head(nullptr), tail(nullptr), length(0) {}

// Добавляет цифру в кольцо
void CircularNumberRing::append(int digit) {
    DigitNode* newNode = new DigitNode(digit);
    if (!tail) {
        head = tail = newNode;
        tail->next = head;
    }
    else {
        tail->next = newNode;
        newNode->next = head;
        tail = newNode;
    }
    length++;
}

// Создает кольцо из строки цифр
void CircularNumberRing::fromString(const string& s) {
    for (char c : s) if (isdigit(c)) append(c - '0');
}

// Получает подчисло из кольца, начиная с указанного узла
BigNumber CircularNumberRing::getSubNumber(DigitNode* start, int len) const {
    BigNumber num;
    DigitNode* curr = start;
    for (int i = 0; i < len; ++i) {
        num.append(curr->digit);
        curr = curr->next;
    }
    return num;
}

// Решает задачу поиска комбинации A+B=C в числовом кольце
string CircularNumberRing::solve() const {
    if (length < 3) return "No";

    string numStr;
    DigitNode* curr = head;
    for (int i = 0; i < length; ++i) {
        numStr += to_string(curr->digit);
        curr = curr->next;
    }

    for (int lenA = 1; lenA <= length - 2; ++lenA) {
        for (int lenB = 1; lenB <= length - lenA - 1; ++lenB) {
            int lenC = length - lenA - lenB;
            if (lenC <= 0) continue;

            string aStr = numStr.substr(0, lenA);
            string bStr = numStr.substr(lenA, lenB);
            string cStr = numStr.substr(lenA + lenB, lenC);

            if ((aStr.size() > 1 && aStr[0] == '0') ||
                (bStr.size() > 1 && bStr[0] == '0') ||
                (cStr.size() > 1 && cStr[0] == '0')) {
                continue;
            }

            BigNumber A, B, C;
            A.fromString(aStr);
            B.fromString(bStr);
            C.fromString(cStr);

            BigNumber sum = BigNumber::add(A, B);
            if (sum.toString() == C.toString()) {
                return aStr + "+" + bStr + "=" + cStr;
            }
        }
    }

    for (int shift = 1; shift < length; ++shift) {
        string shiftedStr = numStr.substr(shift) + numStr.substr(0, shift);

        for (int lenA = 1; lenA <= length - 2; ++lenA) {
            for (int lenB = 1; lenB <= length - lenA - 1; ++lenB) {
                int lenC = length - lenA - lenB;
                if (lenC <= 0) continue;

                string aStr = shiftedStr.substr(0, lenA);
                string bStr = shiftedStr.substr(lenA, lenB);
                string cStr = shiftedStr.substr(lenA + lenB, lenC);

                if ((aStr.size() > 1 && aStr[0] == '0') ||
                    (bStr.size() > 1 && bStr[0] == '0') ||
                    (cStr.size() > 1 && cStr[0] == '0')) {
                    continue;
                }

                BigNumber A, B, C;
                A.fromString(aStr);
                B.fromString(bStr);
                C.fromString(cStr);

                BigNumber sum = BigNumber::add(A, B);
                if (sum.toString() == C.toString()) {
                    return aStr + "+" + bStr + "=" + cStr;
                }
            }
        }
    }

    return "No";
}

// Деструктор - освобождает память
CircularNumberRing::~CircularNumberRing() {
    if (!head) return;
    DigitNode* curr = head->next;
    while (curr != head) {
        DigitNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
    delete head;
}

// Очищает буфер ввода
void clearInput() {
    cin.clear();
    while (cin.get() != '\n');
}


// Обрабатывает невалидный ввод, предлагая пользователю выбор
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

// Обрабатывает ввод из файла
void processFile() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
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

        InputValidator validator(line);

        if (!validator.hasMinimumDigits()) {
            cout << "Недопустимая строка (только " << validator.getCleanedInput().length()
                << " цифр): " << validator.getCleanedInput() << "\n";
            continue;
        }

        if (validator.containsNonDigits() && !handleInvalidInput(validator.getOriginalInput(), validator.getCleanedInput())) {
            cout << "Обработка отменена пользователем\n";
            break;
        }

        CircularNumberRing ring;
        ring.fromString(validator.getCleanedInput());
        string res = ring.solve();
        cout << "Результат: " << res << "\n\n";
        out << validator.getCleanedInput() << " -> " << res << endl;
        hasValidData = true;
    }

    if (!hasValidData) {
        cout << "Файл не содержит подходящих данных (минимум 3 цифры в строке)!\n";
    }
    else {
        cout << "Результаты сохранены в output.txt\n";
    }
}

// Обрабатывает ручной ввод пользователя
void processManual() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "\nВведите строку: ";
    string input;
    getline(cin, input);

    InputValidator validator(input);

    if (!validator.hasMinimumDigits()) {
        cout << "Нужно минимум 3 цифры!\n";
        return;
    }

    if (validator.containsNonDigits() && !handleInvalidInput(validator.getOriginalInput(), validator.getCleanedInput())) {
        cout << "Ввод отменен\n";
        return;
    }

    CircularNumberRing ring;
    ring.fromString(validator.getCleanedInput());
    string res = ring.solve();
    cout << "Результат: " << res << endl;

    ofstream out("output.txt", ios::app);
    out << validator.getCleanedInput() << " -> " << res << endl;
    cout << "Сохранено в output.txt\n";
}

// Отображает главное меню программы
void showMenu() {
    cout << "\n=== Программа: Числовое кольцо ===\n"
        << "1. Чтение из файла\n"
        << "2. Ручное заполнение\n"
        << "3. Выход\n"
        << "Выбор: ";
}
