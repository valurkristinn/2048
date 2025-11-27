#ifndef GAMELOGIC_H
#define GAMELOGIC_H

void startNewGame(int *board, int *hasMerged,
                  int *score);
int performRound(int inp, int *board, int *hasMerged,
                 int *score);
#endif
