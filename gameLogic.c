#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int board[16];

enum Direction { UP = -4, DOWN = 4, LEFT = -1, RIGHT = 1 };

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
    board[tileNum] = tileVal;
}

int getMoveValidity(int tilenum, enum Direction direction) {
    int i = tilenum / 4;
    int j = tilenum % 4;

    int nexti = (tilenum + direction) / 4;
    int nextj = (tilenum + direction) % 4;

    switch (direction) {
    case UP:
        if (nexti != i - 1 || i - 1 > 3)
            return 0;
        break;
    case DOWN:
        if (nexti != i + 1 || i + 1 < 0)
            return 0;
        break;
    case LEFT:
        if (nextj != j - 1 || i - 1 > 3)
            return 0;
        break;
    case RIGHT:
        if (nextj != j + 1 || i + 1 < 0)
            return 0;
        break;
    }

    int currentTileVal = board[tilenum];
    int nextTileVal = board[tilenum + direction];

    if (nextTileVal == 0) {
        return 1;
    }
    if (currentTileVal == nextTileVal) {
        return 2;
    }
    return 0;
}

void move(int tileNum, enum Direction direction) {
    int nextTileNum = tileNum + direction;
    if (board[tileNum] == 0) {
        return;
    }

    int validity = getMoveValidity(tileNum, direction);

    if (!validity) {
        return;
    }


    board[nextTileNum] = board[tileNum] * validity;
    board[tileNum] = 0;

    move(nextTileNum, direction);
}

void display() {
    // for (int i = 0; i < 99; i++) {
    //     printf("\n");
    // }
    for (int i = 0; i < 16; i++) {
        printf("%d ", board[i]);
        if (i % 4 == 3) {
            printf("\n");
        }
    }
    printf("\n");
}

void newGame() {
    memset(board, 0, sizeof(board));
    newTile();
    newTile();
    printf("%d\n", board[9]);

    display();
    move(9, UP);
    display();
}

int main() {
    // srand(time(NULL));
    newGame();
}
