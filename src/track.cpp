#include "track.h"

Track::Track(int width, int height, char c, short color) {
    this->width = width;
    this->height = height;
    this->c = c;
    this->color = color;
}

void Track::printHorizontalWall(int startY, int startX, int endX) {
    attron(COLOR_PAIR(color));
    for (int i = 0; i < endX; i++) {
        mvaddch(startY, startX, c);
        startX++;
    }
    attroff(COLOR_PAIR(color));
}

void Track::printVerticalWall(int startY, int startX, int endY) {
    attron(COLOR_PAIR(color));
    for (int i = 0; i < endY; i++) {
        startY++;
        mvaddch(startY, startX, c);
    }
    attroff(COLOR_PAIR(color));
}