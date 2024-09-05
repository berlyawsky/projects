#include <stdio.h>
#include <unistd.h>

#define W 80
#define H 25

void print_field(int field[H][W]);
void update_field(int field[H][W]);
int neighbors_count(int field[H][W], int x, int y);
void read_field(int field[H][W]);

int main () {
    int field[H][W];
    char c = 'c';
    read_field(field);
    while(c != 'q') {
        print_field(field);
        update_field(field);
        usleep(50000);
    }
    return 0;
}

void read_field(int field[H][W]) {
    int n = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            scanf(" %d", &n);
            field[i][j] = n;
        }
    }
    freopen("/dev/tty", "r+", stdin);
}

void print_field(int field[H][W]) {
    printf("\033[1m\033[0d\033[2J");
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (field[i][j]) printf("o");
            else printf(" ");
        }
        printf("\n");
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