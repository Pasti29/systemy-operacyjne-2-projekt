#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <cstdlib>
#include <thread>
#include <random>

#define BLUE_PAIR        1
#define CYAN_PAIR        2
#define GREEN_PAIR       3
#define MAGENTA_PAIR     4
#define RED_PAIR         5
#define WHITE_PAIR       6
#define YELLOW_PAIR      7

struct CAR {
    int oldX;
    int oldY;
    int newX;
    int newY;
    char c;
    int color;
};

std::list<CAR> L;

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
    horizontalWall(startY, startX, 25, c, index);
    horizontalWall(startY, startX + 34, 29, c, index);
    horizontalWall(startY, startX + 70, 30, c, index);
    verticalWall(startY, startX + width - 1, height, c, index);
    horizontalWall(startY + height + 1, startX, 25, c, index);
    horizontalWall(startY + height + 1, startX + 34, 29, c, index);
    horizontalWall(startY + height + 1, startX + 70, 30, c, index);
}

void innerWallTrack1(int startY, int startX, int width, int height, char c[2], short index)
{
   horizontalWall(startY, startX, 6, c, index);
   horizontalWall(startY, startX  + 13, 13, c, index);
   horizontalWall(startY, startX  + 33, 29, c, index);
   horizontalWall(startY, startX  + 70, 22, c, index);
   verticalWall(startY, startX + width - 1, height, c, index);
   horizontalWall(startY + height + 1, startX, 6, c, index);
   horizontalWall(startY  + height + 1, startX  + 13, 13, c, index);
   horizontalWall(startY  + height + 1, startX  + 33, 29, c, index);
   horizontalWall(startY  + height + 1, startX  + 70, 22, c, index);
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
    height -= 2;
    int startY = 10;
    int startX = 20;

    outerWallTrack1(startY, startX, width, height, (char*)"@", BLUE_PAIR);

    startY += 4;
    width -= 8;
    height -= 8;
    innerWallTrack1(startY, startX, width, height, (char*)"@", BLUE_PAIR);
}

void buildTrack2(int width, int height)
{
    height -= 2;
    int startY = 0;
    int startX = 45;

    wallTrack2(startY, startX, width, height, (char*)"#", CYAN_PAIR);

    startY += 4;
    startX += 8;
    width -= 16;
    height -= 8;
    wallTrack2(startY, startX, width, height, (char*)"#", CYAN_PAIR);
}

void moveChar()
{
    while (true)
    {
        if (!L.empty())
        {
            CAR v = L.front();
            L.pop_front();
            attron(COLOR_PAIR(v.color));
            mvaddch(v.oldX, v.oldY, ' ');
            mvaddch(v.newX, v.newY, v.c);
            attroff(COLOR_PAIR(v.color));
            refresh();
        }
        else
        {
            usleep(100);
        }
    }
    return;
}

void moveOuterCar(int sleepTime, char c, int color, int offset)
{
    int startY = 15 + offset;
    int startX = 49;
    int newY = startY;
    int newX = startX;

    while(true)
    {
        L.push_back({startY, startX, newY, newX, c, color});
        usleep(sleepTime);
        startY = newY;
        startX = newX;
        if (startY > 2 && startX == 49)
        {
            newY--;
        }

        if (startY == 2 && startX < 86)
        {
            newX++;
        }

        if (startY < 43 && startX == 86)
        {
            newY++;
        }

        if (startY == 43 && startX > 49)
        {
            newX--;
        }
    }
}

void moveInnerCar(int sleepTime, char c, int color)
{
    int startY = 12;
    int startX = 20;
    int newY = startY;
    int newX = startX;
    int lap = 0;

    while(true)
    {
        L.push_back({startY, startX, newY, newX, c, color});
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distSleep(50'000, 100'000);
    std::uniform_int_distribution<> distChar(66, 90);
    std::uniform_int_distribution<> distColor(3, 7);
    std::uniform_int_distribution<> distFactorySleep(5, 10);

    srand(time(NULL));

    initscr();
    curs_set(0);
    start_color();

    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(MAGENTA_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);

    buildTrack1(100, 28);
    buildTrack2(46, 46);
    refresh();

    std::thread t(moveChar);

    std::list<std::thread> threadList;

    threadList.push_back(std::thread(moveOuterCar, 60'000, (char)distChar(gen), distColor(gen), 0));
    usleep(10);
    threadList.push_back(std::thread(moveOuterCar, 60'000, (char)distChar(gen), distColor(gen), 10));
    usleep(10);
    threadList.push_back(std::thread(moveOuterCar, 60'000, (char)distChar(gen), distColor(gen), 20));
    usleep(10);

    while (true)
    {
        int v = distSleep(gen);
        char c = (char) distChar(gen);
        int color = distColor(gen);
        threadList.push_back(std::thread(moveInnerCar, v, c, color));
        sleep(distFactorySleep(gen));
    }
    t.join();
    endwin();

    return 0;
}
