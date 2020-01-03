#include <bits/stdc++.h> /*
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>*/
#include <unistd.h>
#include "pixel.h"
#include "simulation.h"

#define DEBUG if (0)
#define COUT cout << "\e[36m"
#define VAR(v) " [\e[32m" << #v << "\e[36m=\e[91m" << v << "\e[36m] "
#define ENDL "\e[39m" << endl

using namespace std;

bool Simulation::checkIndexes(int x, int y)
{
    return ((x >= 0 && y >= 0 && x < rows && y < columns)? 1 : 0);
}

bool Simulation::shift(Pixel& curPix, int dvx, int dvy){DEBUG COUT << "PIXEL SHIFT" << VAR(curPix.x) << VAR(curPix.y) <<VAR(curPix.icon) << ENDL;
//curPix.resetMoved();
return false;
}/**/

bool Simulation::shift(Air& curPix, int dvx, int dvy)
{
    DEBUG COUT << "AIR SHIFT" << ENDL;
    return true;
}

bool Simulation::shift(Earth& curPix, int dvx, int dvy)
{
    DEBUG COUT << "EARTH SHIFT" << ENDL;
    return false;
}

bool Simulation::shift(Water& curPix, int dvx, int dvy)
{
    int& x = curPix.x, y =curPix.y;
    if(curPix.moved)
    {
        return false;
    }
    curPix.moved = 1;
    DEBUG COUT << "WATERSHIFT" << ENDL;
    curPix.vx = ((curPix.vx+dvx <= -1)? -1 : ((curPix.vx+dvx >= 1)? 1 : 0)); ///so the velocity belongs to <-1, 1>
    curPix.vy = ((curPix.vy+dvy <= -1)? -1 : ((curPix.vy+dvy >= 1)? 1 : 0)); ///so the velocity belongs to <-1, 1>
    DEBUG COUT << "Shifting " << VAR(x) << VAR(y)  << VAR(curPix.vx) << VAR(curPix.vy) << ENDL;

    if(world[x+curPix.vx][y+curPix.vy]->moved == 0 && COUT << "INTERNAL SHIFT" << ENDL &&  world[x+curPix.vx][y+curPix.vy]->dispatchShift(*this, curPix.vx+1, curPix.vy))
    {
        DEBUG COUT << "ACCUALLY SHIFTED" << VAR(x) << VAR(y) << VAR(curPix.vx) << VAR(curPix.vy) << ENDL;
        DEBUG print();
        int oldX = x, oldY = y; ///for not to swap on swapped indexes
        swap(world[oldX][oldY], world[oldX+curPix.vx][oldY+curPix.vy]);
        swap(world[oldX][oldY]->x, world[oldX+curPix.vx][oldY+curPix.vy]->x);
        swap(world[oldX][oldY]->y, world[oldX+curPix.vx][oldY+curPix.vy]->y);

        DEBUG print();
        DEBUG checkAllCoords();
        curPix.moved = 1;
        return true;
    }
    else
    {
        int directionChangeX = 1;
        int directionChangeY = -1;
        if(rand() %2)
            swap(directionChangeX, directionChangeY);

        if(world[x+curPix.vy*directionChangeX][y+curPix.vx*directionChangeY]->moved == 0 && COUT << "REDIRECTING SHIFT" << ENDL &&  world[x+curPix.vy*directionChangeX][y+curPix.vx*directionChangeY]->dispatchShift(*this, curPix.vy*directionChangeX+1, curPix.vx*directionChangeY))
        {
            DEBUG COUT << "ACCUALLY REDIRECTED AND SHIFTED" << VAR(x) << VAR(y) << VAR(curPix.vy*directionChangeX) << VAR(curPix.vx*directionChangeY) << ENDL;
            DEBUG print();


            if(checkIndexes(x+curPix.vy*directionChangeX, y+curPix.vx*directionChangeY) == 0 || checkIndexes(x, y) == 0)
            {
                DEBUG COUT << VAR(x) << VAR(y) << VAR(x+curPix.vy*directionChangeX) << VAR(y+curPix.vx*directionChangeY) << ENDL;
            }
            int oldX = x, oldY = y;  ///for not to swap on swapped indexes
            swap(world[oldX][oldY], world[oldX+curPix.vy*directionChangeX][oldY+curPix.vx*directionChangeY]);
            swap(world[oldX][oldY]->x, world[oldX+curPix.vy*directionChangeX][oldY+curPix.vx*directionChangeY]->x);
            swap(world[oldX][oldY]->y, world[oldX+curPix.vy*directionChangeX][oldY+curPix.vx*directionChangeY]->y);
            curPix.vx = curPix.vy*directionChangeX;
            curPix.vy = curPix.vx*directionChangeY;
            DEBUG print();
            DEBUG checkAllCoords();
            curPix.moved = 1;
            return true;
        }

        curPix.moved = 0;
    }
    return false;
}


void Simulation::clearMoved()
{
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            world[i][j]->resetMoved(0);
        }
    }
}

bool Simulation::checkAllCoords()
{
    bool ret = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if(world[i][j]->x != i || world[i][j]->y != j)
            {
                DEBUG COUT << "i, j = " << i << ", " << j << " | x, y = " << world[i][j]->x << ", " << world[i][j]->y << ENDL;
                ret = 1;
            }
        }
    }
    return ret;
}

void Simulation::calculate()
{
    clearMoved();
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            world[i][j]->dispatchShift(*this, 1, 0);
            //print();
            DEBUG COUT << ENDL;
        }
    }
    DEBUG COUT << "CALC DONE" << ENDL << ENDL;
}


void Simulation::print()//█
{
    for(int i = 0; i < rows; ++i)
    {
        DEBUG cout << i;
        for(int j = 0; j < columns; ++j)
        {

            cout << world[i][j]->icon;
        }
        cout << endl;
    }
}

void Simulation::simulate(int timeLimit)
{
    system("clear");
    print();
    for(int t = 0; t < timeLimit; ++t)
    {
        calculate();
        usleep(300000);                  ///CYCLE TIME
        system("clear");
        print();
        checkAllCoords();
        //usleep(3000000);
    }
}

void Simulation::input()
{
    cin >> rows >> columns;

    world.resize(rows);
    for(vector<unique_ptr<Pixel>>& el : world)
        el.resize(columns);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            char ch = 0;
            cin >> ch;
            if(ch == 'W')
            {
                world[i][j] = make_unique<Water>(i, j);
            }
            else if(ch == 'E')
            {
                world[i][j] = make_unique<Earth>(i, j);
            }
            else if(ch == '.')
            {
                world[i][j] = make_unique<Air>(i, j);
            }
        }
    }
    DEBUG COUT << "INPUT DONE" << VAR(columns) << VAR(rows)<< ENDL;
}

Simulation::Simulation(int seed /*= 1*/)
{
    srand(seed);
}

