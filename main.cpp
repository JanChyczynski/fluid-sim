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
    mySim.input();
    mySim.simulate(duration);

    return 0;
}
