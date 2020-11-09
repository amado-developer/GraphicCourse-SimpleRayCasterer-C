#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Raycaster.h"
#include <iomanip>
#include <fstream>
using namespace std;
int main()
{
    Raycaster r(1000, 500, "Game");
    r.load_map("map.txt");
    r.render_main_menu("./font.ttf", 40, "Press any key to enter!", {255, 255, 255, 255}, 320, 200);
    return 0;
}
