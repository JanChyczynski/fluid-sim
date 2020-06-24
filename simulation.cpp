#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <unistd.h>
#include "pixel.h"
#include "simulation.h"

#define DEBUG if (0)
#define COUT cout << "\e[36m"
#define VAR(v) " [\e[32m" << #v << "\e[36m=\e[91m" << v << "\e[36m] "
#define ENDL "\e[39m" << endl

using namespace std;

bool Simulation::checkIndices(int x, int y)
{
    return ((x >= 0 && y >= 0 && x < rows && y < columns)? 1 : 0);
}

inline int Simulation::normV(int v)
{
    return ((v <= -1)? -1 : ((v >= 1)? 1 : 0));  ///so the velocity belongs to <-1, 1>
}

bool Simulation::shift(Pixel& currentPixel, int deltavx, int deltavy){DEBUG COUT << "PIXEL SHIFT" << VAR(currentPixel.x) << VAR(currentPixel.y) <<VAR(currentPixel.icon) << ENDL;
//currentPixel.resetMoved();
return false;
}/**/

bool Simulation::shift(Air& currentPixel, int deltavx, int deltavy)
{
    DEBUG COUT << "AIR SHIFT" << ENDL;
    return true;
}

bool Simulation::shift(Earth& currentPixel, int deltavx, int deltavy)
{
    DEBUG COUT << "EARTH SHIFT" << ENDL;
    return false;
}

bool Simulation::shift(Water& currentPixel, int deltavx, int deltavy) ///It doesn't follow physics
{
    int& x = currentPixel.x, y =currentPixel.y;
    if(currentPixel.moved)
    {
        return false;
    }
    if(deltavx > 0 && currentPixel.vx < -1)
    {
        currentPixel.vx /= 2;
    }
    if(deltavy > 0 && currentPixel.vy < -1)
    {
        currentPixel.vy /= 2;
    }
    currentPixel.moved = 1;
    DEBUG COUT << "WATERSHIFT" << ENDL;

    currentPixel.vx += deltavx;
    currentPixel.vy += deltavy;
    DEBUG COUT << "Shifting " << VAR(x) << VAR(y)  << VAR(currentPixel.vx) << VAR(currentPixel.vy) << ENDL;

    if(world[x+normV(currentPixel.vx)][y+normV(currentPixel.vy)]->moved == 0 &&  world[x+normV(currentPixel.vx)][y+normV(currentPixel.vy)]->dispatchShift(*this, currentPixel.vx+1, currentPixel.vy))
    {
        DEBUG COUT << "ACCUALLY SHIFTED" << VAR(x) << VAR(y) << VAR(currentPixel.vx) << VAR(currentPixel.vy) << ENDL;
        DEBUG print();
        int oldX = x, oldY = y; ///for not to swap on swapped indices
        swap(world[oldX][oldY], world[oldX+normV(currentPixel.vx)][oldY+normV(currentPixel.vy)]);
        swap(world[oldX][oldY]->x, world[oldX+normV(currentPixel.vx)][oldY+normV(currentPixel.vy)]->x);
        swap(world[oldX][oldY]->y, world[oldX+normV(currentPixel.vx)][oldY+normV(currentPixel.vy)]->y);

        DEBUG print();
        DEBUG checkAllCoords();
        currentPixel.moved = 1;
        return true;
    }
    else
    {
        int directionChangeX = 1;
        int directionChangeY = -1;
        if(rand() %2)
            swap(directionChangeX, directionChangeY);

        int prePushVx = currentPixel.vx;
        int prePushVy = currentPixel.vy;

        //currentPixel.vx -= normV(currentPixel.vx);
        //currentPixel.vy -= normV(currentPixel.vy);
        currentPixel.vx /= 3;
        currentPixel.vy /= 2;
        for(int i = 0; i < 4 && (abs(currentPixel.vx) > 0 || abs(currentPixel.vy) > 0); i++, swap(directionChangeX, directionChangeY)) ///i: 0, 1 redirecting at free; 2, 3 redirecting and pushing
        {
            if(i== 2)
            {
                //currentPixel.vx -= normV(currentPixel.vx);
                //currentPixel.vy -= normV(currentPixel.vy);
                currentPixel.vx /= 3;
                currentPixel.vy /= 3;

            }

            if(i < 2 && world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]->free == 0)///at first try redirecting on a free box
            {
                continue;
            }
            if(world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]->moved == 0 && world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]->dispatchShift(*this, currentPixel.vy*directionChangeX+1, currentPixel.vx*directionChangeY))
            {
                DEBUG COUT << "ACCUALLY REDIRECTED AND SHIFTED" << VAR(x) << VAR(y) << VAR(currentPixel.vy*directionChangeX) << VAR(currentPixel.vx*directionChangeY) << ENDL;
                DEBUG print();


                if(checkIndices(x+currentPixel.vy*directionChangeX, y+currentPixel.vx*directionChangeY) == 0 || checkIndices(x, y) == 0)
                {
                    DEBUG COUT << VAR(x) << VAR(y) << VAR(x+currentPixel.vy*directionChangeX) << VAR(y+currentPixel.vx*directionChangeY) << ENDL;
                }
                int oldX = x, oldY = y;  ///for not to swap on swapped indexes
                swap(world[oldX][oldY], world[oldX+normV(currentPixel.vy*directionChangeX)][oldY+normV(currentPixel.vx*directionChangeY)]);
                swap(world[oldX][oldY]->x, world[oldX+normV(currentPixel.vy*directionChangeX)][oldY+normV(currentPixel.vx*directionChangeY)]->x);
                swap(world[oldX][oldY]->y, world[oldX+normV(currentPixel.vy*directionChangeX)][oldY+normV(currentPixel.vx*directionChangeY)]->y);
                currentPixel.vx = currentPixel.vy*directionChangeX;
                currentPixel.vy = currentPixel.vx*directionChangeY;
                DEBUG print();
                DEBUG checkAllCoords();
                currentPixel.moved = 1;
                return true;
            }

        }
        currentPixel.vx = prePushVx;
        currentPixel.vy = prePushVy;

        if(abs(currentPixel.vx) > 3) currentPixel.vx /= 2;
        if(abs(currentPixel.vy) > 1) currentPixel.vy /= 2;

        currentPixel.moved = 0;
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
            if(i < rows-1 && world[i+1][j]->solid == 0) ///free fall
            {
                world[i][j]->dispatchShift(*this, 1, 0);
            }
            else if(i < rows-1 && j > 0 && world[i+1][j-1]->free && world[i][j-1]->free) ///slipping from the edge
            {
                world[i][j]->dispatchShift(*this, 0, -1);
            }
            else if(i < rows-1 && j < columns-1 && world[i+1][j+1]->free && world[i][j+1]->free) ///slipping from the edge
            {
                world[i][j]->dispatchShift(*this, 0, 1);
            }
            else ///resting on the ground
            {
                world[i][j]->dispatchShift(*this, 0, 0);
            }
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

void Simulation::simulate(int duration)
{
    system("clear");
    DEBUG COUT << "first print" << ENDL;
    print();
    for(int t = 0; t < duration; ++t)
    {
        calculate();
        usleep(150000);                  ///CYCLE TIME
        system("clear");
        DEBUG COUT << VAR(t) << ENDL << ENDL;
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
            else
            {
                cout << "Unknown symbol on input: row " << i << ", column " << j << ". Exiting..." << endl;
                exit(0);
            }
        }
    }
    DEBUG COUT << "INPUT DONE" << VAR(columns) << VAR(rows)<< ENDL;
}

Simulation::Simulation(int seed /*= 1*/)
{
    srand(seed);
}

