#pragma once

#include <bits/stdc++.h> /*
#include <iostream>
#include <vector>
#include <string>
#include <memory>*/
//#include "simulation.h"

///from "simulation.h"
class Simulation;

struct Pixel
{
    std::string icon = " ";
    int x, y, index;
    bool moved = 0, free = 0;
    virtual void resetMoved(bool newMoved=0);

    virtual bool dispatchShift(Simulation& sim, int dvx, int dvy);

    Pixel(int argx = -1, int argy = -1, int argindex = -1);
    virtual ~Pixel();
};


struct Air : public Pixel
{
    bool dispatchShift(Simulation& sim, int dvx, int dvy) override;

    Air(int argx = -1, int argy = -1, int argindex = -1);
};

struct Earth : public Pixel
{
    bool dispatchShift(Simulation& sim, int dvx, int dvy) override;

    Earth(int argx = -1, int argy = -1, int argindex = -1);
};

struct Water : public Pixel
{
    int vx = 0, vy = 0;//, energyX = 0, energyY = 0;


    bool dispatchShift(Simulation& sim, int dvx, int dvy) override;

    void resetMoved(bool newMoved = 0) override;

    Water(int argx = -1, int argy = -1, int argindex = -1, int argvx = 0, int argvy = 0);
};
