#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int board[16];
int emptyTiles[16];

enum Direction { UP = -4, DOWN = 4, LEFT = -1, RIGHT = 1 };

void *getEmptyTiles(int *count) {
    *count = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) {
            emptyTiles[(*count)++] = i;
        }
    }
    return emptyTiles;
}

void newTile(int count) {
    int tileVal = (rand() % 2 + 1) * 2;
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

int moveTile(int tileNum, enum Direction direction, int hasMerged[]) {
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

    moveTile(nextTileNum, direction, hasMerged);
    return 1;
}

int moveAll(enum Direction direction) {
    int hasMerged[16];
    memset(hasMerged, 0, sizeof(hasMerged));
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
            if (moveTile(i * 4 + j, direction, hasMerged)) {
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

enum Direction getInput() {
    char c = getchar();
    while (1) {
        switch (c) {
        case 'w':
            return UP;
        case 's':
            return DOWN;
        case 'a':
            return LEFT;
        case 'd':
            return RIGHT;
        default:
            c = getchar();
        }
    }
}

void newGame() {
    memset(board, 0, sizeof(board));
    int count;
    int loop = 1;


    getEmptyTiles(&count);
    newTile(count);
    getEmptyTiles(&count);
    newTile(count);

    while (!(count == 1 && isGameOver())) {
        display();

        while (!moveAll(getInput())) {
            printf("invalid move, try again");
        }
        getEmptyTiles(&count);
        newTile(count);
    }
    display();
}

int main() {
    // srand(time(NULL));
    newGame();
}
