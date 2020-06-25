#include <iostream>
#include <iomanip>
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
    return ((v <= -1)? -1 : ((v >= 1)? 1 : 0));  ///so the velocity belongs to <-1, 1> and we only move pixels by 1
}

bool Simulation::shift(Pixel& currentPixel, int deltavx, int deltavy, int x, int y){DEBUG COUT << "PIXEL SHIFT" << VAR(x) << VAR(y) <<VAR(currentPixel.icon) << ENDL;
//currentPixel.resetMoved();
return false;
}/**/

bool Simulation::shift(Air& currentPixel, int deltavx, int deltavy, int x, int y)
{
    DEBUG COUT << "AIR SHIFT" << ENDL;
    return true;
}

bool Simulation::shift(Earth& currentPixel, int deltavx, int deltavy, int x, int y)
{
    DEBUG COUT << "EARTH SHIFT" << ENDL;
    return false;
}

bool Simulation::shift(Water& currentPixel, int deltavx, int deltavy, int x, int y) ///It doesn't obey physics  | x - row, y - column, (0, 0) is top left corner
{
    if(currentPixel.moved)
    {
        return false;
    }

    currentPixel.moved = 1;

    currentPixel.vx += deltavx;
    currentPixel.vy += deltavy;

    DEBUG COUT << "WATERSHIFT" << ENDL;
    DEBUG COUT << "Shifting " << VAR(x) << VAR(y)  << VAR(currentPixel.vx) << VAR(currentPixel.vy) << ENDL;

    if(world[x+normV(currentPixel.vx)][y+normV(currentPixel.vy)]->moved == 0 &&  world[x+normV(currentPixel.vx)][y+normV(currentPixel.vy)]->dispatchShift(*this, (currentPixel.vx)/2+1, (currentPixel.vy)/2, x+normV(currentPixel.vx), y+normV(currentPixel.vy))) ///destination was free or space were made
    {
        DEBUG COUT << "ACTUALLY SHIFTED" << VAR(x) << VAR(y) << VAR(currentPixel.vx) << VAR(currentPixel.vy) << ENDL;
        DEBUG if(checkIndices(x+normV(currentPixel.vx), y+normV(currentPixel.vy)) == 0 || checkIndices(x, y) == 0)
            COUT << "INCORRECT INDICES!" << ENDL;
        swap(world[x][y], world[x+normV(currentPixel.vx)][y+normV(currentPixel.vy)]);

        currentPixel.vx = (currentPixel.vx)/2;///energy is divided between pushing and pushed object
        currentPixel.vy = (currentPixel.vy)/2;
        currentPixel.moved = 1;

        return true;
    }
    else  ///DEFLECTION / REDIRECTION
    {
        ///90 deg deflection
        int directionChangeX = 1;
        int directionChangeY = -1;
        if(rand() %2)  ///randomizes which direction is tried first
            swap(directionChangeX, directionChangeY);

        if(world[x+normV(currentPixel.vx)][y+normV(currentPixel.vy)]->solid) ///losing enery from collision with solid object
        {
            if(abs(currentPixel.vx) > 2) currentPixel.vx /= 2;
            if(abs(currentPixel.vy) > 2) currentPixel.vy /= 2;
        }
        else ///losing energy from other collisions
        {
            currentPixel.vx -= currentPixel.vx/3;
            currentPixel.vy -= currentPixel.vy/3;
        }

        for(int redirectionIteration = 0; redirectionIteration < 4 && (abs(currentPixel.vx) > 0 || abs(currentPixel.vy) > 0); redirectionIteration++, swap(directionChangeX, directionChangeY)) ///redirectionIteration: 0, 1 redirecting at free box; 2, 3 redirecting and pushing
        {
            if(redirectionIteration < 2 && world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]->free == 0)///at first try redirecting on a free box
            {
                continue;
            }
            if(world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]->moved == 0 && world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]->dispatchShift(*this, (currentPixel.vy/2)*directionChangeX+1, (currentPixel.vx/2)*directionChangeY, x+normV(currentPixel.vy*directionChangeX), y+normV(currentPixel.vx*directionChangeY)))
            {
                DEBUG COUT << "ACTUALLY REDIRECTED AND SHIFTED" << VAR(x) << VAR(y) << VAR(currentPixel.vy*directionChangeX) << VAR(currentPixel.vx*directionChangeY) << ENDL;
                DEBUG if(checkIndices(x+currentPixel.vy*directionChangeX, y+currentPixel.vx*directionChangeY) == 0 || checkIndices(x, y) == 0)
                    COUT << "INCORRECT INDICES!" << ENDL;


                swap(world[x][y], world[x+normV(currentPixel.vy*directionChangeX)][y+normV(currentPixel.vx*directionChangeY)]);

                currentPixel.vx = currentPixel.vy*directionChangeX;
                currentPixel.vy = currentPixel.vx*directionChangeY;

                currentPixel.vx /=2; ///energy is divided between pushing and pushed object
                currentPixel.vy /=2;

                currentPixel.moved = 1;
                return true;
            }

        }

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


void Simulation::calculate()
{
    clearMoved();
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            if(i < rows-1 && world[i+1][j]->solid == 0) ///free fall
            {
                world[i][j]->dispatchShift(*this, 1, 0, i, j);
            }
            else if(i < rows-1 && j > 0 && world[i+1][j-1]->free && world[i][j-1]->free) ///slipping off the edge
            {
                world[i][j]->dispatchShift(*this, 0, -1, i, j);
            }
            else if(i < rows-1 && j < columns-1 && world[i+1][j+1]->free && world[i][j+1]->free) ///slipping off the edge
            {
                world[i][j]->dispatchShift(*this, 0, 1, i, j);
            }
            else ///resting on the ground
            {
                world[i][j]->dispatchShift(*this, 0, 0, i, j);
            }
            DEBUG COUT << ENDL;
        }
    }
    DEBUG COUT << "CALC DONE" << ENDL << ENDL;
}


void Simulation::print(int iconType)//█
{
    for(int i = 0; i < rows; ++i)
    {
        DEBUG cout << setw(2) << i;
        for(int j = 0; j < columns; ++j)
        {
            if(iconType == 1) world[i][j]->setDebugIcon();

            cout << world[i][j]->icon;
        }
        cout << endl;
    }
}

void Simulation::simulate(int duration, int iconType /*= 0*/) ///0 - blocks, 1 - vertical velocity info, 2 - no ANSI
{
    system("clear");
    DEBUG COUT << "first print" << ENDL;
    print(iconType);
    for(int t = 0; t < duration; ++t)
    {
        calculate();
        usleep(50000);                  ///CYCLE TIME
        system("clear");
        DEBUG COUT << VAR(t) << ENDL << ENDL;
        print(iconType);
        DEBUG usleep(3000000);
    }
}

void Simulation::input(int iconType /*= 0*/)
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
                world[i][j] = make_unique<Water>(0, 0, iconType);
            }
            else if(ch == 'E')
            {
                world[i][j] = make_unique<Earth>(iconType);
            }
            else if(ch == '.')
            {
                world[i][j] = make_unique<Air>();
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

