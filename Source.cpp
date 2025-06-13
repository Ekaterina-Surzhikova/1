#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"

using namespace std;

// ���� ��� �������� ����� ����� �����
struct DigitNode {
    int digit;
    DigitNode* next;

    DigitNode(int d) : digit(d), next(nullptr) {}
};

// ����� ��� ������ � �������� �������
class BigNumber {
private:
    DigitNode* head;
    DigitNode* tail;
    int length;

    // ����������� �������� � ���������
    static int addWithCarry(DigitNode* a, DigitNode* b, BigNumber& result, int carry) {
        if (!a && !b) return carry;
        int sum = carry + (a ? a->digit : 0) + (b ? b->digit : 0);
        result.append(sum % 10);
        return addWithCarry(a ? a->next : nullptr, b ? b->next : nullptr, result, sum / 10);
    }

public:
    BigNumber() : head(nullptr), tail(nullptr), length(0) {}

    // ���������� ����� � ����� �����
    void append(int digit) {
        DigitNode* newNode = new DigitNode(digit);
        if (!tail) head = tail = newNode;
        else tail = tail->next = newNode;
        length++;
    }

    // �������� ����� �� ������
    void fromString(const string& s) {
        for (char c : s) if (isdigit(c)) append(c - '0');
    }

    // �������������� ����� � ������
    string toString() const {
        string result;
        for (DigitNode* curr = head; curr; curr = curr->next)
            result += to_string(curr->digit);
        return result.empty() ? "0" : result;
    }

    // �������� ���� ������� �����
    static BigNumber add(const BigNumber& a, const BigNumber& b) {
        BigNumber result;
        if (addWithCarry(a.head, b.head, result, 0)) result.append(1);
        return result;
    }

    // �������� �� ������� ����
    bool hasLeadingZeros() const {
        return length > 1 && head->digit == 0;  // ����� �� ������ ���������� � ����, ���� ��� ����� > 1
    }

    // ���������� ��� ������������ ������
    ~BigNumber() {
        while (head) {
            DigitNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// ��������� ������ ����
class CircularNumberRing {
private:
    DigitNode* head;  // ������ ������
    DigitNode* tail;  // ����� ������ (��������� � head)
    int length;       // ���������� ����

public:
    CircularNumberRing() : head(nullptr), tail(nullptr), length(0) {}

    // ���������� ����� � ������
    void append(int digit) {
        DigitNode* newNode = new DigitNode(digit);
        if (!tail) {
            head = tail = newNode;
            tail->next = head;  // �������� ������
        }
        else {
            tail->next = newNode;
            newNode->next = head;
            tail = newNode;
        }
        length++;
    }

    // �������� ������ �� ������
    void fromString(const string& s) {
        for (char c : s) if (isdigit(c)) append(c - '0');
    }

    // ��������� ��������������������� ���� �� ������
    BigNumber getSubNumber(DigitNode* start, int len) const {
        BigNumber num;
        DigitNode* curr = start;
        for (int i = 0; i < len; ++i) {
            num.append(curr->digit);
            curr = curr->next;
        }
        return num;
    }

    // ����� ������� � ������
    string solve() const {
        if (length < 3) return "No";  // ������� 3 ����� ��� A, B � C

        // ���������� ��� ��������� ��������� ������� � ����� A � B
        DigitNode* start = head;
        for (int i = 0; i < length; ++i) {
            for (int lenA = 1; lenA <= length - 2; ++lenA) {
                for (int lenB = 1; lenB <= length - lenA - 1; ++lenB) {
                    int lenC = length - lenA - lenB;
                    if (lenC <= 0) continue;

                    // �������� ����� A, B � C �� ������
                    BigNumber A = getSubNumber(start, lenA);
                    DigitNode* bStart = start;
                    for (int j = 0; j < lenA; ++j) bStart = bStart->next;
                    BigNumber B = getSubNumber(bStart, lenB);
                    DigitNode* cStart = bStart;
                    for (int j = 0; j < lenB; ++j) cStart = cStart->next;
                    BigNumber C = getSubNumber(cStart, lenC);

                    // ��������� ������� ����
                    if (A.hasLeadingZeros() || B.hasLeadingZeros() || C.hasLeadingZeros())
                        continue;

                    // ���������, ����������� �� A + B = C
                    if (BigNumber::add(A, B).toString() == C.toString()) {
                        return A.toString() + "+" + B.toString() + "=" + C.toString();
                    }
                }
            }
            start = start->next;  // ��������� � ��������� ��������� �������
        }
        return "No";  // ������� �� �������
    }

    // ���������� ��� ������������ ������
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

// ������� ������ �����
void clearInput() {
    cin.clear();
    while (cin.get() != '\n');
}

// ����� �������� ����
void showMenu() {
    cout << "\n=== ���������: �������� ������ ===\n"
        << "1. ������ �� �����\n"
        << "2. ������ ����������\n"
        << "3. �����\n"
        << "�����: ";
}

// ��������� ���������� ��������
bool handleInvalidInput(const string& input, const string& clean) {
    cout << "\n������� ���������� �������:\n"
        << "�������� ������: " << input << "\n"
        << "��������� ������: " << clean << "\n\n"
        << "1. ������������ ��������� ������\n"
        << "2. ��������� ���������\n"
        << "�����: ";

    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        clearInput();
        cout << "������� 1 ��� 2: ";
    }
    clearInput();
    return choice == 1;
}

// ��������� ��������� �����
void processFile() {
    ifstream in("input.txt");
    ofstream out("output.txt", ios::app);

    if (!in) {
        cout << "���� input.txt �� ������!\n";
        return;
    }

    if (in.peek() == EOF) {
        cout << "���� ������!\n";
        return;
    }

    cout << "\n������ �����:\n";
    string line;
    bool hasValidData = false;

    while (getline(in, line)) {
        cout << "������: " << line << endl;

        string clean;
        for (char c : line) {
            if (isdigit(c)) clean += c;
        }

        if (clean.empty()) {
            cout << "������������ ������ (��� ����)\n";
            continue;
        }

        if (clean.length() < 3) {
            cout << "������������ ������ (������ " << clean.length()
                << " ����): " << clean << "\n";
            continue;
        }

        if (line != clean && !handleInvalidInput(line, clean)) {
            cout << "��������� �������� �������������\n";
            break;
        }

        CircularNumberRing ring;
        ring.fromString(clean);
        string res = ring.solve();
        cout << "���������: " << res << "\n\n";
        out << clean << " -> " << res << endl;
        hasValidData = true;
    }

    if (!hasValidData) {
        cout << "���� �� �������� ���������� ������ (������� 3 ����� � ������)!\n";
    }
    else {
        cout << "���������� ��������� � output.txt\n";
    }
}

// ��������� ������� �����
void processManual() {
    cout << "\n������� ������: ";
    string input;
    getline(cin, input);

    string clean;
    for (char c : input) if (isdigit(c)) clean += c;

    if (input != clean && !handleInvalidInput(input, clean)) {
        cout << "���� �������\n";
        return;
    }

    if (clean.length() < 3) {
        cout << "����� ������� 3 �����!\n";
        return;
    }

    CircularNumberRing ring;
    ring.fromString(clean);
    string res = ring.solve();
    cout << "���������: " << res << endl;

    ofstream out("output.txt", ios::app);
    out << clean << " -> " << res << endl;
    cout << "��������� � output.txt\n";
}
