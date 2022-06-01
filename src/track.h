#pragma once

#include <ncurses.h>

class Track {
   protected:
    int width;
    int height;
    char c;
    short color;

   public:
    Track(int width, int height, char c, short color);

   protected:
    void printHorizontalWall(int startY, int startX, int endX);
    void printVerticalWall(int startY, int startX, int endY);

   public:
    virtual void print() = 0;
};