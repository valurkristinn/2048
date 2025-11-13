#include "gameLogic.h"
#include "ui.h"
#include <stdio.h>

void display() { refreshTiles(board, hasMerged); }

int main() {
    initGame();
    initInterface();
    startNewGame();
    testing();
    display();
    do {
        display();
    } while (performRound(getInput()));
    display();
    draw_game_over_overlay();
    testing();
    closeInterface();
}
