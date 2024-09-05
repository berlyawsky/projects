#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

#define W 80
#define H 25
#define MAP_COUNT 6 //макрос для кол-ва карт + выход

void show_menu(int *selected_map);                  //вывод меню
void load_map(int field[H][W], int selected_map);   //загрузка карты
void print_field(int field[H][W]);                  //вывод поля
void update_field(int field[H][W]);                 //обновление поля
int neighbors_count(int field[H][W], int x, int y); //подсчет соседей
void read_field(int field[H][W], const char *filename); //читаем файл с картой
void read_speed(char *c, int *speed);                   //меняем скорость

int main () {
    int field[H][W];                                    
    int speed = 5000;                                   //начальная скорость
    int selected_map = 0;                               //номер выбранной карты

    initscr();                  // Инициализация ncurses
    //cbreak();                   // Включение режима "cbreak" для немедленного ввода Я ТАК И НЕ ПОНЯЛА НАХУЯ ОН НУЖЕН, БЕЗ НЕГО ВСЁ РАБОТАЕТ
    noecho();                   // Отключение отображения вводимых символов
    keypad(stdscr, TRUE);       // Включение обработки функциональных клавиш (вверх-вниз для меню)
    curs_set(0);                // Невидимый курсор

    while (1) {
        char c1 = 'c';
        show_menu(&selected_map);   // меню и выбор карты

        if (selected_map == MAP_COUNT - 1) { //если последняя опция exit
            break; // Выход из основного цикла
        }

        load_map(field, selected_map); // Загрузить выбранную карту

        nodelay(stdscr, TRUE); 

        while(c1 != 'q') {
            print_field(field);
            update_field(field);
            read_speed(&c1, &speed);
            usleep(speed);
        }
    }
    endwin();                   //ncurses off
    return 0;
}

void show_menu(int *selected_map) {
    int ch;
    int highlight = 0; // Индекс выделенной карты
    const char *maps[MAP_COUNT] = {"Map 1", "Map 2", "Map 3", "Map 4", "Map 5", "Exit"}; //объявляем массив указателей на строки (массив строк)

    nodelay(stdscr, FALSE);  //чтобы меню не моргало

    while (1) {
        clear(); // Очистка экрана (без этого при обратном выходе в меню на экране будут остатки прошлой карты)
        for (int i = 0; i < MAP_COUNT; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // (Функция attron используется для включения определённых атрибутов отображения текста в терминале, A_REVERSE меняет цветами фон и текст, т.е. подсвечивает)
            }
            mvprintw(i, 0, "%s", maps[i]); // Вывод названия карты
            if (i == highlight) {
                attroff(A_REVERSE); // Снятие подсветки
            }
        }
        refresh(); // Обновление экрана, но без неё тоже работает ок

        ch = getch(); // Получение ввода от пользователя
        switch (ch) {
            case KEY_UP: // Если нажата клавиша вверх
                highlight = (highlight == 0) ? MAP_COUNT - 1 : highlight - 1; // Перемещение вверх
                break;
            case KEY_DOWN: // Если нажата клавиша вниз
                highlight = (highlight == MAP_COUNT - 1) ? 0 : highlight + 1; // Перемещение вниз
                break;
            case 10: // Если нажата клавиша Enter
                *selected_map = highlight; // Сохранение выбранной карты
                return; // Выход из функции
        }

        //usleep(10000); // Небольшая задержка, но без неё тоже работает ок
    }
}

void load_map(int field[H][W], int selected_map) {
    char filename[20];  //filename используется для хранения имени файла, который будет загружен
    snprintf(filename, sizeof(filename), "map%d.txt", selected_map + 1); //выполняет форматированное записывание строки в массив filename
    read_field(field, filename);
}

void read_field(int field[H][W], const char *filename) {
    FILE *file = fopen(filename, "r");      //объявляем указатель на структуру FILE, открываем файл с картой для чтения
    if (file == NULL) {                     //Если файл не может быть открыт
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fscanf(file, "%d", &field[i][j]);
        }
    }
    fclose(file);
}

void print_field(int field[H][W]) {
    clear();
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (field[i][j]) printw("o");
            else printw(" ");
        }
        printw("\n");
    }
}

void update_field(int field[H][W]) {
    int neighbors = 0;
    int new_field[H][W];
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            neighbors = neighbors_count(field, i, j);
            if (field[i][j]) {
                new_field[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0; 
            }
            else {
                new_field[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            field[i][j] = new_field[i][j];
        }
    }
}

int neighbors_count(int field[H][W], int x, int y) {
    int count = 0;
    for(int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            int a = (H + i) % H;
            int b = (W + j) % W;
            count += field[a][b];
        }
    }
    return count -= field[x][y];
}

void read_speed(char *c, int *speed) {
    *c = getch();

    switch (*c) {
        case '1': *speed = 5000; break;
        case '2': *speed = 25000; break;
        case '3': *speed = 50000; break;
        case '4': *speed = 250000; break;
        case '5': *speed = 500000; break;
        default: break;
    }

}