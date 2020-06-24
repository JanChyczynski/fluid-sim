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

bool Pixel::dispatchShift(Simulation& sim, int deltavx, int deltavy)
{
    return sim.shift(*this, deltavx, deltavy);
}

Pixel::Pixel(int argx/*= -1*/, int argy/*= -1*/): x(argx), y(argy){}
Pixel::~Pixel() = default;

///AIR
bool Air::dispatchShift(Simulation& sim, int deltavx, int deltavy)
{
    return sim.shift(*this, deltavx, deltavy);
}

Air::Air(int argx/*= -1*/, int argy/*= -1*/): Pixel(argx, argy)
{
    icon = " ";
    free = 1;
}

///EARTH
bool Earth::dispatchShift(Simulation& sim, int deltavx, int deltavy)
{
    return sim.shift(*this, deltavx, deltavy);
}

Earth::Earth(int argx/*= -1*/, int argy/*= -1*/): Pixel(argx, argy)
{
    icon = "\e[91m█";
    solid = 1;
}

///WATER
bool Water::dispatchShift(Simulation& sim, int deltavx, int deltavy)
{
    return sim.shift(*this, deltavx, deltavy);
}

void Water::resetMoved(bool newMoved/*= 0*/)
{
    moved = newMoved;
    //DEBUG COUT << "RESET WATER" << ENDL;
}

Water::Water(int argx/*= -1*/, int argy/*= -1*/, int argvx/*= 0*/, int argvy/*= 0*/): Pixel(argx, argy), vx(argvx), vy(argvy)
{
    icon = "\e[36m█";
}

