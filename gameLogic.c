#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gameLogic.h"

enum Direction { UP = -4, DOWN = 4, LEFT = -1, RIGHT = 1 };

int getEmptyTiles(int *board, int *emptyTiles) {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) {
            emptyTiles[count++] = i;
        }
    }
    return count;
}

int countEmptyTiles(int *board) {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) {
            count++;
        }
    }
    return count;
}

void newTile(int *board) {
    int *emptyTiles = calloc(16, sizeof(int));
    if (!emptyTiles)
        exit(1);
    int count = getEmptyTiles(board, emptyTiles);
    int tileVal = (rand() % 10) < 9 ? 2 : 4;
    int tileNum = emptyTiles[rand() % count];
    board[tileNum] = tileVal;
    free(emptyTiles);
}

int getMoveValidity(int tilenum, enum Direction direction, int *board) {
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

int moveTile(int tileNum, enum Direction direction, int *board, int *hasMerged,
             int *score) {
    if (board[tileNum] == 0) {
        return 0;
    }

    int validity = getMoveValidity(tileNum, direction, board);
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
        *score += board[nextTileNum];
    }

    moveTile(nextTileNum, direction, board, hasMerged, score);
    return 1;
}

int moveAll(enum Direction direction, int *board, int *hasMerged, int *score) {
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
            if (moveTile(i * 4 + j, direction, board, hasMerged, score)) {
                hasMoved = 1;
            }
        }
    }
    return hasMoved;
}

int isGameOver(int *board) {
    enum Direction directions[4] = {UP, DOWN, LEFT, RIGHT};

    for (int i = 0; i < 4; i++) {
        for (int j = i % 2 == 0 ? 0 : 1; j < 4; j += 2) {
            for (int k = 0; k < 4; k++) {
                if (getMoveValidity(i * 4 + j, directions[k], board)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

enum Direction intToDir(int i) {
    switch (i) {
    case 0:
        return UP;
    case 1:
        return DOWN;
    case 2:
        return LEFT;
    case 3:
        return RIGHT;
    default:
        return 0;
    }
}

void startNewGame(int *board, int *hasMerged, int *score) {
    memset(board, 0, 16 * sizeof(int));
    memset(hasMerged, 0, 16 * sizeof(int));
    *score = 0;

    newTile(board);
    newTile(board);
}

int performRound(int inp, int *board, int *hasMerged, int *score) {
    if (countEmptyTiles(board) == 1 && isGameOver(board)) {
        return 0;
    }

    if (!moveAll(intToDir(inp), board, hasMerged, score)) {
        return 2;
    }

    newTile(board);

    return 1;
}
