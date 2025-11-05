#include "gameLogic.h"
#include "ui.h"
#include <stdio.h>

void display() {
    drawNumbers(board);
    testing();
    animateTiles(hasMerged);
}

int main() {
    initGame();
    initInterface();
    drawTiles();
    startNewGame();
    do {
        display();
    } while (performRound(getInput()));
    display();
    closeInterface();
}
