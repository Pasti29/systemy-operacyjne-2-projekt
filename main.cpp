#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <cstdlib>
#include <thread>

std::list<std::vector<int>> L;

void horizontalWall(int startY, int startX, int width, char c[2], short index)
{
    attron(COLOR_PAIR(index));
    move(startY, startX);
    for (int i = 0; i < width; i++)
    {
        printw(c);
        startX++;
    }
    attroff(COLOR_PAIR(index));
}

void verticalWall(int startY, int startX, int height, char c[2], short index)
{
    attron(COLOR_PAIR(index));
    for (int i = 0; i < height; i++)
    {
        startY++;
        move(startY, startX);
        printw(c);
    }
    attroff(COLOR_PAIR(index));
}

void outerWallTrack1(int startY, int startX, int width, int height, char c[2], short index)
{
    horizontalWall(startY, startX, width, c, index);
    verticalWall(startY, startX + width - 1, height, c, index);
    horizontalWall(startY + height + 1, startX, width, c, index);
}

void innerWallTrack1(int startY, int startX, int width, int height, char c[2], short index)
{
   horizontalWall(startY, startX, 6, c, index);
   horizontalWall(startY, startX  + 13, width - 13, c, index);
   verticalWall(startY, startX + width - 1, height, c, index);
   horizontalWall(startY + height + 1, startX, 6, c, index);
   horizontalWall(startY  + height + 1, startX  + 13, width - 13, c, index);
   verticalWall(startY, startX + 5, height, c, index);
   verticalWall(startY, startX + 13, height, c, index);
}

void wallTrack2(int startY, int startX, int width, int height, char c[2], short index)
{
    horizontalWall(startY, startX, width, c, index);
    verticalWall(startY, startX + width - 1, height, c, index);
    verticalWall(startY, startX, height, c, index);
    horizontalWall(startY + height + 1, startX, width, c, index);
}

void buildTrack1(int width, int height)
{
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    height -= 2;
    int startY = 10;
    int startX = 20;

    outerWallTrack1(startY, startX, width, height, (char*)"@", 1);

    startY += 4;
    width -= 8;
    height -= 8;
    innerWallTrack1(startY, startX, width, height, (char*)"@", 1);
}

void buildTrack2(int width, int height)
{
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    height -= 2;
    int startY = 0;
    int startX = 45;

    wallTrack2(startY, startX, width, height, (char*)"#", 2);

    startY += 4;
    startX += 8;
    width -= 16;
    height -= 8;
    wallTrack2(startY, startX, width, height, (char*)"#", 2);
}

void moveChar()
{
    while (true)
    {
        if (!L.empty())
        {
            std::vector<int> v = L.front();
            L.pop_front();
            mvaddch(v[0], v[1], ' ');
            mvaddch(v[2], v[3], 'C');
            refresh();
        }
        else
        {
            usleep(1);
        }
    }
    return;
}

void moveCar(int sleepTime)
{
    int startY = 12;
    int startX = 20;
    int newY = startY;
    int newX = startX;
    int lap = 0;

    while(true)
    {
        L.push_back({startY, startX, newY, newX});
        usleep(sleepTime);
        startY = newY;
        startX = newX;
        if (lap <= 3)
        {
            if (startY == 12 && startX == 29)
            {
                lap++;
            }

            if (startY == 35 && startX > 29)
            {
                newX--;
            }
            else if (startY < 35 && startX == 115)
            {
                newY++;
            }
            else if (startY == 12 && startX < 115)
            {
                newX++;
            }
            else if (lap < 3)
            {
                newY--;
            }
            else if (lap == 3 && startY == 35 && startX > 20)
            {
                newX--;
            }
            else
            {
                return;
                
            }
        }
        
    }
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));

    // pthread_t thread;

    initscr();
    curs_set(0);
    start_color();
    buildTrack1(100, 28);
    buildTrack2(46, 46);
    refresh();

    std::thread t(moveChar);

    std::list<std::thread> threadList;

    while (true)
    {
        int v = (rand() % 50000) + 50000;
        threadList.push_back(std::thread(moveCar, v));
        sleep((rand() % 10) + 5);
    }
    t.join();
    endwin();

    return 0;
}
