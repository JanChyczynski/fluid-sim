#include "simulation.h"
#include "pixel.h"

#include <bits/stdc++.h> /*
#include <iostream>
#include <vector>
#include <string>
#include <memory>*/


#define DEBUG if (1)
#define COUT cout << "\e[36m"
#define VAR(v) " [\e[32m" << #v << "\e[36m=\e[91m" << v << "\e[36m] "
#define ENDL "\e[39m" << endl

using namespace std;

///PIXEL
void Pixel::resetMoved(bool newMoved/*=0*/){/*DEBUG COUT << "RESET PIXEL" << ENDL;*/}

bool Pixel::dispatchShift(Simulation& sim, int dvx, int dvy)
{
    return sim.shift(*this, dvx, dvy);
}

Pixel::Pixel(int argx/*= -1*/, int argy/*= -1*/, int argindex/*= -1*/): x(argx), y(argy), index(argindex){}
Pixel::~Pixel() = default;

///AIR
bool Air::dispatchShift(Simulation& sim, int dvx, int dvy)
{
    return sim.shift(*this, dvx, dvy);
}

Air::Air(int argx/*= -1*/, int argy/*= -1*/, int argindex/*= -1*/): Pixel(argx, argy, argindex)
{
    icon = " ";
}

///EARTH
bool Earth::dispatchShift(Simulation& sim, int dvx, int dvy)
{
    return sim.shift(*this, dvx, dvy);
}

Earth::Earth(int argx/*= -1*/, int argy/*= -1*/, int argindex/*= -1*/): Pixel(argx, argy, argindex)
{
    icon = "\e[91m█";
}

///WATER
bool Water::dispatchShift(Simulation& sim, int dvx, int dvy)
{
    return sim.shift(*this, dvx, dvy);
}

void Water::resetMoved(bool newMoved/*= 0*/)
{
    moved = newMoved;
    //DEBUG COUT << "RESET WATER" << ENDL;
}

Water::Water(int argx/*= -1*/, int argy/*= -1*/, int argindex/*= -1*/, int argvx/*= 0*/, int argvy/*= 0*/): Pixel(argx, argy, argindex), vx(argvx), vy(argvy)
{
    icon = "\e[36m█";
}

