#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int board[4][4];

int* getEmptyTiles(int* count){
    int* emptyTiles = calloc(17, sizeof(int));
    *count = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(board[i][j] == 0){
                emptyTiles[(*count)++] = i * 4 + j;
            }
        } 
    }  
    return emptyTiles;
}

void newTile(){
    int tileVal = ( rand() % 2 + 1 ) * 2;
    int count; 
    int* emptyTiles = getEmptyTiles(&count);
    int tileNum = emptyTiles[rand() % count];
    int j = tileNum % 4;
    board[(tileNum - j)/4][j] = tileVal;
}

void display() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", board[i][j] ); 
        }
        printf("\n"); 
    }
}

void newGame(){
    memset(board, 0, sizeof(board));
    newTile();
    newTile();
    display();
}



int main(){
    // srand(time(NULL));
    newGame(); 
}
