#pragma once

#include <ncurses.h>

class Track {
    protected:
        int width;
        int height;

    public:
        Track(int width, int height);
    protected:
        void printHorizontalWall(int startY, int startX, int endX, char c[2], short index);
        void printVerticalWall(int startY, int startX, int endY, char c[2], short index);
    public:
        virtual void print() = 0;
};