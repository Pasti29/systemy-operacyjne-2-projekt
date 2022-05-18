#include "track1.h"

void Track1::printInnerWall(int startY, int startX) {
    int h = this->height - 10;
    int w = this->width - 8;
    printHorizontalWall(startY, startX, 6);
    printHorizontalWall(startY, startX + 13, 13);
    printHorizontalWall(startY, startX + 33, 29);
    printHorizontalWall(startY, startX + 70, 22);
    printVerticalWall(startY, startX + w - 1, h);
    printHorizontalWall(startY + h + 1, startX, 6);
    printHorizontalWall(startY + h + 1, startX + 13, 13);
    printHorizontalWall(startY + h + 1, startX + 33, 29);
    printHorizontalWall(startY + h + 1, startX + 70, 22);
    printVerticalWall(startY, startX + 5, h);
    printVerticalWall(startY, startX + 13, h);
}

void Track1::printOuterWall(int startY, int startX) {
    int h = this->height - 2;
    int w = this->width;
    printHorizontalWall(startY, startX, 25);
    printHorizontalWall(startY, startX + 34, 29);
    printHorizontalWall(startY, startX + 70, 30);
    printVerticalWall(startY, startX + w - 1, h);
    printHorizontalWall(startY + h + 1, startX, 25);
    printHorizontalWall(startY + h + 1, startX + 34, 29);
    printHorizontalWall(startY + h + 1, startX + 70, 30);
}

void Track1::print() {
    int startY = 10;
    int startX = 20;

    printOuterWall(startY, startX);

    startY += 4;
    printInnerWall(startY, startX);
}