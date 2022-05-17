#pragma once

#include <ncurses.h>
#include "track.h"

#define BLUE_PAIR 1

class Track1 : private Track {
    public:
        using Track::Track;

    private:
        void printInnerWall(int startY, int startX, char c[2], short index);
        void printOuterWall(int startY, int startX, char c[2], short index);
    public:
        void print();
};
