#ifndef UI_H
#define UI_H

void initInterface();
void initGameOverlay();
int getInput();
void closeInterface();
void clearw();
void refreshTiles(int board[], int hasMerged[], int score, int inp);
void gameOver();


#endif
