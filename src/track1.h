#pragma once

#include <ncurses.h>

#include "track.h"

class Track1 : private Track {
   public:
    using Track::Track;

   private:
    void printInnerWall(int startY, int startX);
    void printOuterWall(int startY, int startX);

   public:
    void print();
};
