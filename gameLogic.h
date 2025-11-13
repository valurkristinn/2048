#ifndef GAMELOGIC_H
#define GAMELOGIC_H

extern int *board;
extern int *hasMerged;
extern int score;

void initGame();
void startNewGame();
int performRound(int dir);

#endif
