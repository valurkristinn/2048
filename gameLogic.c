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
    int j = tileNum % 4;
    board[tileNum] = tileVal;
}

int isMoveValid(int tilenum, enum Direction direction) {
    int currentTileVal  = board[tilenum];
    int nextTileVal = board[tilenum+direction];
    return currentTileVal == nextTileVal || nextTileVal == 0;
}

void move(enum Direction direction) {

}

void display() {
    for (int i = 0; i < 99; i++) {
        printf("\n");
    }
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
