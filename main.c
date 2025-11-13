#include "gameLogic.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

void display(int inp) { refreshTiles(board, hasMerged, score, inp); }

int main() {
    initGame();
    initInterface();
    int inp = 0;
    while (inp != 5) {
        startNewGame();

        clearw();
        initGameOverlay();

        do {
            display(inp);
            inp = getInput();
        } while (inp < 4 && performRound(inp));

        if (inp < 4) {
            display(inp);
            gameOver();
            while (inp < 4) {
                inp = getInput();
            }
        }
    }
    closeInterface();
}
