#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    int choice;
    do {
        showMenu();
        if (!(cin >> choice)) {
            clearInput();
            choice = 0;
        }
        clearInput();

        switch (choice) {
        case 1: processFile(); break;
        case 2: processManual(); break;
        case 3: cout << "Завершение.\n"; break;
        default: cout << "Неверный выбор!\n";
        }
    } while (choice != 3);

    return 0;
}