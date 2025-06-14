#include "Header.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    int choice;
    do {
        showMenu();// Отображает главное меню программы
        if (!(cin >> choice)) {
            clearInput(); //Очищает буфер ввода
            choice = 0;
        }
        clearInput();

        switch (choice) {
        case 1: processFile(); break; // Обрабатывает ввод из файла
        case 2: processManual(); break; // Обрабатывает ручной ввод пользователя
        case 3: cout << "Завершение.\n"; break;
        default: cout << "Неверный выбор!\n";
        }
    } while (choice != 3);

    return 0;
}
