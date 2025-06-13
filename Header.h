#pragma once
#include <iostream>
#include <fstream>
#include <string>

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
    static int addWithCarry(DigitNode* a, DigitNode* b, BigNumber& result, int carry);

public:
    BigNumber() : head(nullptr), tail(nullptr), length(0) {}

    // ���������� ����� � ����� �����
    void append(int digit);
    // �������� ����� �� ������
    void fromString(const string& s);
    // �������������� ����� � ������
    string toString() const;

    // �������� ���� ������� �����
    static BigNumber add(const BigNumber& a, const BigNumber& b);
    // �������� �� ������� ����
    bool hasLeadingZeros() const;
    // ���������� ��� ������������ ������
    ~BigNumber();
};

class CircularNumberRing {
private:
    DigitNode* head;  // ������ ������
    DigitNode* tail;  // ����� ������ (��������� � head)
    int length;       // ���������� ����

public:
    CircularNumberRing() : head(nullptr), tail(nullptr), length(0) {}

    // ���������� ����� � ������
    void append(int digit);

    // �������� ������ �� ������
    void fromString(const string& s);
    // ��������� ��������������������� ���� �� ������
    BigNumber getSubNumber(DigitNode* start, int len) const;
    // ����� ������� � ������
    string solve() const;
    // ���������� ��� ������������ ������
    ~CircularNumberRing();

};
// ������� ������ �����
void clearInput();
// ����� �������� ����
void showMenu();
// ��������� ���������� ��������
bool handleInvalidInput(const string& input, const string& clean);
// ��������� ��������� �����
void processFile();
// ��������� ������� �����
void processManual();
