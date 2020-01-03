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
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int timeLimit;
    cin >> timeLimit;

    Simulation mySim;
    mySim.input();
    mySim.simulate(timeLimit);

    return 0;
}
