#include "gameLogic.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    int *board = calloc(16, sizeof(int));
    int *hasMerged = calloc(16, sizeof(int));
    int score = 0;

    if (!board || !hasMerged) {
        exit(1);
    }

    initInterface();
    int inp = 0;
    while (inp != 5) {
        startNewGame(board, hasMerged, &score);

        clearw();
        initGameOverlay();

        do {
            refreshTiles(board, hasMerged, score, inp);
            inp = getInput();
        } while (inp < 4 && performRound(inp, board, hasMerged, &score));

        if (inp < 4) {
            refreshTiles(board, hasMerged, score, inp);
            gameOver();
            while (inp < 4) {
                inp = getInput();
            }
        }
    }
    free(board);
    free(hasMerged);
    closeInterface();
}
