#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include "simulation.h"
#include "pixel.h"


#define DEBUG if (1)
#define COUT cout << "\e[36m"
#define VAR(v) " [\e[32m" << #v << "\e[36m=\e[91m" << v << "\e[36m] "
#define ENDL "\e[39m" << endl

using namespace std;

int main()
{
    int duration;
    cin >> duration;

    Simulation mySim(16);

    int iconType = 0; ///use 2 if your terminal doesn't support ANSI
    mySim.input(iconType);
    mySim.simulate(duration, iconType);

    return 0;
}
