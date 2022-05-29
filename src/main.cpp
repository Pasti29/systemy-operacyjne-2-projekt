#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "track1.h"
#include "track2.h"

#define BLUE_PAIR       1
#define CYAN_PAIR       2
#define GREEN_PAIR      3
#define MAGENTA_PAIR    4
#define RED_PAIR        5
#define WHITE_PAIR      6
#define YELLOW_PAIR     7

#define UP      1
#define RIGHT   2
#define DOWN    3
#define LEFT    4

/*
    Etap II
    Utworzyć czekanie na skrzyżowaniu.
    Jeżeli samochód jest na skrzyżowaniu, to inne samochody, które chcą wjechać
    na to skrzyżowanie, muszą zaczekać. Jak już samochód wyjedzie ze skrzyżowania, to
    jeden samochód może przejechać i jak ten przejedzie, to wtedy mogą inne przejechać.
*/

std::mutex MUTEXES[4];

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
    int direction;
    bool active = true;
    bool blocked = false;
};

std::list<CAR> CAR_INFO_LIST;
std::list<CAR*> CROSSROAD_WAITING_LIST_1;
std::list<CAR*> CROSSROAD_WAITING_LIST_2;
std::list<CAR*> CROSSROAD_WAITING_LIST_3;
std::list<CAR*> CROSSROAD_WAITING_LIST_4;


bool ENDING = false;

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

void moveThroughCrossing(CAR *car, short direction, short mutexNumber) {
    // MUTEXES[mutexNumber].lock();
    switch (direction) {
    case UP: case DOWN:
        for (int i = 0; i < 5; i++) {
            if (direction == UP) {
                (*car).y--;
            } else {
                (*car).y++;
            }
            if (i < 4)
                usleep((*car).sleepTime);
        }
        break;
    case RIGHT: case LEFT:
        for (int i = 0; i < 10; i++) {
            if (direction == RIGHT) {
                (*car).x++;
            } else {
                (*car).x--;
            }
            if (i < 9)
                usleep((*car).sleepTime);
        }
        break;
    default:
        break;
    }
    (*car).blocked = false;
    // MUTEXES[mutexNumber].unlock();
}

void threadHandleCrossroad1(short crossroadNumber) {

    while (true) {
        if (ENDING) return;

        if (CROSSROAD_WAITING_LIST_1.empty()) usleep(10000);

        while (!CROSSROAD_WAITING_LIST_1.empty()) {
            MUTEXES[crossroadNumber].lock();
            auto car  = CROSSROAD_WAITING_LIST_1.front();
            CROSSROAD_WAITING_LIST_1.pop_front();
            MUTEXES[crossroadNumber].unlock();
            moveThroughCrossing(car, (*car).direction, crossroadNumber);
        }
    }
}

void threadHandleCrossroad2(short crossroadNumber) {

    while (true) {
        if (ENDING) return;

        if (CROSSROAD_WAITING_LIST_2.empty()) usleep(10000);

        while (!CROSSROAD_WAITING_LIST_2.empty()) {
            MUTEXES[crossroadNumber].lock();
            auto car  = CROSSROAD_WAITING_LIST_2.front();
            CROSSROAD_WAITING_LIST_2.pop_front();
            MUTEXES[crossroadNumber].unlock();
            moveThroughCrossing(car, (*car).direction, crossroadNumber);
        }
    }
}

void threadHandleCrossroad3(short crossroadNumber) {

    while (true) {
        if (ENDING) return;

        if (CROSSROAD_WAITING_LIST_3.empty()) usleep(10000);

        while (!CROSSROAD_WAITING_LIST_3.empty()) {
            MUTEXES[crossroadNumber].lock();
            auto car  = CROSSROAD_WAITING_LIST_3.front();
            CROSSROAD_WAITING_LIST_3.pop_front();
            MUTEXES[crossroadNumber].unlock();
            moveThroughCrossing(car, (*car).direction, crossroadNumber);
        }
    }
}

void threadHandleCrossroad4(short crossroadNumber) {

    while (true) {
        if (ENDING) return;

        if (CROSSROAD_WAITING_LIST_4.empty()) usleep(10000);

        while (!CROSSROAD_WAITING_LIST_4.empty()) {
            MUTEXES[crossroadNumber].lock();
            auto car  = CROSSROAD_WAITING_LIST_4.front();
            CROSSROAD_WAITING_LIST_4.pop_front();
            MUTEXES[crossroadNumber].unlock();
            moveThroughCrossing(car, (*car).direction, crossroadNumber);
        }
    }
}

void addToCrossroadWaitingList(CAR *car, std::list<CAR*> &waitingList, short crossroadNumber) {
    MUTEXES[crossroadNumber].lock();
    (*car).blocked = true;
    waitingList.push_back(car);
    MUTEXES[crossroadNumber].unlock();
}

/*
    Funkcja wykonywająca ruch samochodu po wewnętrznym torze
*/
void moveInnerCar(CAR *car)
{

    int startX = (*car).x;
    int startY = (*car).y;

    while (!ENDING)
    {
        usleep((*car).sleepTime);
        if ((*car).y > 2 && (*car).x == 49)
        {
            switch ((*car).y) {
            case 14:
                (*car).direction = UP;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_1, 0);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime);        
                break;
            case 37:
                (*car).direction = UP;

                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_4, 3);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime);
                break;
            default:
                (*car).y--;
                break;
            }
        }
        else if ((*car).y == 2 && (*car).x < 86)
        {
            (*car).x++;
        }
        else if ((*car).y < 43 && (*car).x == 86)
        {
            switch ((*car).y) {
            case 10:
                (*car).direction = DOWN;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_2, 1);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime);  
                break;
            case 33:
                (*car).direction = DOWN;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_3, 2);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime);  
                break;
            default:
                (*car).y++;
                break;
            }
        }
        else if ((*car).y == 43 && (*car).x > 49)
        {
            (*car).x--;
        }
    }
    return;
}

/*
    Funkcja wykonywająca ruch samochodu po zewnętrznym torze
*/
void moveOuterCar(CAR *car)
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
            switch ((*car).x) {
            case 90:
                (*car).direction = LEFT;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_3, 2);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime);  
                break;
            case 53:
                (*car).direction = LEFT;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_4, 3);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime); 
                break;
            default:
                (*car).x--;
                break;
            }
        }
        else if ((*car).y < 35 && (*car).x == 115)
        {
            (*car).y++;
        }
        else if ((*car).y == 12 && (*car).x < 115)
        {
            switch ((*car).x) {
            case 45:
                (*car).direction = RIGHT;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_1, 0);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime); 
                break;
            case 82:
                (*car).direction = RIGHT;
                
                addToCrossroadWaitingList(car, CROSSROAD_WAITING_LIST_2, 1);
                while ((*car).blocked && !ENDING) usleep((*car).sleepTime); 
                break;
            default:
                (*car).x++;
                break;
            }
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
    std::uniform_int_distribution<> distFactorySleep(3, 8);

    initscr();
    curs_set(0);
    start_color();
    noecho();

    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(MAGENTA_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);

    Track1 track1 = Track1(100, 28, '@', BLUE_PAIR);
    Track2 track2 = Track2(46, 46, '#', CYAN_PAIR);

    track1.print();
    track2.print();
    refresh();

    std::thread printCarsThread(printCars);

    std::thread crossroad1(threadHandleCrossroad1, 0);
    std::thread crossroad2(threadHandleCrossroad2, 1);
    std::thread crossroad3(threadHandleCrossroad3, 2);
    std::thread crossroad4(threadHandleCrossroad4, 3);

    std::list<std::thread> threadList;

    std::uniform_int_distribution<> distY(2, 43);
    std::uniform_int_distribution<> distX(49, 86);
    /*
        Tworzenie trzech wątków, które przedstawiają trzy
        samochody jadące po wewnętrznym torze.
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
        CAR_INFO_LIST.push_back({x, y, 60'000, (char)distChar(gen), distColor(gen)});
        threadList.push_back(std::move(std::thread(moveInnerCar, &(CAR_INFO_LIST.back()))));
    }
    /*
        Tworzenie dziesięciu wątków, które przedstawiają dziesięć
        samochodów jadących po zewnętrznym torze.
    */

    while (!ENDING)
    {
        int randomSleepTime = distSleep(gen);
        char randomChar = (char)distChar(gen);
        int randomColor = distColor(gen);

        CAR_INFO_LIST.push_back({20, 12, randomSleepTime, randomChar, randomColor});
        threadList.push_back(std::move(std::thread(moveOuterCar, &(CAR_INFO_LIST.back()))));

        sleep(distFactorySleep(gen));
    }

    printCarsThread.join();

    crossroad1.join();
    crossroad2.join();
    crossroad3.join();
    crossroad4.join();

    while (!threadList.empty())
    {
        threadList.front().join();
        threadList.pop_front();
    }
    
    endwin();

    return 0;
}
