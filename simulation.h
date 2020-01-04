///high-velocity branch
#pragma once

#include <bits/stdc++.h> /*
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>*/
//#include "pixel.h"


///from "pixel.h"
struct Pixel;
struct Air;
struct Earth;
struct Water;

class Simulation
{
public:
    int rows = 0, columns = 0;

    std::vector<std::vector<std::unique_ptr<Pixel>>> world;

    bool checkIndexes(int x, int y);

    inline int normV(int v);

    bool shift(Pixel& curPix, int dvx, int dvy);

    bool shift(Air& curPix, int dvx, int dvy);

    bool shift(Earth& curPix, int dvx, int dvy);

    bool shift(Water& curPix, int dvx, int dvy);

    void clearMoved();

    bool checkAllCoords();

    void calculate();

    void print();

    void simulate(int timeLimit);

    void input();

    Simulation(int seed = 1);
};
