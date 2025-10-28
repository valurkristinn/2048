#include <stdio.h>
#include "gameLogic.h"

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

int main() {
    init();
    startNewGame();
    do {
        display();
    } while (performRound());
    display();
}
