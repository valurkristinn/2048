#define _XOPEN_SOURCE_EXTENDED
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "ui.h"

enum Direction { UP = -4, DOWN = 4, LEFT = -1, RIGHT = 1 };

int xMax;
int yMax;
int mainWidth;
int mainHeight;
int xMargin;
int yMargin;
int tileWidth;
int tileHeight;

void initInterface() {
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    xMax = getmaxx(stdscr);
    yMax = getmaxy(stdscr);
    tileWidth = 9;
    tileHeight = 5;
    mainWidth = tileWidth * 4 + 7;
    mainHeight = tileHeight * 4 + 2;
    getMargin();

    start_color();

    init_pair(1, 59, 250);
    init_pair(2, 59, 255);
    init_pair(3, 59, 253);
    init_pair(4, 231, 208);
    init_pair(5, 231, 202);
    init_pair(6, 231, 196);
    init_pair(7, 231, 160);
    init_pair(8, 231, 220);
    init_pair(9, 231, 214);
    init_pair(10, 231, 208);
    init_pair(11, 231, 202);
    init_pair(12, 231, 196);
    init_pair(13, 16, 226);
    init_pair(14, 16, 220);
    init_pair(15, 16, 214);
    init_pair(16, 231, 93);
    init_pair(17, 231, 27);

    init_pair(18, COLOR_BLACK, COLOR_WHITE);
}

void getMargin() {
    xMargin = (xMax - mainWidth) / 2;
    yMargin = (yMax - mainHeight) / 2;
}

void closeInterface() { endwin(); }

int get_color_pair(int value) {
    if (value == 0)
        return 1;
    return (value <= 65536) ? (__builtin_ctz(value) + 1) : 17;
}

void drawBox(int x, int y, int width, int height) {
    mvhline(y, x + 1, ACS_HLINE, width - 2);
    mvhline(y + height - 1, x + 1, ACS_HLINE, width - 2);
    mvvline(y + 1, x, ACS_VLINE, height - 2);
    mvvline(y + 1, x + width - 1, ACS_VLINE, height - 2);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + width - 1, ACS_URCORNER);
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);
}

// void drawBoxThick(int x, int y, int width, int height) {
//     wchar_t upperLine[tileWidth*4];
//     wmemset(upperLine, L'\u2580', width - 2);
//     upperLine[width - 2] = L'\0';
//     mvaddwstr(y, x + 1, upperLine);
//
//     wchar_t lowerLine[256];
//     wmemset(lowerLine, L'\u2584', width - 2);
//     lowerLine[width - 2] = L'\0';
//     mvaddwstr(y + height - 1, x + 1, lowerLine);
//
//     wchar_t column[256];
//     wmemset(column,' ',)
//
//     for (int i = 0; i < height; i++) {
//         mvaddwstr(y + i, x, "█");
//     }
// }

// nota frekar bara background color
void drawBoxThick(int x, int y, int width, int height) {
    wattron(stdscr, COLOR_PAIR(18));
    mvhline(y, x, ' ', width);
    mvhline(y + height - 1, x, ' ', width);
    mvvline(y + 1, x, ' ', height - 2);
    mvvline(y + 1, x + width - 1, ' ', height - 2);

    // mvvline(y + 1, x + 1, ' ', height - 2);
    // mvvline(y + 1, x + width - 2, ' ', height - 2);
    wattroff(stdscr, COLOR_PAIR(18));
}

int getXofTile(int tileNum) {
    return xMargin + 2 + (tileWidth + 1) * (tileNum % 4);
}

int getYofTile(int tileNum) {
    return yMargin + 1 + (tileHeight) * (tileNum / 4);
}

void drawTiles() {
    drawBox(xMargin, yMargin, mainWidth, mainHeight);
    for (int i = 0; i < 16; i++) {
        drawBox(getXofTile(i), getYofTile(i), tileWidth, tileHeight);
    }
    refresh();
}

int countDecimals(int n) {
    if (n < 10)
        return 1;
    if (n < 100)
        return 2;
    if (n < 1000)
        return 3;
    if (n < 10000)
        return 4;
    if (n < 100000)
        return 5;
    if (n < 1000000)
        return 6;
    if (n < 10000000)
        return 7;
    if (n < 100000000)
        return 8;
    if (n < 1000000000)
        return 9;
    return 0;
}

void drawNumbers(int board[]) {
    for (int i = 0; i < 16; i++) {
        int val = board[i];
        for (int j = 1; j < tileWidth - 2; j++) {
            mvaddch(getYofTile(i) + (tileHeight / 2), getXofTile(i) + j, ' ');
        }

        if (val != 0) {
            int decimals = countDecimals(val);
            char str[decimals + 1];
            sprintf(str, "%d", val);
            int x = getXofTile(i) + (tileWidth - decimals) / 2;
            int y = getYofTile(i) + (tileHeight / 2);
            mvaddstr(y, x, str);
        }
    }
    refresh();
}

void eraseBetweenTiles() {
    for (int i = 1; i < mainHeight; i++) {
        for (int j = 0; j < 6; j++) {
            mvaddch(yMargin + i, xMargin + 1 + j * (tileWidth + 1), ' ');
        }
    }
}

void eraseBorder(int x, int y, int width, int height) {
    for (int i = 0; i < width; i++) {
        mvaddch(y, x + i, ' ');
        mvaddch(y + height - 1, x + i, ' ');
    }
    for (int j = 1; j < height - 1; j++) {
        mvaddch(y + j, x, ' ');
        mvaddch(y + j, x + width - 1, ' ');
    }
}



void animateTiles(int hasMerged[]) {
    for (int i = 0; i < 16; i++) {
        if (hasMerged[i]) {
            drawBoxThick(getXofTile(i), getYofTile(i), tileWidth, tileHeight);
        }
    }
    refresh();
    usleep(1500 * 100);

    for (int i = 0; i < 16; i++) {
        if (hasMerged[i]) {
            drawBox(getXofTile(i), getYofTile(i), tileWidth, tileHeight);
         }
    }
    
}

int getInput() {
    while (1) {
        int inp = getch();
        switch (inp) {
        case 'w':
        case 'k':
        case 259:
            return -4;
            break;
        case 's':
        case 'j':
        case 258:
            return 4;
            break;
        case 'a':
        case 'h':
        case 260:
            return -1;
            break;
        case 'd':
        case 'l':
        case 261:
            return 1;
            break;
        }
    }
}

void testing() {
}
