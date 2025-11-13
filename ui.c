#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "gameLogic.h"
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

void getMargin() {
    xMargin = (xMax - mainWidth) / 2;
    yMargin = (yMax - mainHeight) / 2;
}

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

    init_pair(1, COLOR_WHITE,
              COLOR_BLACK);          // Empty (2^0 = 1, but represents empty)
    init_pair(2, 250, COLOR_BLACK);  // 2 - light gray
    init_pair(3, 230, COLOR_BLACK);  // 4 - beige
    init_pair(4, 214, COLOR_BLACK);  // 8 - orange
    init_pair(5, 208, COLOR_BLACK);  // 16 - dark orange
    init_pair(6, 196, COLOR_BLACK);  // 32 - red
    init_pair(7, 160, COLOR_BLACK);  // 64 - darker red
    init_pair(8, 226, COLOR_BLACK);  // 128 - yellow
    init_pair(9, 220, COLOR_BLACK);  // 256 - gold
    init_pair(10, 190, COLOR_BLACK); // 512 - yellow-orange
    init_pair(11, 184, COLOR_BLACK); // 1024 - lighter yellow
    init_pair(12, 178, COLOR_BLACK); // 2048 - golden yellow
    init_pair(13, 172, COLOR_BLACK); // 4096 - orange-yellow
    init_pair(14, 166, COLOR_BLACK); // 8192 - darker orange-yellow
    init_pair(15, 160, COLOR_BLACK); // 16384 - red-orange
    init_pair(16, 154, COLOR_BLACK); // 32768 - darker red-orange
    init_pair(17, 148, COLOR_BLACK); // 65536 - even darke
    init_pair(18, COLOR_BLACK, COLOR_WHITE);
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

void initGameOverlay() {
    int x = xMargin;
    int y = yMargin + mainHeight;
    cchar_t arrow_up, arrow_down, arrow_left, arrow_right;

    setcchar(&arrow_up, L"\u2191", 0, 0, NULL);
    setcchar(&arrow_down, L"\u2193", 0, 0, NULL);
    setcchar(&arrow_left, L"\u2190", 0, 0, NULL);
    setcchar(&arrow_right, L"\u2192", 0, 0, NULL);

    drawBox(x, y, mainWidth, 7);

    mvprintw(y + 2, x + 3, "quit:    q");
    mvprintw(y + 4, x + 3, "restart: r");
    mvprintw(y + 2, x + 15, "score: ");

    mvadd_wch(y + 2, x + mainWidth - 7, &arrow_up);
    mvadd_wch(y + 3, x + mainWidth - 9, &arrow_left);
    mvadd_wch(y + 3, x + mainWidth - 5, &arrow_right);
    mvadd_wch(y + 4, x + mainWidth - 7, &arrow_down);
}

void clearw(){
    clear();
}

void closeInterface() {
    clear();
    endwin();
}

int get_color_pair(int value) {
    if (value == 0)
        return 1;
    return (value <= 65536) ? (__builtin_ctz(value) + 1) : 17;
}

void drawBoxThick(int x, int y, int width, int height) {
    wchar_t topLine[4 * (width - 1)];
    wmemset(topLine, L'\u2580', width - 2);
    topLine[width - 2] = L'\0';
    mvaddwstr(y, x + 1, topLine);

    wchar_t bottomLine[4 * (width - 1)];
    wmemset(bottomLine, L'\u2584', width - 2);
    bottomLine[width - 2] = L'\0';
    mvaddwstr(y + height - 1, x + 1, bottomLine);

    wchar_t block[] = L"\u2588";

    for (int i = 0; i < height; i++) {
        mvaddwstr(y + i, x, block);
        mvaddwstr(y + i, x + width - 1, block);
    }
}

int getXofTile(int tileNum) {
    return xMargin + 2 + (tileWidth + 1) * (tileNum % 4);
}

int getYofTile(int tileNum) {
    return yMargin + 1 + (tileHeight) * (tileNum / 4);
}

void drawTiles(int board[]) {
    drawBox(xMargin, yMargin, mainWidth, mainHeight);
    for (int i = 0; i < 16; i++) {
        int colorPair = COLOR_PAIR(get_color_pair(board[i]));
        wattron(stdscr, colorPair);
        drawBox(getXofTile(i), getYofTile(i), tileWidth, tileHeight);
    }
    wattroff(stdscr, COLOR_PAIR(get_color_pair(15)));
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
        // for (int j = 1; j < tileWidth - 2; j++) {
        //     mvaddch(, , ' ');
        // }
        mvhline(getYofTile(i) + (tileHeight / 2), getXofTile(i) + 1, ' ',
                tileWidth - 4);

        if (val != 0) {
            int decimals = countDecimals(val);
            char str[decimals + 1];
            sprintf(str, "%d", val);
            int x = getXofTile(i) + (tileWidth - decimals) / 2;
            int y = getYofTile(i) + (tileHeight / 2);
            mvaddstr(y, x, str);
        }
    }
}

void eraseBetweenTiles() {
    for (int i = 1; i < mainHeight; i++) {
        for (int j = 0; j < 6; j++) {
            mvaddch(yMargin + i, xMargin + 1 + j * (tileWidth + 1), ' ');
        }
    }
}

void eraseBorder(int x, int y, int width, int height) {
    mvhline(y, x, ' ', width);
    mvhline(y + height - 1, x, ' ', width);
    mvvline(y + 1, x, ' ', height - 2);
    mvvline(y + 1, x + width - 1, ' ', height - 2);
}

void animateTiles(int hasMerged[]) {
    for (int i = 0; i < 16; i++) {
        if (hasMerged[i]) {
            drawBoxThick(getXofTile(i), getYofTile(i), tileWidth, tileHeight);
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
            return 0;
        case 's':
        case 'j':
        case 258:
            return 1;
        case 'a':
        case 'h':
        case 260:
            return 2;
        case 'd':
        case 'l':
        case 261:
            return 3;
        case 'q':
            return 5;
        case 'r':
            return 4;
        }
    }
}

void interruptedPause(int ms) {
    timeout(ms);
    int ch = getch();
    timeout(-1);

    if (ch != ERR) {
        ungetch(ch);
    }
}

void highlightMenuItem(int item, int color) {
    int x = xMargin;
    int y = yMargin + mainHeight;
    switch (item) {
    case 0:
        x += mainWidth - 7;
        y += 2;
        break;
    case 1:
        x += mainWidth - 7;
        y += 4;
        break;
    case 2:
        x += mainWidth - 9;
        y += 3;
        break;
    case 3:
        x += mainWidth - 5;
        y += 3;
        break;
    case 4:
        x += 12;
        y += 4;
        break;
    }
    mvchgat(y, x, 1, A_NORMAL, color, NULL);
}

void refreshOverlay(int score, int inp) {
    mvprintw(yMargin + mainHeight + 2, xMargin + 22, "%d", score);
    highlightMenuItem(inp, 18);
}

void refreshTiles(int board[], int hasMerged[], int score, int inp) {
    drawTiles(board);
    drawNumbers(board);

    animateTiles(hasMerged);
    refreshOverlay(score, inp);

    refresh();
    interruptedPause(170);
    drawTiles(board);
    highlightMenuItem(inp,1);
    refresh();
}

void drawGameOverOverlay() {
    const char *ascii_art[] = {" ██████╗  █████╗ ███╗   ███╗███████╗      "
                               "██████╗ ██╗   ██╗███████╗██████╗ ",
                               "██╔════╝ ██╔══██╗████╗ ████║██╔════╝     "
                               "██╔═══██╗██║   ██║██╔════╝██╔══██╗",
                               "██║  ███╗███████║██╔████╔██║█████╗       ██║   "
                               "██║██║   ██║█████╗  ██████╔╝",
                               "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝       ██║   "
                               "██║╚██╗ ██╔╝██╔══╝  ██╔══██╗",
                               "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗     "
                               "╚██████╔╝ ╚████╔╝ ███████╗██║  ██║",
                               " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝      "
                               "╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝"};

    int num_lines = 6;
    int max_width = 75;

    int start_y = (yMax - num_lines) / 2;
    int start_x = (xMax - max_width) / 2;

    wchar_t w_line_buf[max_width + 1];

    for (int i = 0; i < num_lines; i++) {
        int y = start_y + i;
        if (y < 0 || y >= yMax)
            continue;

        const char *line = ascii_art[i];

        mbstowcs(w_line_buf, line, max_width + 1);

        size_t wide_len = wcslen(w_line_buf);

        for (int j = 0; j < wide_len; j++) {
            int x = start_x + j;
            if (x < 0 || x >= xMax)
                continue;

            if (w_line_buf[j] != L' ') {
                cchar_t char_to_add;
                setcchar(&char_to_add, &w_line_buf[j], 0, 0, NULL);

                mvadd_wch(y, x, &char_to_add);
            }
        }
    }

    refresh();
}

void gameOver() { drawGameOverOverlay(); }
