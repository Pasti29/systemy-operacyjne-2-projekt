#include "track.h"

Track::Track(int width, int height) {
    this->width = width;
    this->height = height;
}

void Track::printHorizontalWall(int startY, int startX, int endX, char c[2], short index) {
    attron(COLOR_PAIR(index));
    move(startY, startX);
    for (int i = 0; i < endX; i++) {
        printw(c);
        startX++;
    }
    attroff(COLOR_PAIR(index));
}

void Track::printVerticalWall(int startY, int startX, int endY, char c[2], short index) {
    attron(COLOR_PAIR(index));
    for (int i = 0; i < endY; i++) {
        startY++;
        move(startY, startX);
        printw(c);
    }
    attroff(COLOR_PAIR(index));
}