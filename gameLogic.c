#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int board[16];

int *getEmptyTiles(int *count) {
    int *emptyTiles = calloc(16, sizeof(int));
    *count = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) {
            emptyTiles[(*count)++] = i;
        }
    }
    return emptyTiles;
}

void newTile() {
    int tileVal = (rand() % 2 + 1) * 2;
    int count;
    int *emptyTiles = getEmptyTiles(&count);
    int tileNum = emptyTiles[rand() % count];
    int j = tileNum % 4;
    board[tileNum] = tileVal;
}

void isMoveValid(int tilenum, int direction) {}

void move(int direction) {}

void display() {
    for (int i = 0; i < 16; i++) {
        printf("%d ", board[i]);
        if (i % 4 == 3) {
            printf("\n");
        }
    }
}

void newGame() {
    memset(board, 0, sizeof(board));
    newTile();
    newTile();
    display();
}

int main() {
    // srand(time(NULL));
    newGame();
}
