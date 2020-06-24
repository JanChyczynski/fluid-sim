#include "simulation.h"
#include "pixel.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>


#define DEBUG if (1)
#define COUT cout << "\e[36m"
#define VAR(v) " [\e[32m" << #v << "\e[36m=\e[91m" << v << "\e[36m] "
#define ENDL "\e[39m" << endl

using namespace std;

///PIXEL
void Pixel::resetMoved(bool newMoved/*=0*/){/*DEBUG COUT << "RESET PIXEL" << ENDL;*/}

bool Pixel::dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y)
{
    return sim.shift(*this, deltavx, deltavy, x, y);
}

void Pixel::setDebugIcon(){}

Pixel::Pixel(){}
Pixel::~Pixel() = default;

///AIR
bool Air::dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y)
{
    return sim.shift(*this, deltavx, deltavy, x, y);
}

Air::Air(): Pixel()
{
    icon = " ";
    free = 1;
}

///EARTH
bool Earth::dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y)
{
    return sim.shift(*this, deltavx, deltavy, x, y);
}

Earth::Earth(): Pixel()
{
    icon = "\e[91m█";
    solid = 1;
}

///WATER
bool Water::dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y)
{
    return sim.shift(*this, deltavx, deltavy, x, y);
}

void Water::resetMoved(bool newMoved/*= 0*/)
{
    moved = newMoved;
    //DEBUG COUT << "RESET WATER" << ENDL;
}

void Water::setDebugIcon()
{
    icon = (vx >= 0)? "\e[36m" : "\e[37m";
    icon += ((abs(vx) > 9)?"+" : to_string(abs(vx)));
}

Water::Water(int argvx/*= 0*/, int argvy/*= 0*/): Pixel(), vx(argvx), vy(argvy)
{
    icon = "\e[36m█";
}

