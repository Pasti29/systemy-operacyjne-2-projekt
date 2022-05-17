#include "track1.h"

void Track1::printInnerWall(int startY, int startX, char c[2], short index) {
    int height = this->height - 2;
    int width = this->width;
    printHorizontalWall(startY, startX, 25, c, index);
    printHorizontalWall(startY, startX + 34, 29, c, index);
    printHorizontalWall(startY, startX + 70, 30, c, index);
    printVerticalWall(startY, startX + width - 1, height, c, index);
    printHorizontalWall(startY + height + 1, startX, 25, c, index);
    printHorizontalWall(startY + height + 1, startX + 34, 29, c, index);
    printHorizontalWall(startY + height + 1, startX + 70, 30, c, index);
}

void Track1::printOuterWall(int startY, int startX, char c[2], short index) {
    int height = this->height - 10;
    int width = this->width - 8;
    printHorizontalWall(startY, startX, 6, c, index);
    printHorizontalWall(startY, startX + 13, 13, c, index);
    printHorizontalWall(startY, startX + 33, 29, c, index);
    printHorizontalWall(startY, startX + 70, 22, c, index);
    printVerticalWall(startY, startX + width - 1, height, c, index);
    printHorizontalWall(startY + height + 1, startX, 6, c, index);
    printHorizontalWall(startY + height + 1, startX + 13, 13, c, index);
    printHorizontalWall(startY + height + 1, startX + 33, 29, c, index);
    printHorizontalWall(startY + height + 1, startX + 70, 22, c, index);
    printVerticalWall(startY, startX + 5, height, c, index);
    printVerticalWall(startY, startX + 13, height, c, index);
}

void Track1::print() {
    int startY = 10;
    int startX = 20;

    printOuterWall(startY, startX, (char *)"@", BLUE_PAIR);

    startY += 4;
    printInnerWall(startY, startX, (char *)"@", BLUE_PAIR);
}