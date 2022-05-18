#include "track2.h"

void Track2::printWall(int startY, int startX, int width, int height) {
    printHorizontalWall(startY, startX, width);
    printVerticalWall(startY, startX + width - 1, height);
    printVerticalWall(startY, startX, height);
    printHorizontalWall(startY + height + 1, startX, width);
}

void Track2::print() {
    int width = this->width;
    int height = this->height - 2;
    int startY = 0;
    int startX = 45;

    printWall(startY, startX, width, height);

    startY += 4;
    startX += 8;
    width -= 16;
    height -= 8;
    printWall(startY, startX, width, height);
}