//
//  main.cpp
//  Lab4-UI
//
//  Created by Amado Garcia on 11/5/20.
//

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Raycaster.h"
#include <iomanip>
#include <fstream>
using namespace std;    

int main(int argc, const char * argv[]) {
    Raycaster r(800, 600, "Game");
    r.load_map("map.txt");
    r.render();
    return 0;
}
