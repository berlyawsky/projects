#include <stdio.h>
#include <ncurses.h>
//#include <unistd.h>
#include <time.h>

#define W 80                 //width
#define H 25                 //hight
#define LR_X 2               //x coordinate of left racket
#define RR_X 77              //x coordinate of right racket
#define SC_Y 3
#define SC_X 20

void field(int LR_Y, int RR_Y, int ball_x, int ball_y, int p1, int p2, WINDOW *win);
void ball(int *ball_x, int *ball_y, int *VX, int *VY);
void rackets(int *LR_Y, int *RR_Y, int *flag);
void collision(int *LR_Y, int *RR_Y, int *ball_x, int *ball_y, int *VX, int *VY, int *p1, int *p2);

int main() {
    int flag = 1;
    int p1 = 0, p2 = 0;                                                     //player's score
    int LR_Y = H / 2, RR_Y = H / 2, ball_x = W / 2, ball_y = H / 2;         // initial rackets' and ball's coordinates
    int VX = 1, VY = 1;                                                     //vectors for ball's movement
    
    struct timespec req = {0};
    req.tv_sec = 0; // секунды
    req.tv_nsec = 50000; // 0.05 миллисекунд

    initscr();
    cbreak();             // Включение режима cbreak
    noecho();             // Отключение отображения вводимых символов
    nodelay(stdscr, TRUE);
    curs_set(0);
    WINDOW *win = newwin(H + 2, W + 2, 0, 0);
    while ((p1 != 21 || p2 != 21) && flag) {
        field(LR_Y, RR_Y, ball_x, ball_y, p1, p2, win);            //painting the field
        rackets(&LR_Y, &RR_Y, &flag);
        collision(&LR_Y, &RR_Y, &ball_x, &ball_y, &VX, &VY, &p1, &p2);
        ball(&ball_x, &ball_y, &VX, &VY);                                           //ball movement
        //usleep(100);
        nanosleep(&req, NULL);
    }
    delwin(win);
    endwin();

    return 0;
}

void field(int LR_Y, int RR_Y, int ball_x, int ball_y, int p1, int p2, WINDOW *win) {
    // Создаем новое окно
    box(win, 0, 0); // Рисуем рамку вокруг окна

    // Заполняем окно пробелами
    for (int i = 1; i <= H; i++) {
        for (int j = 1; j <= W; j++) {
            if (i - 1 == ball_y && j - 1 == ball_x) {
                mvwaddch(win, i, j, 'o'); // Рисуем мяч
            } else if (i - 1 == SC_Y && j - 1 == W / 2 - SC_X) {
                mvwprintw(win, i, j, "%d", p1 % 10);
            } else if (i - 1 == SC_Y && j - 1 == W / 2 - SC_X - 1 && p1 > 9) {
                mvwprintw(win, i, j, "%d", p1 / 10);
            } else if (i - 1 == SC_Y && j - 1 == W / 2 + SC_X) {
                mvwprintw(win, i, j, "%d", p2 % 10);
            } else if (i - 1 == SC_Y && j - 1 == W / 2 + SC_X - 1 && p2 > 9) {
                mvwprintw(win, i, j, "%d", p2 / 10);
            } else if ((i - 1 == LR_Y && j - 1 == LR_X) || (i - 1 == RR_Y && j - 1 == RR_X)) {
                mvwaddch(win, i, j, '|'); // Рисуем ракетки
            } else if ((i - 1 == LR_Y - 1 && j - 1 == LR_X) || (i - 1 == RR_Y - 1 && j - 1 == RR_X)) {
                mvwaddch(win, i, j, '|'); // Рисуем ракетки
            } else if ((i - 1 == LR_Y + 1 && j - 1 == LR_X) || (i - 1 == RR_Y + 1 && j - 1 == RR_X)) {
                mvwaddch(win, i, j, '|'); // Рисуем ракетки
            } else {
                mvwaddch(win, i, j, ' '); // Заполняем пробелами
            }
        }
    }

    // Обновляем окно
    wrefresh(win);
    //delwin(win); // Удаляем окно после использования
}

void ball(int *ball_x, int *ball_y, int *VX, int *VY) {
    *ball_x += *VX;
    *ball_y += *VY;
}

void rackets(int *LR_Y, int *RR_Y, int *flag) {
    char c = getch();
    switch (c) {
        case 'q':
            *flag = 0; break;
        case 'a':
            if (*LR_Y - 1) (*LR_Y)--; break;
        case 'z':
            if (*LR_Y - 23) (*LR_Y)++; break;
        case 'k':
            if (*RR_Y - 1) (*RR_Y)--; break;
        case 'm':
            if (*RR_Y - 23) (*RR_Y)++; break;
    }
}

void collision(int *LR_Y, int *RR_Y, int *ball_x, int *ball_y, int *VX, int *VY, int *p1, int *p2) {
    if (*ball_y == 0 || *ball_y == H - 1)   //top&bottom
        (*VY) *= -1;

    if (*ball_x == 1 || *ball_x == W - 1) {                     //left&right goal
        (*VX) *= -1;
        if (*ball_x == 1) (*p2)++;
        else (*p1)++;
        (*ball_x) =  W / 2;
        (*ball_y) = H / 2;
    }

    if (*ball_x == LR_X + 1 && (*ball_y == *LR_Y || *ball_y == *LR_Y - 1 || *ball_y == *LR_Y + 1)) //left racket collision
        (*VX) *= -1;
    if (*ball_x == RR_X - 1 && (*ball_y == *RR_Y || *ball_y == *RR_Y - 1 || *ball_y == *RR_Y + 1)) //right racket collision
        (*VX) *= -1;
}