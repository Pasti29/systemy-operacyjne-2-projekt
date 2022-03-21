#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <cstdlib>
#include <thread>
#include <random>
#include <iostream>

#define BLUE_PAIR        1
#define CYAN_PAIR        2
#define GREEN_PAIR       3
#define MAGENTA_PAIR     4
#define RED_PAIR         5
#define WHITE_PAIR       6
#define YELLOW_PAIR      7

struct CAR {
    int x;
    int y;
    int sleepTime;
    char c;
    int color;
    bool active = true;
};

std::list<CAR> CAR_INFO_LIST;

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

void removeDeactivated()
{
    while (true)
    {
        sleep(1);
        CAR_INFO_LIST.remove_if([](CAR c){ return !c.active; });
    }
}

void moveChar()
{
    std::list<std::vector<int>> list;
    while (true)
    {
        std::string s = std::to_string(CAR_INFO_LIST.size());
        mvaddch(1, 2, ' ');
        for (int i = 0; i < s.size(); i++)
            mvaddch(1, 1 + i, s[i]);
        while(!list.empty())
        {
            std::vector<int> v = list.front();
            list.pop_front();
            mvaddch(v[0], v[1], ' ');
        }

        for (CAR car : CAR_INFO_LIST)
        {
            attron(COLOR_PAIR(car.color));
            mvaddch(car.y, car.x, car.c);
            list.push_back({car.y, car.x});
            attroff(COLOR_PAIR(car.color));
            
            
        }
        refresh();
        
        usleep(1'000);
    }
    return;
}

void moveOuterCar(CAR* car)
{
    while(true)
    {
        usleep((*car).sleepTime);
        if ((*car).y > 2 && (*car).x == 49)
        {
            (*car).y = (*car).y - 1;
        }
        else if ((*car).y == 2 && (*car).x < 86)
        {
            (*car).x = (*car).x + 1;
        }
        else if ((*car).y < 43 && (*car).x == 86)
        {
            (*car).y = (*car).y + 1;
        }
        else if ((*car).y == 43 && (*car).x > 49)
        {
            (*car).x = (*car).x - 1;
        }
    }
}

void moveInnerCar(CAR* car)
{
    int lap = 0;

    while(true)
    {
        usleep((*car).sleepTime);
        if ((*car).y == 12 && (*car).x == 29)
        {
            lap++;
        }

        if ((*car).y == 35 && (*car).x > 29)
        {
            (*car).x--;
        }
        else if ((*car).y < 35 && (*car).x == 115)
        {
            (*car).y++;
        }
        else if ((*car).y == 12 && (*car).x < 115)
        {
            (*car).x++;
        }
        else if (lap < 3)
        {
            (*car).y--;
        }
        else if (lap == 3 && (*car).y == 35 && (*car).x > 20)
        {
            (*car).x--;
        }
        else
        {
            (*car).active = false;
            return;
            
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
    std::thread garbageCollectorThread(removeDeactivated);

    std::list<std::thread> threadList;

    std::uniform_int_distribution<> distY(2, 43);
    std::uniform_int_distribution<> distX(49, 86);
    for (int i = 0; i < 3; i++)
    {
        int y = distY(gen);
        int x = distX(gen);

        if (y > 2 && y < 43)
        {
            if (x % 2)
                x = 49;
            else
                x = 86;
        }

        CAR_INFO_LIST.push_back({x, y, 60'000, (char)distChar(gen), distColor(gen)});
        threadList.push_back(std::thread(moveOuterCar, &(CAR_INFO_LIST.back())));
    }

    while (true)
    {
        int randomSleepTime = distSleep(gen);
        char randomChar = (char) distChar(gen);
        int randomColor = distColor(gen);

        CAR_INFO_LIST.push_back({20, 12, randomSleepTime, randomChar, randomColor});
        threadList.push_back(std::thread(moveInnerCar, &(CAR_INFO_LIST.back())));

        sleep(distFactorySleep(gen));
    }
    t.join();
    garbageCollectorThread.join();
    endwin();

    return 0;
}
