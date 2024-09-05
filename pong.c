#include <stdio.h>

#define W 80                //width
#define H 25                //hight
#define LR_X 2               //x coordinate of left racket
#define RR_X 77              //x coordinate of right racket
#define SC_Y 3
#define SC_X 20

void field(int LR_Y, int RR_Y, int ball_x, int ball_y, int p1, int p2);
void ball(int *ball_x, int *ball_y, int *VX, int *VY);
void rackets(int *LR_Y, int *RR_Y);
void collision(int *LR_Y, int *RR_Y, int *ball_x, int *ball_y, int *VX, int *VY, int *p1, int *p2);

int main() {
    int p1 = 0, p2 = 0;                                                     //player's score
    int LR_Y = H / 2, RR_Y = H / 2, ball_x = W / 2, ball_y = H / 2;         // initial rackets' and ball's coordinates
    int VX = 1, VY = 1;                                                     //vectors for ball's movement

    while (p1 != 21 || p2 != 21) {
        field(LR_Y, RR_Y, ball_x, ball_y, p1, p2);            //painting the field
        rackets(&LR_Y, &RR_Y);
        collision(&LR_Y, &RR_Y, &ball_x, &ball_y, &VX, &VY, &p1, &p2);
        ball(&ball_x, &ball_y, &VX, &VY);                                           //ball movement
    }

    return 0;
}

void field(int LR_Y, int RR_Y, int ball_x, int ball_y, int p1, int p2) {
    printf("\033[1m\033[0d\033[2J");
    for (int i = 0; i < W + 1; i++) printf("_"); //top border
    printf("\n");

    for (int i = 0; i < H; i++) {
        printf("|");
        for (int j = 0; j < W; j++) {
            if (i == ball_y && j == ball_x)
                printf("o");
            else if (i == SC_Y && j == W / 2 - SC_X)
                printf("%d", p1%10);
            else if (i == SC_Y && j == W / 2 - SC_X-1 && p1 > 9)
                printf("%d", p1/10);
            else if (i == SC_Y && j == W / 2 + SC_X)
                printf("%d", p2%10);
            else if (i == SC_Y && j == W / 2 + SC_X-1 && p2 > 9)
                printf("%d", p2/10);
            else if ((i == LR_Y && j == LR_X) || (i == RR_Y && j == RR_X))
                printf("|");
            else if ((i == LR_Y - 1 && j == LR_X) || (i == RR_Y - 1 && j == RR_X))
                printf("|");
            else if ((i == LR_Y + 1 && j == LR_X) || (i == RR_Y + 1 && j == RR_X))
                printf("|");
            else
                printf(" ");

        }
        printf("|\n");
    }
    for (int i = 0; i < W + 1; i++) printf("‾");
    printf("\n");
}

void ball(int *ball_x, int *ball_y, int *VX, int *VY) {
    *ball_x += *VX;
    *ball_y += *VY;
}

void rackets(int *LR_Y, int *RR_Y) {
    char c;
    scanf(" %c", &c);
    switch (c) {
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
    if (*ball_y == 1 || *ball_y == H - 1)   //top&bottom
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