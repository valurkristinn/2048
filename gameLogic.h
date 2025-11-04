#ifndef GAMELOGIC_H
#define GAMELOGIC_H

extern int *board;
extern int *hasMerged;

void initGame();
void startNewGame();
int performRound(int dir);

#endif
