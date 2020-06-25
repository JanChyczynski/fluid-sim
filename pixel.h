#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

///from "simulation.h"
class Simulation;

struct Pixel
{
    std::string icon = " ";
    bool moved = 0, free = 0, solid = 0;
    virtual void resetMoved(bool newMoved=0);

    virtual bool dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y);

    virtual void setDebugIcon();

    Pixel();
    virtual ~Pixel();
};


struct Air : public Pixel
{
    bool dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y) override;

    Air();
};

struct Earth : public Pixel
{
    bool dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y) override;

    Earth(int iconType = 0);
};

struct Water : public Pixel
{
    int vx = 0, vy = 0;//, energyX = 0, energyY = 0;


    bool dispatchShift(Simulation& sim, int deltavx, int deltavy, int x, int y) override;

    void resetMoved(bool newMoved = 0) override;

    void setDebugIcon() override;

    Water(int argvx = 0, int argvy = 0, int iconType = 0);
};
