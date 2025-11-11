#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gameLogic.h"

enum Direction { UP = -4, DOWN = 4, LEFT = -1, RIGHT = 1 };

int *board;
int emptyTiles[16];
int *hasMerged;
int *count;

void getEmptyTiles() {
    *count = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) {
            emptyTiles[(*count)++] = i;
        }
    }
}

void newTile() {
    int tileVal = (rand() % 2 + 1) * 2;
    int tileNum = emptyTiles[rand() % *count];
    board[tileNum] = tileVal;
}

int getMoveValidity(int tilenum, enum Direction direction) {
    int i = tilenum / 4;
    int j = tilenum % 4;

    int nexti = (tilenum + direction) / 4;
    int nextj = (tilenum + direction) % 4;

    switch (direction) {
    case UP:
        if (nexti != i - 1 || i - 1 < 0)
            return 0;
        break;
    case DOWN:
        if (nexti != i + 1 || i + 1 > 3)
            return 0;
        break;
    case LEFT:
        if (nextj != j - 1 || j - 1 < 0)
            return 0;
        break;
    case RIGHT:
        if (nextj != j + 1 || j + 1 > 3)
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

int moveTile(int tileNum, enum Direction direction) {
    if (board[tileNum] == 0) {
        return 0;
    }

    int validity = getMoveValidity(tileNum, direction);
    int nextTileNum = tileNum + direction;

    if (!validity || hasMerged[nextTileNum] || hasMerged[tileNum]) {
        return 0;
    }

    board[nextTileNum] = board[tileNum] * validity;
    board[tileNum] = 0;

    hasMerged[nextTileNum] = hasMerged[tileNum];
    hasMerged[tileNum] = 0;

    if (validity == 2) {
        hasMerged[nextTileNum] = 1;
    }

    moveTile(nextTileNum, direction);
    return 1;
}

int moveAll(enum Direction direction) {
    memset(hasMerged, 0, 16 * sizeof(int));
    int hasMoved = 0;

    // up&left 0 0 + +
    // down&right 3 3 - -

    int start = 0;
    int step = 1;
    if (direction == DOWN || direction == RIGHT) {
        start = 3;
        step = -1;
    }

    for (int i = start; step > 0 ? i <= 3 : i >= 0; i += step) {
        for (int j = start; step > 0 ? j <= 3 : j >= 0; j += step) {
            if (moveTile(i * 4 + j, direction)) {
                hasMoved = 1;
            }
        }
    }
    return hasMoved;
}

int isGameOver() {
    enum Direction directions[4] = {UP, DOWN, LEFT, RIGHT};

    for (int i = 0; i < 4; i++) {
        for (int j = i % 2 == 0 ? 0 : 1; j < 4; j += 2) {
            for (int k = 0; k < 4; k++) {
                if (getMoveValidity(i * 4 + j, directions[k])) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

enum Direction intToDir(int i) {
    switch (i) {
    case -4:
        return UP;
    case 4:
        return DOWN;
    case -1:
        return LEFT;
    case 1:
        return RIGHT;
    default:
        return 0;
    }
}

void initGame() {
    board = calloc(16, sizeof(int));
    hasMerged = calloc(16, sizeof(int));
    count = malloc(sizeof(int));
}

void startNewGame() {
    memset(board, 0, sizeof(*board));

    getEmptyTiles();
    newTile();
    getEmptyTiles();
    newTile();
}

int performRound(int dir) {
    if (*count == 1 && isGameOver()) {
        return 0;
    }
    if (!moveAll(intToDir(dir))) {
        return 2;
    }

    getEmptyTiles();
    newTile();
    return 1;
}
