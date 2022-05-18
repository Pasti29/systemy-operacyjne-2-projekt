#pragma once

#include <ncurses.h>

#include "track.h"

class Track2 : private Track {
    public:
        using Track::Track;

    private:
        void printWall(int startY, int startX, int width, int height);
    public:
        void print();
};
