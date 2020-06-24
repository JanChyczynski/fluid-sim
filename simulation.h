///high-velocity branch
#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>


///from "pixel.h"
struct Pixel;
struct Air;
struct Earth;
struct Water;

class Simulation
{
private:
    int rows = 0, columns = 0;

    std::vector<std::vector<std::unique_ptr<Pixel>>> world;

    bool checkIndices(int x, int y);

    inline int normV(int v);
public:
    bool shift(Pixel& currentPixel, int deltavx, int deltavy, int x, int y);

    bool shift(Air& currentPixel, int deltavx, int deltavy, int x, int y);

    bool shift(Earth& currentPixel, int deltavx, int deltavy, int x, int y);

    bool shift(Water& currentPixel, int deltavx, int deltavy, int x, int y);
private:
    void clearMoved();

    void calculate();

    void print();
public:
    void simulate(int timeLimit);

    void input();

    Simulation(int seed = 1);
};
