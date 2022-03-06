#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>

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

void* moveCar(void* threadMove)
{
    pthread_t thread = (pthread_t) threadMove;
    int startX = 12;
    int startY = 20;
    while(true)
    {
        
        sleep(1);
    }
}

void moveChar(void *args)
{
    int *input = (int *) args; 
    int startX = *input;
    int startY = *(input + 1);
    int endX = *(input + 2);
    int endY = *(input + 3);

    mvaddch(startX, startY, ' ');
    mvaddch(endX, endY, 'C');
    refresh();
}

int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;
    initscr();
    curs_set(0);
    start_color();
    buildTrack1(100, 30);
    buildTrack2(50, 50);
    refresh();
    sleep(2);
    pthread_create(&thread1, NULL, moveCar, NULL);
    // moveCar();
    sleep(4);
    pthread_create(&thread2, NULL, moveCar, NULL);
    getch();
    endwin();
    return 0;
}
