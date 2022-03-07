#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <cstdlib>
#include <time.h>

pthread_t threadDraw;

std::list<std::vector<int>> L;

void horizontalWall(int startX, int startY, int width, char c[2], short index)
{
    attron(COLOR_PAIR(index));
    move(startX, startY);
    for (int i = 0; i < width; i++)
    {
        printw(c);
        startY++;
    }
    attroff(COLOR_PAIR(index));
}

void verticalWall(int startX, int startY, int height, char c[2], short index)
{
    attron(COLOR_PAIR(index));
    for (int i = 0; i < height; i++)
    {
        startX++;
        move(startX, startY);
        printw(c);
    }
    attroff(COLOR_PAIR(index));
}

void outerWallTrack1(int startX, int startY, int width, int height, char c[2], short index)
{
    horizontalWall(startX, startY, width, c, index);
    verticalWall(startX, startY + width - 1, height, c, index);
    horizontalWall(startX + height + 1, startY, width, c, index);
}

void innerWallTrack1(int startX, int startY, int width, int height, char c[2], short index)
{
   horizontalWall(startX, startY, 6, c, index);
   horizontalWall(startX, startY  + 13, width - 13, c, index);
   verticalWall(startX, startY + width - 1, height, c, index);
   horizontalWall(startX + height + 1, startY, 6, c, index);
   horizontalWall(startX  + height + 1, startY  + 13, width - 13, c, index);
   verticalWall(startX, startY + 5, height, c, index);
   verticalWall(startX, startY + 13, height, c, index);
}

void wallTrack2(int startX, int startY, int width, int height, char c[2], short index)
{
    horizontalWall(startX, startY, width, c, index);
    verticalWall(startX, startY + width - 1, height, c, index);
    verticalWall(startX, startY, height, c, index);
    horizontalWall(startX + height + 1, startY, width, c, index);
}

void buildTrack1(int width, int height)
{
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    height -= 2;
    int startX = 10;
    int startY = 20;

    outerWallTrack1(startX, startY, width, height, (char*)"@", 1);

    startX += 4;
    width -= 8;
    height -= 8;
    innerWallTrack1(startX, startY, width, height, (char*)"@", 1);
}

void buildTrack2(int width, int height)
{
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    height -= 2;
    int startX = 0;
    int startY = 45;

    wallTrack2(startX, startY, width, height, (char*)"#", 2);

    startX += 4;
    startY += 8;
    width -= 16;
    height -= 8;
    wallTrack2(startX, startY, width, height, (char*)"#", 2);
}

void *moveChar(void *args)
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
        // else
        // {
        //     sleep(1);
        // }
        
    }
    return NULL;
}

void *moveCar(void *threadMove)
{
    int *sleepTime = (int *)threadMove;
    int startX = 12;
    int startY = 20;
    int newX = startX;
    int newY = startY;
    while(true)
    {
        L.push_back({startX, startY, newX, newY});
        sleep(*sleepTime);
        startX = newX;
        startY = newY;
        newY++;
    }
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));

    pthread_t thread;
    pthread_t thread1, thread2, thread3, thread4;
    int v1, v2, v3, v4;
    initscr();
    curs_set(0);
    start_color();
    buildTrack1(100, 30);
    buildTrack2(50, 50);
    refresh();
    sleep(5);
    v1 = 4;
    v2 = 3;
    v3 = 2;
    v4 = 1;
    pthread_create(&thread, NULL, moveChar, NULL);
    pthread_create(&thread1, NULL, moveCar, &v1);
    // moveCar();
    sleep(10);
    pthread_create(&thread2, NULL, moveCar, &v2);
    sleep(10);
    pthread_create(&thread3, NULL, moveCar, &v3);
    sleep(10);
    pthread_create(&thread4, NULL, moveCar, &v4);
    pthread_join(thread, NULL);
    getch();
    endwin();

    return 0;
}
