#include <iostream>
#include <SDL.h>
#include "Raycaster.h"
#include <iomanip>
#include <fstream>
using namespace std;

int main()
{
    Raycaster r(1000, 500, "Levels");
    r.load_map("map.txt");
    r.render();
}
