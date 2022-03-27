#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>
#include <iostream>

#define BLUE_PAIR 1
#define CYAN_PAIR 2
#define GREEN_PAIR 3
#define MAGENTA_PAIR 4
#define RED_PAIR 5
#define WHITE_PAIR 6
#define YELLOW_PAIR 7

/*
    Struktura z informacją o danych samochodu.
*/
struct CAR
{
    int x;
    int y;
    int sleepTime;
    char c;
    int color;
    bool active = true;
};

std::list<CAR> CAR_INFO_LIST;

bool ENDING = false;

/*
    Rysowanie poziomej ściany od punktu [startX, startY] o długości
    width, złożony z znaku c. index oznacza kolor znaków.
*/
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

/*
    Rysowanie pionowej ściany od punktu [startX, startY] o długości
    height, złożony z znaku c. index oznacza kolor znaków.
*/
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

/*
    Rysowanie zewnętrznej ściany dla toru 1
*/
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

/*
    Rysowanie wewnętrznej ściany dla toru 1
*/
void innerWallTrack1(int startY, int startX, int width, int height, char c[2], short index)
{
    horizontalWall(startY, startX, 6, c, index);
    horizontalWall(startY, startX + 13, 13, c, index);
    horizontalWall(startY, startX + 33, 29, c, index);
    horizontalWall(startY, startX + 70, 22, c, index);
    verticalWall(startY, startX + width - 1, height, c, index);
    horizontalWall(startY + height + 1, startX, 6, c, index);
    horizontalWall(startY + height + 1, startX + 13, 13, c, index);
    horizontalWall(startY + height + 1, startX + 33, 29, c, index);
    horizontalWall(startY + height + 1, startX + 70, 22, c, index);
    verticalWall(startY, startX + 5, height, c, index);
    verticalWall(startY, startX + 13, height, c, index);
}

/*
    Rysowanie ścian do toru 2
*/
void wallTrack2(int startY, int startX, int width, int height, char c[2], short index)
{
    horizontalWall(startY, startX, width, c, index);
    verticalWall(startY, startX + width - 1, height, c, index);
    verticalWall(startY, startX, height, c, index);
    horizontalWall(startY + height + 1, startX, width, c, index);
}

/*
    Rysowanie toru nr 1
*/
void buildTrack1(int width, int height)
{
    height -= 2;
    int startY = 10;
    int startX = 20;

    outerWallTrack1(startY, startX, width, height, (char *)"@", BLUE_PAIR);

    startY += 4;
    width -= 8;
    height -= 8;
    innerWallTrack1(startY, startX, width, height, (char *)"@", BLUE_PAIR);
}

/*
    Rysowanie toru nr 2
*/
void buildTrack2(int width, int height)
{
    height -= 2;
    int startY = 0;
    int startX = 45;

    wallTrack2(startY, startX, width, height, (char *)"#", CYAN_PAIR);

    startY += 4;
    startX += 8;
    width -= 16;
    height -= 8;
    wallTrack2(startY, startX, width, height, (char *)"#", CYAN_PAIR);
}

/*
    Funkcja wykonywana przez wątek printCarsThread.
    Rysuje co 1ms samochody na ekranie.
    Lista 'list' jest potrzebna do zamazywania pól, na których
    poprzednio znajdowały się samochody.
    Wątek kończy pracę, gdy został wysłany sygnał o kończeniu programu
    (zmienna ENDING) oraz wszystkie samochody z listy CAR_INFO_LIST
    zostaną usunięte.
*/
void printCars()
{
    std::list<std::vector<int>> list;
    nodelay(stdscr, TRUE);
    int ch;

    while (true)
    {
        if ((ch = getch()) == ERR)
        {
            while (!list.empty())
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
        else if (ch == 'q')
        {
            ENDING = true;
            return;
        }
    }
    return;
}

/*
    Funkcja wykonywająca ruch samochodu po torze nr. 2
*/
void moveCarTrack2(CAR *car)
{
    int startX = (*car).x;
    int startY = (*car).y;
    int lap = 0;

    while (!ENDING)
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
    return;
}

/*
    Funkcja wykonywająca ruch samochodu po torze nr. 1
*/
void moveInnerCar(CAR *car)
{
    int lap = 0;

    while (!ENDING)
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
            return;
        }
    }
    return;
}

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distSleep(50'000, 100'000);
    std::uniform_int_distribution<> distChar(66, 90);
    std::uniform_int_distribution<> distColor(3, 7);
    std::uniform_int_distribution<> distFactorySleep(2, 5);

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

    std::thread printCarsThread(printCars);

    std::list<std::thread> threadList;

    std::uniform_int_distribution<> distY(2, 43);
    std::uniform_int_distribution<> distX(49, 86);
    /*
        Tworzenie trzech wątków, które przedstawiają trzy
        samochody jadące na torze nr. 2.
    */
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
        // std::thread t(moveCarTrack2, &(CAR_INFO_LIST.back()));
        CAR_INFO_LIST.push_back({x, y, 60'000, (char)distChar(gen), distColor(gen)});
        threadList.push_back(std::move(std::thread(moveCarTrack2, &(CAR_INFO_LIST.back()))));
    }
    /*
        Tworzenie dziesięciu wątków, które przedstawiają dziesięć
        samochodów jadących na torze nr. 1.
    */

    while (!ENDING)
    {
        int randomSleepTime = distSleep(gen);
        char randomChar = (char)distChar(gen);
        int randomColor = distColor(gen);

        CAR_INFO_LIST.push_back({20, 12, randomSleepTime, randomChar, randomColor});
        threadList.push_back(std::move(std::thread(moveInnerCar, &(CAR_INFO_LIST.back()))));

        sleep(distFactorySleep(gen));
    }

    printCarsThread.join();

    while (!threadList.empty())
    {
        threadList.front().join();
        threadList.pop_front();
    }
    
    endwin();

    return 0;
}
